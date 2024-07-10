#pragma once
#include "DxLib.h"
#include<iostream>

using namespace std;

class Player;
class Vec3;

class Camera
{
public:
	Camera();							// �R���X�g���N�^.
	~Camera();							// �f�X�g���N�^.

	void Update();	// �X�V.
	Vec3 cameraToPlayer(const Vec3& targetPos);
	// �|�W�V������getter/setter.
	const VECTOR& GetPos() const { return pos; }

	float GetCameraAngle() const { return cameraAngle; }

private:
	VECTOR	pos;			// �|�W�V����.

	float cameraAngle = -DX_PI_F / 2;
	VECTOR velocity = VGet(0, 0, 0);


};