#include "MyMath.h"
#include <cmath>

Vector3 Transform(Vector3& velocity, WorldTransform& worldTransform)
{
	Vector3 vec{ 0,0,0 };

	vec.x = velocity.x * worldTransform.matWorld_.m[0][0]
		+ velocity.y * worldTransform.matWorld_.m[1][0]
		+ velocity.z * worldTransform.matWorld_.m[2][0];

	vec.y = velocity.x * worldTransform.matWorld_.m[0][1]
		+ velocity.y * worldTransform.matWorld_.m[1][1]
		+ velocity.z * worldTransform.matWorld_.m[2][1];

	vec.z = velocity.x * worldTransform.matWorld_.m[0][2]
		+ velocity.y * worldTransform.matWorld_.m[1][2]
		+ velocity.z * worldTransform.matWorld_.m[2][2];

	return vec;
}

//ベクトルの長さを求める
float Length(Vector3& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

//正規化する
Vector3 Normalize(Vector3& vec)
{
	float len = Length(vec);
	if (len != 0)
	{
		vec /= len;
	}
	return vec;
}

//座標間の距離を求める
double CalculateDistance(Vector3& vec1, Vector3& vec2)
{
	double distance = pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec1.y, 2) + pow(vec2.z - vec1.z, 2);
	return distance;
}

//球同士の当たり判定
bool SphereHit(Vector3& vec1, Vector3& vec2, double radius1, double radius2)
{
	if (CalculateDistance(vec1, vec2) <= radius1 + radius2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//度数法からラジアンに変換
float RadianConversion(float angle)
{
	angle = angle * 3.14 / 180;
	return angle;
}