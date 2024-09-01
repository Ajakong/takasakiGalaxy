#include "Model.h"
#include<DxLib.h>
#include<algorithm>

using namespace std;


float Cross(const Vec2& lv, const Vec2& rv) {
	return lv.x * rv.y - lv.y * rv.x;
}


namespace {
	VECTOR GetVECTORFromVector3(const Vec3& v) {
		return VGet(v.x, v.y, v.z);
	}
}
Model::Model(int h, int vsH, int psH) :
	_handle(h),
	vsH_(vsH),
	psH_(psH),
	_loadcompleted(false),
	_pos(),
	_vel(),
	_time(0)
{
}


Model::~Model()
{
}


int
Model::GetHandle()const {
	return _handle;
}
bool
Model::IsLoadCompleted() {
	if (_loadcompleted)return true;
	if (DxLib::CheckHandleASyncLoad(_handle)) {
		_loadcompleted = false;
	}
	else {
		_loadcompleted = true;
		int num = MV1GetMeshNum(_handle);
		for (int i = 0; i < num; ++i)
		{
			auto type = MV1GetTriangleListVertexType(_handle, i);
			if (type == DX_MV1_VERTEX_TYPE_4FRAME)
			{
				useOriginalShader_ = true;
				break;
			}
		}
	}
	return _loadcompleted;
}

void
Model::Update() {
	DxLib::MV1SetAttachAnimTime(_handle, _currentAttachedNo, _time);
	_time += 0.5f;//0.5なのは30fpsデータを60fpsで使用するため
	if (_time > _totalTime) {
		_time = 0.0f;
	}
}

void
Model::Draw() {
	if (useOriginalShader_)
	{
		MV1SetUseOrigShader(true);
		SetUseVertexShader(vsH_);
		SetUsePixelShader(psH_);
	}
	DxLib::MV1DrawModel(_handle);
	MV1SetUseOrigShader(false);
}

///モデルの原点中心Y軸回転する
///@param yrot 回転角(ラジアン)
void
Model::RotateY(float yrot) {
	_direction = yrot;
	DxLib::MV1SetRotationXYZ(_handle, VGet(0.f, _direction, 0.0f));
}

///特定の座標に移動させる
///@param pos いるべき座標
void
Model::SetPosition(const Vec3& pos) {
	_pos = pos;
	DxLib::MV1SetPosition(_handle, GetVECTORFromVector3(_pos));
}

///特定の座標から移動する
///@param vel 今の座標からのオフセット
void
Model::Move(const Vec3& vel) {
	_pos += vel;
	DxLib::MV1SetPosition(_handle, GetVECTORFromVector3(_pos));
}

void
Model::Move(const Vec3& vel, const Rect& range) {
	_pos += vel;
	_pos.x = min(max(_pos.x, range.Left() + 0.01f), range.Right() - 0.01f);
	_pos.z = min(max(_pos.z, range.Top()), range.Bottom());
	DxLib::MV1SetPosition(_handle, GetVECTORFromVector3(_pos));
}


void
Model::Move(const Vec3& vel, const Rect& range, const Rect& pushOutRect) {
	auto pos = _pos;
	_pos += vel;
	_pos.x = min(max(_pos.x, range.Left() + 0.01f), range.Right() - 0.01f);
	_pos.z = min(max(_pos.z, range.Top()), range.Bottom());
	//posがpushOutRectの内部にあるかどうか判定
	//if (pushOutRect.Left() <= _pos.x&&_pos.x <= pushOutRect.Right() &&
	//	pushOutRect.Top() <= _pos.z&&_pos.z <= pushOutRect.Bottom()) {//内部にある
		//外積を用いて交点があるかどうかを計算する…その前に移動ベクトルの向きで交点と交差する可能性のある辺を選別
		//右成分があれば左の壁。奥成分があれば手前の壁。左成分があれば右の壁。手前成分があれば奥の壁と判定します
	auto pos2 = Vec2(pos.x, pos.z);
	auto vec2 = Vec2(vel.x, vel.z);
	if (vel.z > 0.0f) {//奥成分がある
		//手前の壁と交差判定
		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Top()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Top()) - pos2, vec2) <= 0) {
			//手前に戻す
			_pos.z = pushOutRect.Top() - 0.01f;;
		}
	}
	if (vel.x > 0.0f) {//右成分がある
		//左の壁と交差判定
		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Bottom()) - pos2, vec2) * Cross(Vec2(pushOutRect.Left(), pushOutRect.Top()) - pos2, vec2) <= 0) {
			//左に戻す
			_pos.x = pushOutRect.Left() - 0.01f;;
		}
	}

	if (vel.x < 0.0f) {//左成分がある
		//右の壁と交差判定
		if (Cross(Vec2(pushOutRect.Right(), pushOutRect.Top()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Bottom()) - pos2, vec2) <= 0) {
			//右に戻す
			_pos.x = pushOutRect.Right() + 0.01f;;
		}
	}
	if (vel.z < 0.0f) {//手前成分がある
		//奥の壁と交差判定
		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Bottom()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Bottom()) - pos2, vec2) <= 0) {
			//奥に戻す
			_pos.z = pushOutRect.Bottom() + 0.01f;;
		}
	}

	//}
	DxLib::MV1SetPosition(_handle, GetVECTORFromVector3(_pos));
}

void
Model::AttachAnimaton(const int animno) {
	_currentAttachedNo = DxLib::MV1AttachAnim(_handle, animno);
	_totalTime = DxLib::MV1GetAttachAnimTotalTime(_handle, _currentAttachedNo);
}


const Vec3&
Model::GetPosition()const {
	return _pos;
}
