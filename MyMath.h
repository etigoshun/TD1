#pragma once
#include "Vector3.h"
#include "WorldTransform.h"

//行列とベクトルの掛け算
Vector3 Transform(Vector3& velocity, WorldTransform& worldTransform);

//ベクトルの長さを求める
float Length(Vector3& vec);

//正規化する
Vector3 Normalize(Vector3& vec);

//座標間の距離を求める
double CalculateDistance(Vector3& vec1, Vector3& vec2);

//球同士の当たり判定
bool SphereHit(Vector3& vec1, Vector3& vec2, double radius1, double radius2);

//度数法からラジアンに変換
float RadianConversion(float angle);