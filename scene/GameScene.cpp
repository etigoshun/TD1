#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#include "MyMath.h"
#include <fstream>

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
//回転角用の乱数範囲を設定
std::uniform_real_distribution<float> spawn(1, 20);
//座標用の乱数範囲を設定
std::uniform_real_distribution<float> translation(-10, 10);

GameScene::GameScene() {}

GameScene::~GameScene() {
	//3Dモデルの削除
	delete model_;
	//デバッグカメラの削除
	delete debugCamera_;
	//3Dモデルの削除
	delete modelSkydome_;
	//プレイヤーの削除
	delete player_;
	//プレイヤーモデルの削除
	delete playerModel_;
	//敵モデルの削除
	delete enemyModel_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	//playerTextureHandle_ = TextureManager::Load("player.png");
	playerTextureHandle_ = TextureManager::Load("kamihikouki/kamihikouki.png");
	enemyTextureHandle_ = TextureManager::Load("cloud/cloud.jpg");
	enemyTextureHandle1_ = TextureManager::Load("enemy.png");
	//skydomeTextureHandle_ = TextureManager::Load("夏の河原（日中）.jpg");

	//3Dモデルの生成
	model_ = Model::Create();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	playerModel_ = Model::CreateFromOBJ("kamihikouki", true);

	enemyModel_ = Model::CreateFromOBJ("cloud", true);;
	//enemyModel_ = Model::CreateFromOBJ("cloud1", true);;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(600, 400);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(playerModel_, playerTextureHandle_);

	//天球の初期化
	newSkydome->Initialize(modelSkydome_);

	Vector3 cameraPos(0, 0, -10);
	Vector3 cameraRot(0, 0, 0);
	//レールカメラの初期化
	newRailCamera->Initialize(cameraPos, cameraRot);

	//自キャラとレールカメラの親子関係を結ぶ
	//player_->SetParent(newRailCamera->GetWorldMatrix());

	LoadEnemyPopData();
}

void GameScene::Update() {
	//デバッグカメラの更新
	//debugCamera_->Update();

	//自キャラの更新
	player_->Update();

	//敵キャラの更新
	for (const std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
	}

	//デスフラグの立った弾を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return enemy->IsDead();
		});

	////デスフラグの立った弾を削除
	//enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
	//	{
	//		return bullet->IsDead();
	//	});

	////弾更新
	//for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_)
	//{
	//	bullet->Update();
	//}

	//当たり判定
	CheckAllCollisions();

	//天球の更新
	newSkydome->Update();

	//レールカメラの更新
	newRailCamera->Update();

	//
	UpdateEnemyPopCommands();

	//レールカメラをゲームシーンのカメラに適応する
	viewProjection_.matView = newRailCamera->GetViewProjection().matView;
	viewProjection_.matProjection = newRailCamera->GetViewProjection().matProjection;
	//ビュープロジェクションの転送
	viewProjection_.TransferMatrix();

	debugText_->SetPos(0, 0);
	debugText_->Printf("isDead:%d", enemy_->IsDead());

	debugText_->SetPos(0, 20);
	debugText_->Printf("position:%f", enemy_->GetTransPosition().z);

#ifdef  _DEBUG
	if (input_->TriggerKey(DIK_SPACE))
	{
		if (isDebugCameraActive_ == false)
		{
			isDebugCameraActive_ = true;
		}
		else if (isDebugCameraActive_ == true)
		{
			isDebugCameraActive_ = false;
		}

		
	}
#endif //  _DEBUG

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->DrawLine3d({ 0, -1, 0 }, { 0, 5, 0 }, {255, 255, 255, 255});

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵キャラの描画
	for (const std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw(viewProjection_);
	}

	////敵弾の描画
	//for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_)
	//{
	//	bullet->Draw(viewProjection_);
	//}


	//天球の描画
	newSkydome->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//距離を図るための変数
	double distance;

	//半径
	float radius;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	//const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();


	//自キャラと敵弾の当たり判定
