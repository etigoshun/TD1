#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>
#include "Affine.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>


/// <summary>
/// 自キャラ
/// </summary>
class Player
{
	//メンバ関数
public:
		
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texturehandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//親子関係を結ぶセッター
	void SetParent(WorldTransform* worldTransform)
	{
		worldTransform_.parent_ = worldTransform;
	}

public:
	//半径
	const float radius = 1.0f;

	//メンバ変数
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	
	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//入力処理するため
	Input* input_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	int num = 0;

	int mode = 0;

	int count = 0;

	Vector3 a = { 20,10,50 };
	Vector3 b = { 20,-10,50 };
	Vector3 c = { -20,-10,50 };
	Vector3 d = { -20,10,50 };
};

