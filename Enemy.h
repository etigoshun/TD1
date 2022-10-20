#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include <cassert>
#include "EnemyBullet.h"
#include <memory>
#include <list>
#include "DebugText.h"

//自機クラスの前方宣言
class Player;

//GameSceneの前方宣言
class GameScene;

//行動フェーズ
enum class Phase
{
	Approach,	//接近する
	Leave,		//離脱する
};

class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texturehandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle,const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	//接近フェーズ初期化
	void ApproachInitialize();

	//プレイヤーのセッター
	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//ゲームシーンのセッター
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//座標を取得
	Vector3 GetTransPosition();

public:
	//発射間隔
	static const int kFireInterval = 60;

	//半径
	const float radius = 1.0f;

	bool IsDead() const { return isDead_; }

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	//std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射タイマー
	int32_t kFireTimerCount = 0;

	//自キャラ
	Player* player_ = nullptr;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	bool isDead_ = false;
};

