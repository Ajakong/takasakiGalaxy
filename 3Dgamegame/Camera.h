#pragma once
#include "DxLib.h"
#include<iostream>

using namespace std;

class Player;
class Vec3;

class Camera
{
public:
	Camera();							// コンストラクタ.
	~Camera();							// デストラクタ.

	void Update();	// 更新.
	Vec3 cameraToPlayer(const Vec3& targetPos);
	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }

	float GetCameraAngle() const { return cameraAngle; }

private:
	VECTOR	pos;			// ポジション.

	float cameraAngle = -DX_PI_F / 2;
	VECTOR velocity = VGet(0, 0, 0);


};