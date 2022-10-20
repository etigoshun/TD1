#include "Affine.h"

//単位行列生成関数
Matrix4 CreateIdentity()
{
	//単位行列
	Matrix4 matIdentity;

	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}

//スケーリング行列関数
Matrix4 CreateScale(Vector3 scale)
{
	//スケーリング行列を宣言
	Matrix4 matScale;

	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;

	return matScale;
}

//Z軸回転行列関数
Matrix4 CreateRotZ(Vector3 rotation)
{
	//各軸用回転行列を宣言
	Matrix4 matRotZ;

	//Z軸回転行列
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;


	return matRotZ;
}

//X軸回転行列関数
Matrix4 CreateRotX(Vector3 rotation)
{
	//各軸用回転行列を宣言
	Matrix4 matRotX;
	//X軸回転行列
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	matRotX.m[3][3] = 1;

	return matRotX;
}

//Y軸回転行列関数
Matrix4 CreateRotY(Vector3 rotation)
{
	//各軸用回転行列を宣言
	Matrix4 matRotY;

	//Y軸回転行列
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[1][1] = 1;
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	matRotY.m[3][3] = 1;

	return matRotY;
}

//平行移動行列関数
Matrix4 CreateTrans(Vector3 translation)
{
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//移動量を行列に設定
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;

	return matTrans;
}