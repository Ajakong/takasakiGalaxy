//#pragma once
//#include"../MyLib/Geometry.h"
//
//
//class Model
//{
//	bool useOriginalShader_ = false;
//	int vsH_;
//	int psH_;
//	int m_handle;//���f���̃n���h��
//	bool m_loadcompleted;//���f���̃��[�h���������Ă���
//	Vec3 m_pos;//���݂̍��W
//	Vec3 m_vel;//���݂̑��x�x�N�g��
//	float m_direction;//���݂̌���(X�����N�_�Ƃ���Y�����S��]�p�x)
//	int m_currentAttachedNo;//���݂̃��C���̃A�^�b�`�ԍ�
//	float m_totalTime;//���݃A�^�b�`���̃A�j���[�V�����̑�����
//	float m_time;//���݂̃A�j���[�V��������
//public:
//	Model(int h, int vsH = -1, int psH = -1);
//	~Model();
//
//	///���f���̃n���h����Ԃ�
//	///@return �n���h��
//	int GetHandle()const;
//
//	///���[�h�ς݂ł���
//	///@retval true ���[�h�ς�
//	///@retval false ���[�h��
//	bool IsLoadCompleted();
//
//	///���f���̃t���[����i�߂�
//	void Update();
//
//
//	///���f����`�悷��
//	void Draw();
//
//	///���f���̌��_���SY����]����
//	///@param yrot ��]�p(���W�A��)
//	void RotateY(float yrot);
//
//	///����̍��W�Ɉړ�������
//	///@param pos ����ׂ����W
//	void SetPosition(const Vec3& pos);
//	const Vec3& GetPosition()const;
//
//	///����̍��W����ړ�����
//	///@param vel ���̍��W����̃I�t�Z�b�g
//	void Move(const Vec3& vel);
//
//	///����̍��W����ړ�����(�����͈͕t��)
//	///@param vel ���̍��W����̃I�t�Z�b�g
//	void Move(const Vec3& vel, const Rect& range);
//
//
//	///����̍��W����ړ�����(�����͈͂���щ����o����`�t��)
//	///@param vel ���̍��W����̃I�t�Z�b�g
//	void Move(const Vec3& vel, const Rect& range, const Rect& pushOutRect);
//
//	///�A�j���[�V�������A�^�b�`����
//	///@param animno �A�j���[�V�����ԍ�
//	void AttachAnimaton(const int animno);
//};
//
