#include "Enemy.h"
#include "Player.h"
#include "MyMath.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position, const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	//接近フェーズ初期化
	ApproachInitialize();
}

void Enemy::Update()
{
	////デスフラグの立った弾を削除
	//bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
	//	{
	//		return bullet->IsDead();
	//	});

	//敵の行動
	switch (phase_)
	{
		case Phase::Approach:
		default:
			//移動(ベクトルを加算)
			worldTransform_.translation_ += velocity_;

			////発射タイマーをデクリメント
			//kFireTimerCount--;

			////指定時間に到達した
			//if (kFireTimerCount < 0)
			//{
			//	//弾を発射
			//	Fire();

			//	//発射タイマーを初期化
			//	kFireTimerCount = kFireInterval;
			//}

			////規定の位置に到達したら離脱
			//if (worldTransform_.translation_.z < 0.0f)
			//{
			//	phase_ = Phase::Leave;
			//}
			break;

		//case Phase::Leave:
		//	//移動(ベクトルを加算)
		//	worldTransform_.translation_ += -velocity_;

		//	//発射タイマーをデクリメント
		//	kFireTimerCount--;

		//	//指定時間に到達した
		//	if (kFireTimerCount < 0)
		//	{
		//		//弾を発射
		//		Fire();

		//		//発射タイマーを初期化
		//		kFireTimerCount = kFireInterval;
		//	}

		//	//規定の位置に到達したら離脱
		//	if (worldTransform_.translation_.z > 100.0f)
		//	{
		//		phase_ = Phase::Approach;
		//	}
		//	break;
	}

	//行列の更新
	worldTransform_.matWorld_ = CreateIdentity();
	worldTransform_.matWorld_ *= CreateScale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= CreateRotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateTrans(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	/*debugText_->SetPos(50, 150);
	debugText_->Printf("rotation:(%d)", kFireTimerCount);*/

	//キャラクターの座標を画面に表示する
	debugText_->SetPos(50, 150);
	debugText_->Printf("translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);


	////弾更新
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	//{
	//	bullet->Update();
	//}
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	////弾を描画
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	//{
	//	bullet->Draw(viewProjection);
	//}
}

void Enemy::Fire()
{
	assert(player_);

	//弾の速度
	const float kBulletSpeed = -0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//自キャラのワールド座標を取得する
	Vector3 goal(0, 0, 0);
	 goal = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	 Vector3 start(0, 0, 0);
	start = this->GetWorldPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	Vector3 difVec(0, 0, 0);
	difVec = goal;
	difVec -= start;
	//ベクトルの正規化
	Normalize(difVec);
	//ベクトルの長さを、速さに合わせる
	velocity = difVec;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	gameScene_->AddEnemyBullet(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	//発射タイマーを初期化
	kFireTimerCount = kFireInterval;
}

//ワールド座標を取得
Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//衝突を検出したら呼び出されるコールバック関数
void Enemy::OnCollision()
{
	isDead_ = true;
}

Vector3 Enemy::GetTransPosition()
{
	//ワールド座標を入れる変数
	Vector3 transPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	transPos.x = worldTransform_.translation_.x;
	transPos.y = worldTransform_.translation_.y;
	transPos.z = worldTransform_.translation_.z;

	return transPos;
}