#pragma region
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_)
	{
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		distance = CalculateDistance(posA, posB);

		radius = player_->radius + bullet->radius;

		//球と球の当たり判定
		if (distance <= radius)
		{
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion


	//自弾と敵キャラの当たり判定
#pragma region
	//自キャラの座標
	posA = enemy_->GetWorldPosition();

	//敵キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		//自弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		distance = CalculateDistance(posA, posB);

		radius = player_->radius + bullet->radius;

		//球と球の当たり判定
		if (distance <= radius)
		{
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion


	//自弾と敵弾の当たり判定
#pragma region
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		//自キャラの座標
		posA = playerBullet->GetWorldPosition();

		//敵キャラと敵弾全ての当たり判定
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_)
		{
			//自弾の座標
			posB = enemyBullet->GetWorldPosition();

			//座標AとBの距離を求める
			distance = CalculateDistance(posA, posB);

			radius = playerBullet->radius + enemyBullet->radius;

			//球と球の当たり判定
			if (distance <= radius)
			{
				//敵キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

//敵の追加(普通)
void GameScene::AddNormalEnemy(Vector3 pos)
{
	// スマートポインタ (使いやすいのがユニークポインタ)
	std::unique_ptr<Enemy>enemy = std::make_unique<Enemy>();

	//敵の弾の速度
	const float kBulletSpeed = -0.1f;
	Vector3 velocity(0, 0, kBulletSpeed);
	//敵キャラの初期位置
	Vector3 enemyPosition = pos;
	
	enemy->SetPlayer(player_);
	//敵の初期化
	enemy->Initialize(enemyModel_, enemyTextureHandle_, pos, velocity);
	//敵キャラにゲームシーンのアドレスを渡す
	enemy->SetGameScene(this);

	//リストに登録する
	enemys_.push_back(std::move(enemy));
}

//敵の追加(速い)
void GameScene::AddSpeedyEnemy(Vector3 pos)
{
	// スマートポインタ (使いやすいのがユニークポインタ)
	std::unique_ptr<Enemy>enemy = std::make_unique<Enemy>();

	//敵の弾の速度
	const float kBulletSpeed = -0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);
	//敵キャラの初期位置
	Vector3 enemyPosition(30, 10, 100);

	enemy->SetPlayer(player_);
	//敵の初期化
	enemy->Initialize(model_, enemyTextureHandle1_, pos, velocity);
	//敵キャラにゲームシーンのアドレスを渡す
	enemy->SetGameScene(this);

	//リストに登録する
	enemys_.push_back(std::move(enemy));
}

//敵の追加(雲)
void GameScene::AddCloudEnemy(Vector3 pos)
{
	// スマートポインタ (使いやすいのがユニークポインタ)
	std::unique_ptr<Enemy>enemy = std::make_unique<Enemy>();

	//敵の弾の速度
	const float kBulletSpeed = -0.1f;
	Vector3 velocity(0, 0, kBulletSpeed);
	//敵キャラの初期位置
	Vector3 enemyPosition(30, 10, 100);

	enemy->SetPlayer(player_);
	//敵の初期化
	enemy->Initialize(model_, enemyTextureHandle1_, pos, velocity);
	//敵キャラにゲームシーンのアドレスを渡す
	enemy->SetGameScene(this);

	//リストに登録する
	enemys_.push_back(std::move(enemy));
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//リストに登録する
	enemyBullets_.push_back(std::move(enemyBullet));
}

//敵発生データの読み込み
void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyWait.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

////敵発生コマンドの更新
//void GameScene::UpdateEnemyPopCommands()
//{
//	//待機処理
//	if (enemyWaitFlag)
//	{
//		enemyWaitTimer_--;
//
//		if (enemyWaitTimer_ <= 0)
//		{
//			enemyWaitFlag = false;
//		}
//		return;
//	}
//
//	//1行分の文字列を入れる変数
//	std::string line;
//
//	//コマンド実行ループ
//	while (getline(enemyPopCommands, line))
//	{
//		//1行分の文字列をストリームに変換して解析しやすくする
//		std::istringstream line_stream(line);
//
//		std::string word;
//		//,区切りで行の先頭文字列を取得
//		getline(line_stream, word, ',');
//
//		// "//"から始まる行はコメント
//		if (word.find("//") == 0)
//		{
//			//コメント行を飛ばす
//			continue;
//		}
//
//		//NORMALコマンド
//		if (word.find("NORMAL") == 0)
//		{
//			//x座標
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//
//			//y座標
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//
//			//z座標
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//			//敵(普通)を発生させる
//			AddNormalEnemy(Vector3(x, y, z));
//		}
//		//SPEEDYコマンド
//		if (word.find("SPEEDY") == 0)
//		{
//			//x座標
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//
//			//y座標
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//
//			//z座標
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//			//敵(速い)を発生させる
//			AddSpeedyEnemy(Vector3(x, y, z));
//		}
//		//CLOUDコマンド
//		if (word.find("CLOUD") == 0)
//		{
//			//x座標
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//
//			//y座標
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//
//			//z座標
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//			//敵(雲)を発生させる
//			AddCloudEnemy(Vector3(x, y, z));
//		}
//		//WAITコマンド
//		else if (word.find("WAIT") == 0)
//		{
//			getline(line_stream, word, ',');
//
//			//待ち時間
//			int32_t waitTime = atoi(word.c_str());
//
//			//待機開始
//			enemyWaitFlag = true;
//			enemyWaitTimer_ = waitTime;
//
//			//コマンドループを抜ける
//			break;
//		}
//	}
//
//
//}

//敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands()
{
	//1行分の文字列を入れる変数
	std::string line;

	//待機処理
	if (enemyWaitFlag == true && normalDeathFlag == true)
	{
		enemyWaitTimer_--;

		if (enemyWaitTimer_ <= 0)
		{
			enemyWaitFlag = false;
		}
		return;
	}

	if (enemySpawnTimer <= 180)
	{
		enemySpawnTimer--;
		Vector3 pos = EnemySpawnRand();

		if (enemySpawnTimer <= 0)
		{
			AddNormalEnemy(pos);

			enemySpawnTimer = 180;
		}
		//敵のZ座標が-20に到達したら敵を消す
		if (enemy_->GetTransPosition().z <= -20)
		{
			enemy_->OnCollision();
		}
	}

	

	//普通の敵
	if (normalFlag == true)
	{
		Vector3 pos = EnemySpawnRand();

		//AddNormalEnemy(pos);

		//敵のZ座標が-50に到達したら敵を消す
		if (pos.z <= -20)
		{
			enemyWaitTimer_ = 120;
			enemyDeathCount += 1;
			normalFlag = false;
			normalDeathFlag = true;
			enemy_->OnCollision();
		}
	}
	//速い敵
	if (speedyFlag == true)
	{
		Vector3 pos = EnemySpawnRand();

		AddSpeedyEnemy(pos);

		//敵のZ座標が-50に到達したら敵を消す
		if (pos.z <= -50)
		{
			enemyDeathCount += 1;
			speedyFlag = false;
		}
	}
	//雲の敵
	if (cloudFlag == true)
	{
		Vector3 pos = EnemySpawnRand();

		AddCloudEnemy(pos);

		//敵のZ座標が-50に到達したら敵を消す
		if (pos.z <= -50)
		{
			enemyDeathCount += 1;
			cloudFlag = false;
		}
	}
	//

	//コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		if (enemyDeathCount == 5)
		{
			//敵の発生間隔
			if (word.find("WAIT") == 0)
			{
				getline(line_stream, word, ',');

				//待ち時間
				int32_t waitTime = atoi(word.c_str());

				//待機開始
				enemyWaitFlag = true;
				enemyWaitTimer_ = waitTime;

				//コマンドループを抜ける
				break;
			}
		}
	}

	
}

Vector3 GameScene::EnemySpawnRand()
{
	int value = spawn(engine);

	Vector3 pos{ 0,0,0 };

	if (value >= 1 && value <= 5)
	{
		pos = a;
	}
	else if (value >= 6 && value <= 10)
	{
		pos = b;
	}
	else if (value >= 11 && value <= 15)
	{
		pos = c;
	}
	else if (value >= 16 && value <= 20)
	{
		pos = d;
	}

	return pos;
}