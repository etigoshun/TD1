#include "Player.h"
#include "MyMath.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="texturehandle">テクスチャハンドル</param>
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	//worldTransform_.translation_.z = 50;
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.translation_ = { 0, 0, 0 };
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	////デスフラグの立った弾を削除
	//bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
	//	{
	//		return bullet->IsDead();
	//	});

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	////移動ベクトルを変更する処理
	//if (input_->PushKey(DIK_RIGHT))
	//{
	//	move.x = 0.1f;
	//}
	//else if (input_->PushKey(DIK_LEFT))
	//{
	//	move.x = -0.1f;
	//}

	//if (input_->PushKey(DIK_UP))
	//{
	//	move.y = 0.1f;
	//}
	//else if (input_->PushKey(DIK_DOWN))
	//{
	//	move.y = -0.1f;
	//}

	////座標移動
	//worldTransform_.translation_ += move;

	//プレイヤー移動処理
	switch (mode)
	{
		case 0:
			if (input_->TriggerKey(DIK_A)) {
				if (num == 0) {
					worldTransform_.translation_ = b;
					count = 0;
				}
				else if (num == 1) {
					worldTransform_.translation_ = c;
					count = 0;
				}
				else if (num == 2) {
					worldTransform_.translation_ = d;
					count = 0;
				}
				else if (num == 3) {
					worldTransform_.translation_ = a;
					count = 0;
				}
				num += 1;
				if (num >= 4) {
					num = 0;
				}
			}
			break;
		case 1:
			if (input_->TriggerKey(DIK_A)) {
				if (num == 0) {
					worldTransform_.translation_ = d;
					count = 0;
				}
				else if (num == 1) {
					worldTransform_.translation_ = a;
					count = 0;
				}
				else if (num == 2) {
					worldTransform_.translation_ = b;
					count = 0;
				}
				else if (num == 3) {
					worldTransform_.translation_ = c;
					count = 0;
				}
				num -= 1;
				if (num < 0) {
					num = 3;
				}
			}
			break;
	}
	if (input_->PushKey(DIK_A)) {
		count++;
		if (count >= 100) {
			if (mode == 0) {
				mode += 1;
				count = 0;
			}
			else if (mode == 1) {
				mode -= 1;
				count = 0;
			}
		}

	}

	

	//移動限界処理
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 17.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//行列の更新
	worldTransform_.matWorld_ = CreateIdentity();
	worldTransform_.matWorld_ *= CreateScale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= CreateRotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateRotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateTrans(worldTransform_.translation_);

	//親子関係
	//worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();

	////キャラクターの座標を画面に表示する
	//debugText_->SetPos(50, 150);
	//debugText_->Printf("translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	//自キャラの回転処理
	Vector3 turn = { 0,0.01f,0 };

	if (input_->PushKey(DIK_J))
	{
		worldTransform_.rotation_ -= turn;
	}
	else if (input_->PushKey(DIK_U))
	{
		worldTransform_.rotation_ += turn;
	}

	/*debugText_->SetPos(50, 125);
	debugText_->Printf("rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);*/

	////キャラクター攻撃処理
	//Attack();

	////弾更新
	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	//{
	//	bullet->Update();
	//}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw(ViewProjection& viewProjection)
{
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	////弾を描画
	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	//{
	//	bullet->Draw(viewProjection);
	//}
}

/// <summary>
/// 攻撃
/// </summary>
void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//弾の速度
		const float kBulletSpeed = 0.4f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = Transform(velocity, worldTransform_);

		Vector3 position = GetWorldPosition();

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

//ワールド座標を取得
Vector3 Player::GetWorldPosition()
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
void Player::OnCollision()
{
	//何もしない
}
