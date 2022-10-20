#include "Skydome.h"

/// <summary>
/// 初期化
/// </summary>
void Skydome::Initialize(Model* model)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 150.0f,150.0f,150.0f };

	//行列の更新
	worldTransform_.matWorld_ = CreateScale(worldTransform_.scale_);

	worldTransform_.TransferMatrix();
}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update()
{}

/// <summary>
/// 描画
/// </summary>
void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
