#pragma once
#include"../MyLib/Vec3.h"


class Model
{
	bool useOriginalShader_ = false;
	int vsH_;
	int psH_;
	int _handle;//���f���̃n���h��
	bool _loadcompleted;//���f���̃��[�h���������Ă���
	Vec3 _pos;//���݂̍��W
	Vec3 _vel;//���݂̑��x�x�N�g��
	float _direction;//���݂̌���(X�����N�_�Ƃ���Y�����S��]�p�x)
	int _currentAttachedNo;//���݂̃��C���̃A�^�b�`�ԍ�
	float _totalTime;//���݃A�^�b�`���̃A�j���[�V�����̑�����
	float _time;//���݂̃A�j���[�V��������
public:
	Model(int h, int vsH = -1, int psH = -1);
	~Model();

	///���f���̃n���h����Ԃ�
	///@return �n���h��
	int GetHandle()const;

	///���[�h�ς݂ł���
	///@retval true ���[�h�ς�
	///@retval false ���[�h��
	bool IsLoadCompleted();

	///���f���̃t���[����i�߂�
	void Update();


	///���f����`�悷��
	void Draw();

	///���f���̌��_���SY����]����
	///@param yrot ��]�p(���W�A��)
	void RotateY(float yrot);

	///����̍��W�Ɉړ�������
	///@param pos ����ׂ����W
	void SetPosition(const Vec3& pos);
	const Vec3& GetPosition()const;

	///����̍��W����ړ�����
	///@param vel ���̍��W����̃I�t�Z�b�g
	void Move(const Vec3& vel);

	///����̍��W����ړ�����(�����͈͕t��)
	///@param vel ���̍��W����̃I�t�Z�b�g
	void Move(const Vec3& vel, const RectF& range);


	///����̍��W����ړ�����(�����͈͂���щ����o����`�t��)
	///@param vel ���̍��W����̃I�t�Z�b�g
	void Move(const Vec3& vel, const RectF& range, const RectF& pushOutRect);

	///�A�j���[�V�������A�^�b�`����
	///@param animno �A�j���[�V�����ԍ�
	void AttachAnimaton(const int animno);
};

