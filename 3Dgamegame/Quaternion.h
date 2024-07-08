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
    Quaternion(float inW, float inX, float inY, float inZ) { w = inW; x = inX, y = inY, z = inZ; }//区別はつける

    Quaternion operator *(Quaternion qua)
    {
        Quaternion a;
        a.w = w * qua.w - x * qua.x - y * qua.y - z * qua.z;//実部
        a.x = w * qua.x + x * qua.w + y * qua.z - z * qua.y;//虚部x
        a.y = w * qua.y + y * qua.w + z * qua.x - x * qua.z;//虚部y
        a.z = w * qua.z + z * qua.w + x * qua.y - y * qua.x;//虚部z
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
        //3次元座標をクオータニオンに変換
        qPos.w = 1.0f;
        qPos.x = vec.x;
        qPos.y = vec.y;
        qPos.z = vec.z;

        //回転クォータニオンのインバースの作成
        //逆クォータニオンを出すのは大変なので、
        //3次元だと同じ値になる共役クオータニオンで作成(虚部だけマイナス反転)
        qInv = Quaternion(w, -x, -y, -z);

        //回転後のクオータニオンの作成
        qPos = qRot * qPos * qInv; //[回転クオータニオン] * [現在のクオータニオン] * [回転クオータニオンのインバース]

        //３次元座標に戻す
        vPos.x = qPos.x;
        vPos.y = qPos.y;
        vPos.z = qPos.z;

        return vPos;
    }
};
