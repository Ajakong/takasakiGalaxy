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

			/*クオータニオンの掛け算*/
			//公式通りです。
			tempQ.w = w * _q.w - x * _q.x - y * _q.y - z * _q.z;//実部
			tempQ.x = w * _q.x + x * _q.w + y * _q.z - z * _q.y;//虚部x
			tempQ.y = w * _q.y + y * _q.w + z * _q.x - x * _q.z;//虚部y
			tempQ.z = w * _q.z + z * _q.w + x * _q.y - y * _q.x;//虚部z

			return tempQ;
		}
	};

	Q Qu;

public:



	void SetMove(float _angle, Vec3& _axis)
	{
		Qu.w = cos(_angle / 2.0f);//実部
		Qu.x = _axis.x * sin(_angle / 2.0f);
		Qu.y = _axis.y * sin(_angle / 2.0f);
		Qu.z = _axis.z * sin(_angle / 2.0f);
	}

	Vec3 Move(Vec3& _pos, Vec3& _vec)
	{
		Q qPos, qInv;
		Vec3 vPos;

		//3次元座標をクオータニオンに変換
		qPos.w = 1.0f;
		qPos.x = _pos.x;
		qPos.y = _pos.y;
		qPos.z = _pos.z;

		//回転クォータニオンのインバースの作成
		//逆クォータニオンを出すのは大変なので、
		//3次元だと同じ値になる共役クオータニオンで作成(虚部だけマイナス反転)
		qInv.w = Qu.w;
		qInv.x = -Qu.x;
		qInv.y = -Qu.y;
		qInv.z = -Qu.z;

		//回転後のクオータニオンの作成
		qPos = Qu * qPos * qInv;

		//３次元座標に戻す
		vPos.x = qPos.x;
		vPos.y = qPos.y;
		vPos.z = qPos.z;

		// 回転後に移動
		vPos.x += _vec.x;
		vPos.y += _vec.y;
		vPos.z += _vec.z;

		return vPos;
	}
};

