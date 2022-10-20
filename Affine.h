#pragma once
#include "Sprite.h"
#include "MathUtility.h"

//単位行列生成関数
Matrix4 CreateIdentity();

//スケーリング行列関数
Matrix4 CreateScale(Vector3 scale);

//Z軸回転行列関数
Matrix4 CreateRotZ(Vector3 rotation);

//X軸回転行列関数
Matrix4 CreateRotX(Vector3 rotation);

//Y軸回転行列関数
Matrix4 CreateRotY(Vector3 rotation);

//平行移動行列関数
Matrix4 CreateTrans(Vector3 translation);
