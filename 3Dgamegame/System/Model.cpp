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
	_time += 0.5f;//0.5�Ȃ̂�30fps�f�[�^��60fps�Ŏg�p���邽��
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

///���f���̌��_���SY����]����
///@param yrot ��]�p(���W�A��)
void
Model::RotateY(float yrot) {
	_direction = yrot;
	DxLib::MV1SetRotationXYZ(_handle, VGet(0.f, _direction, 0.0f));
}

///����̍��W�Ɉړ�������
///@param pos ����ׂ����W
void
Model::SetPosition(const Vec3& pos) {
	_pos = pos;
	DxLib::MV1SetPosition(_handle, GetVECTORFromVector3(_pos));
}

///����̍��W����ړ�����
///@param vel ���̍��W����̃I�t�Z�b�g
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
	//pos��pushOutRect�̓����ɂ��邩�ǂ�������
	//if (pushOutRect.Left() <= _pos.x&&_pos.x <= pushOutRect.Right() &&
	//	pushOutRect.Top() <= _pos.z&&_pos.z <= pushOutRect.Bottom()) {//�����ɂ���
		//�O�ς�p���Č�_�����邩�ǂ������v�Z����c���̑O�Ɉړ��x�N�g���̌����Ō�_�ƌ�������\���̂���ӂ�I��
		//�E����������΍��̕ǁB������������Ύ�O�̕ǁB������������ΉE�̕ǁB��O����������Ή��̕ǂƔ��肵�܂�
	auto pos2 = Vec2(pos.x, pos.z);
	auto vec2 = Vec2(vel.x, vel.z);
	if (vel.z > 0.0f) {//������������
		//��O�̕ǂƌ�������
		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Top()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Top()) - pos2, vec2) <= 0) {
			//��O�ɖ߂�
			_pos.z = pushOutRect.Top() - 0.01f;;
		}
	}
	if (vel.x > 0.0f) {//�E����������
		//���̕ǂƌ�������
		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Bottom()) - pos2, vec2) * Cross(Vec2(pushOutRect.Left(), pushOutRect.Top()) - pos2, vec2) <= 0) {
			//���ɖ߂�
			_pos.x = pushOutRect.Left() - 0.01f;;
		}
	}

	if (vel.x < 0.0f) {//������������
		//�E�̕ǂƌ�������
		if (Cross(Vec2(pushOutRect.Right(), pushOutRect.Top()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Bottom()) - pos2, vec2) <= 0) {
			//�E�ɖ߂�
			_pos.x = pushOutRect.Right() + 0.01f;;
		}
	}
	if (vel.z < 0.0f) {//��O����������
		//���̕ǂƌ�������
		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Bottom()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Bottom()) - pos2, vec2) <= 0) {
			//���ɖ߂�
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
