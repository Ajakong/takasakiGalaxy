#pragma once
#include"MyLib/Vec3.h"

class Quaternion
{
public:
    float w;
    float x;
    float y;
    float z;

public:
    Quaternion() { w = 0; x = 0; y = 0; z = 0; }
    Quaternion(float inW, float inX, float inY, float inZ) { w = inW; x = inX, y = inY, z = inZ; }//��ʂ͂���

    Quaternion operator *(Quaternion qua)
    {
        Quaternion a;
        a.w = w * qua.w - x * qua.x - y * qua.y - z * qua.z;//����
        a.x = w * qua.x + x * qua.w + y * qua.z - z * qua.y;//����x
        a.y = w * qua.y + y * qua.w + z * qua.x - x * qua.z;//����y
        a.z = w * qua.z + z * qua.w + x * qua.y - y * qua.x;//����z
        return a;
    }

    Vec3 operator *(Vec3 vec)
    {
        Quaternion qPos, qInv, qRot;
        Vec3 vPos;

        qRot.w = w;
        qRot.x = x;
        qRot.z = z;
        qRot.y = y;
        //3�������W���N�I�[�^�j�I���ɕϊ�
        qPos.w = 1.0f;
        qPos.x = vec.x;
        qPos.y = vec.y;
        qPos.z = vec.z;

        //��]�N�H�[�^�j�I���̃C���o�[�X�̍쐬
        //�t�N�H�[�^�j�I�����o���̂͑�ςȂ̂ŁA
        //3�������Ɠ����l�ɂȂ鋤���N�I�[�^�j�I���ō쐬(���������}�C�i�X���])
        qInv = Quaternion(w, -x, -y, -z);

        //��]��̃N�I�[�^�j�I���̍쐬
        qPos = qRot * qPos * qInv; //[��]�N�I�[�^�j�I��] * [���݂̃N�I�[�^�j�I��] * [��]�N�I�[�^�j�I���̃C���o�[�X]

        //�R�������W�ɖ߂�
        vPos.x = qPos.x;
        vPos.y = qPos.y;
        vPos.z = qPos.z;

        return vPos;
    }
};
