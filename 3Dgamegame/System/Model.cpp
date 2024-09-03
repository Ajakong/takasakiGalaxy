//#include "Model.h"
//#include<DxLib.h>
//#include<algorithm>
//
//using namespace std;
//
//
//float Cross(const Vec2& lv, const Vec2& rv) {
//	return lv.x * rv.y - lv.y * rv.x;
//}
//
//
//namespace {
//	VECTOR GetVECTORFromVector3(const Vec3& v) {
//		return VGet(v.x, v.y, v.z);
//	}
//}
//Model::Model(int h, int vsH, int psH) :
//	m_handle(h),
//	vsH_(vsH),
//	psH_(psH),
//	m_loadcompleted(false),
//	m_pos(),
//	m_vel(),
//	m_time(0),
//	m_currentAttachedNo(0),
//	m_direction(0.f),
//	m_totalTime(0.f)
//{
//}
//
//
//Model::~Model()
//{
//}
//
//
//int
//Model::GetHandle()const {
//	return m_handle;
//}
//bool
//Model::IsLoadCompleted() {
//	if (m_loadcompleted)return true;
//	if (DxLib::CheckHandleASyncLoad(m_handle)) {
//		m_loadcompleted = false;
//	}
//	else {
//		m_loadcompleted = true;
//		int num = MV1GetMeshNum(m_handle);
//		for (int i = 0; i < num; ++i)
//		{
//			auto type = MV1GetTriangleListVertexType(m_handle, i);
//			if (type == DX_MV1_VERTEX_TYPE_4FRAME)
//			{
//				useOriginalShader_ = true;
//				break;
//			}
//		}
//	}
//	return m_loadcompleted;
//}
//
//void
//Model::Update() {
//	DxLib::MV1SetAttachAnimTime(m_handle, m_currentAttachedNo, m_time);
//	m_time += 0.5f;//0.5�Ȃ̂�30fps�f�[�^��60fps�Ŏg�p���邽��
//	if (m_time > m_totalTime) {
//		m_time = 0.0f;
//	}
//}
//
//void
//Model::Draw() {
//	if (useOriginalShader_)
//	{
//		MV1SetUseOrigShader(true);
//		SetUseVertexShader(vsH_);
//		SetUsePixelShader(psH_);
//	}
//	DxLib::MV1DrawModel(m_handle);
//	MV1SetUseOrigShader(false);
//}
//
/////���f���̌��_���SY����]����
/////@param yrot ��]�p(���W�A��)
//void
//Model::RotateY(float yrot) {
//	m_direction = yrot;
//	DxLib::MV1SetRotationXYZ(m_handle, VGet(0.f, m_direction, 0.0f));
//}
//
/////����̍��W�Ɉړ�������
/////@param pos ����ׂ����W
//void
//Model::SetPosition(const Vec3& pos) {
//	m_pos = pos;
//	DxLib::MV1SetPosition(m_handle, GetVECTORFromVector3(m_pos));
//}
//
/////����̍��W����ړ�����
/////@param vel ���̍��W����̃I�t�Z�b�g
//void
//Model::Move(const Vec3& vel) {
//	m_pos += vel;
//	DxLib::MV1SetPosition(m_handle, GetVECTORFromVector3(m_pos));
//}
//
//void
//Model::Move(const Vec3& vel, const Rect& range) {
//	m_pos += vel;
//	m_pos.x = min(max(m_pos.x, range.Left() + 0.01f), range.Right() - 0.01f);
//	m_pos.z = min(max(m_pos.z, range.Top()), range.Bottom());
//	DxLib::MV1SetPosition(m_handle, GetVECTORFromVector3(m_pos));
//}
//
//
//void
//Model::Move(const Vec3& vel, const Rect& range, const Rect& pushOutRect) {
//	auto pos = m_pos;
//	m_pos += vel;
//	m_pos.x = min(max(m_pos.x, range.Left() + 0.01f), range.Right() - 0.01f);
//	m_pos.z = min(max(m_pos.z, range.Top()), range.Bottom());
//	//pos��pushOutRect�̓����ɂ��邩�ǂ�������
//	//if (pushOutRect.Left() <= m_pos.x&&m_pos.x <= pushOutRect.Right() &&
//	//	pushOutRect.Top() <= m_pos.z&&m_pos.z <= pushOutRect.Bottom()) {//�����ɂ���
//		//�O�ς�p���Č�_�����邩�ǂ������v�Z����c���̑O�Ɉړ��x�N�g���̌����Ō�_�ƌ�������\���̂���ӂ�I��
//		//�E����������΍��̕ǁB������������Ύ�O�̕ǁB������������ΉE�̕ǁB��O����������Ή��̕ǂƔ��肵�܂�
//	auto pos2 = Vec2(pos.x, pos.z);
//	auto vec2 = Vec2(vel.x, vel.z);
//	if (vel.z > 0.0f) {//������������
//		//��O�̕ǂƌ�������
//		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Top()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Top()) - pos2, vec2) <= 0) {
//			//��O�ɖ߂�
//			m_pos.z = pushOutRect.Top() - 0.01f;;
//		}
//	}
//	if (vel.x > 0.0f) {//�E����������
//		//���̕ǂƌ�������
//		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Bottom()) - pos2, vec2) * Cross(Vec2(pushOutRect.Left(), pushOutRect.Top()) - pos2, vec2) <= 0) {
//			//���ɖ߂�
//			m_pos.x = pushOutRect.Left() - 0.01f;;
//		}
//	}
//
//	if (vel.x < 0.0f) {//������������
//		//�E�̕ǂƌ�������
//		if (Cross(Vec2(pushOutRect.Right(), pushOutRect.Top()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Bottom()) - pos2, vec2) <= 0) {
//			//�E�ɖ߂�
//			m_pos.x = pushOutRect.Right() + 0.01f;;
//		}
//	}
//	if (vel.z < 0.0f) {//��O����������
//		//���̕ǂƌ�������
//		if (Cross(Vec2(pushOutRect.Left(), pushOutRect.Bottom()) - pos2, vec2) * Cross(Vec2(pushOutRect.Right(), pushOutRect.Bottom()) - pos2, vec2) <= 0) {
//			//���ɖ߂�
//			m_pos.z = pushOutRect.Bottom() + 0.01f;;
//		}
//	}
//
//	//}
//	DxLib::MV1SetPosition(m_handle, GetVECTORFromVector3(m_pos));
//}
//
//void
//Model::AttachAnimaton(const int animno) {
//	m_currentAttachedNo = DxLib::MV1AttachAnim(m_handle, animno);
//	m_totalTime = DxLib::MV1GetAttachAnimTotalTime(m_handle, m_currentAttachedNo);
//}
//
//
//const Vec3&
//Model::GetPosition()const {
//	return m_pos;
//}
