#pragma once
#include "MyLib/Vec3.h"
#include <cmath>

class Quaternion
{
public:
	Quaternion()
	{
		Qu.w = 1;
		Qu.x = 0;
		Qu.y = 0;
		Qu.z = 0;
	};
	Quaternion(float _w, float _x, float _y, float _z)
	{
		Qu.w = _w;
		Qu.x = _x;
		Qu.y = _y;
		Qu.z = _z;
	};
	virtual ~Quaternion() {}

private:
	struct Q
	{
		float w;
		float x;
		float y;
		float z;

		Q operator * (const Q& _q) const
		{
			Q tempQ;

			/*�N�I�[�^�j�I���̊|���Z*/
			tempQ.w = w * _q.w - x * _q.x - y * _q.y - z * _q.z;//����
			tempQ.x = w * _q.x + x * _q.w + y * _q.z - z * _q.y;//����x
			tempQ.y = w * _q.y + y * _q.w + z * _q.x - x * _q.z;//����y
			tempQ.z = w * _q.z + z * _q.w + x * _q.y - y * _q.x;//����z

			return tempQ;
		}
	};

	Q Qu;

public:
	//��]�N�H�[�^�j�I��
	Quaternion CreateRotationQuaternion(double radian, Vec3 Axis)
	{
		Quaternion ans;
		double norm;
		double ccc, sss;

		ans.Qu.w = ans.Qu.x = ans.Qu.y = ans.Qu.z = 0.0;

		norm = Axis.x * Axis.x + Axis.y * Axis.y + Axis.z * Axis.z;
		if (norm <= 0.0) return ans;

		norm = 1.0 / sqrt(norm);
		Axis.x *= norm;
		Axis.y *= norm;
		Axis.z *= norm;

		ccc = cos(0.5 * radian);
		sss = sin(0.5 * radian);

		ans.Qu.w = ccc;
		ans.Qu.x = sss * Axis.x;
		ans.Qu.y = sss * Axis.y;
		ans.Qu.z = sss * Axis.z;

		return ans;
	}
	void SetQuaternion(Vec3 pos) { Qu.w = 1.0; Qu.x = pos.x; Qu.y = pos.y; Qu.z = pos.z; }

	void SetMove(float _angle, Vec3 _axis)//��]���Ɗp���x�̐ݒ�
	{
		_axis.Normalize();
		Qu.w = cos(_angle / 2.0f);//����
		Qu.x = _axis.x * sin(_angle / 2.0f);
		Qu.y = _axis.y * sin(_angle / 2.0f);
		Qu.z = _axis.z * sin(_angle / 2.0f);
	}
	/// <summary>
	/// ���s�ړ�
	/// </summary>
	/// <param name="m_pos"></param>
	/// <param name="_vec"></param>
	/// <returns></returns>
	Vec3 Move(Vec3& m_pos, Vec3& _vec)
	{
		Q qPos, qInv;
		Vec3 vPos;

		//3�������W���N�I�[�^�j�I���ɕϊ�
		qPos.w = 1.0f;
		qPos.x = m_pos.x;
		qPos.y = m_pos.y;
		qPos.z = m_pos.z;

		//��]�N�H�[�^�j�I���̃C���o�[�X�̍쐬
		//�t�N�H�[�^�j�I�����o���̂͑�ςȂ̂ŁA
		//3�������Ɠ����l�ɂȂ鋤���N�I�[�^�j�I���ō쐬(���������}�C�i�X���])
		qInv.w = Qu.w;
		qInv.x = -Qu.x;
		qInv.y = -Qu.y;
		qInv.z = -Qu.z;

		//��]��̃N�I�[�^�j�I���̍쐬
		qPos = Qu * qPos * qInv;

		//�R�������W�ɖ߂�
		vPos.x = qPos.x;
		vPos.y = qPos.y;
		vPos.z = qPos.z;

		// ��]��Ɉړ�
		vPos.x += _vec.x;
		vPos.y += _vec.y;
		vPos.z += _vec.z;

		return vPos;
	}

	Vec3 ToVec3()
	{
		return Vec3(Qu.x, Qu.y, Qu.z);
	}

	MATRIX ToMat()
	{
		MATRIX matQ;

		float x2 = Qu.x * Qu.x;
		float y2 = Qu.y * Qu.y;
		float z2 = Qu.z * Qu.z;
		float w2 = Qu.w * Qu.w;

		float r00 = x2 - y2 - z2 + w2;
		float r01 = 2.0f * (Qu.x * Qu.y + Qu.z * Qu.w);
		float r02 = 2.0f * (Qu.x * Qu.z - Qu.y * Qu.w);

		float r10 = 2.0f * (Qu.x * Qu.y - Qu.z * Qu.w);
		float r11 = -x2 + y2 - z2 + w2;
		float r12 = 2.0f * (Qu.y * Qu.z + Qu.x * Qu.w);

		float r20 = 2.0f * (Qu.x * Qu.z + Qu.y * Qu.w);
		float r21 = 2.0f * (Qu.y * Qu.z - Qu.x * Qu.w);
		float r22 = -x2 - y2 + z2 + w2;

		matQ.m[0][0] = r00;
		matQ.m[0][1] = r01;
		matQ.m[0][2] = r02;
		matQ.m[1][0] = r10;
		matQ.m[1][1] = r11;
		matQ.m[1][2] = r12;
		matQ.m[2][0] = r20;
		matQ.m[2][1] = r21;
		matQ.m[2][2] = r22;

		matQ.m[3][3] = 1.0f;

		return matQ;
	}
	
};

