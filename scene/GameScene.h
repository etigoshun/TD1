#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <memory>
#include <list>
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	//敵の追加
	void AddNormalEnemy(Vector3 pos);

	//敵の追加(速い)
	void AddSpeedyEnemy(Vector3 pos);

	//敵の追加(雲)
	void AddCloudEnemy(Vector3 pos);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	//ランダム
	Vector3 EnemySpawnRand();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//プレイヤーのテクスチャハンドル
	uint32_t playerTextureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//自キャラ
	Player* player_ = nullptr;

	//自弾
	//PlayerBullet* playerBullet_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//敵
	//Enemy* enemy_ = nullptr;

	//敵のテクスチャハンドル
	uint32_t enemyTextureHandle_ = 0;

	//敵のテクスチャハンドル1
	uint32_t enemyTextureHandle1_ = 0;


	//天球
	//std::list<std::unique_ptr<Skydome>> skydome_;

	//	天球を生成し、初期化
	std::unique_ptr<Skydome> newSkydome = std::make_unique<Skydome>();

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//敵のテクスチャハンドル
	uint32_t skydomeTextureHandle_ = 0;

	//レールカメラ
	std::unique_ptr<RailCamera> newRailCamera = std::make_unique<RailCamera>();

	//プレイヤー
	//std::unique_ptr<Player> player_ = std::make_unique<Player>();

	// スマートポインタ (使いやすいのがユニークポインタ)
	std::unique_ptr<Enemy>enemy_ = std::make_unique<Enemy>();

	std::unique_ptr<PlayerBullet> playerBullet_ = std::make_unique<PlayerBullet>();

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_;

	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool enemyWaitFlag = false;
	int32_t enemyWaitTimer_ = 0;

	//プレイヤーモデル
	Model* playerModel_ = nullptr;

	//敵モデル
	Model* enemyModel_ = nullptr;

	Vector3 a = { 20,10,50 };
	Vector3 b = { 20,-10,50 };
	Vector3 c = { -20,-10,50 };
	Vector3 d = { -20,10,50 };

	bool normalFlag = false;
	bool cloudFlag = false;
	bool speedyFlag = false;
	bool splitFlag = false;

	bool normalDeathFlag = false;
	bool cloudDeathFlag = false;
	bool speedyDeathFlag = false;
	bool splitDeathFlag = false;

	int enemyDeathCount = 0;

	int enemySpawnTimer = 180;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
