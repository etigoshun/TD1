#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
#include "Input.h"

class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="rotation">初期角度</param>
	void Initialize(Vector3& position, Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド行列を取得
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//入力処理するため
	Input* input_ = nullptr;
};

