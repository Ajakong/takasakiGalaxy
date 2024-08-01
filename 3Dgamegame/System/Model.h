#pragma once
#include"../Geometry.h"


class Model
{
	bool useOriginalShader_ = false;
	int vsH_;
	int psH_;
	int _handle;//モデルのハンドル
	bool _loadcompleted;//モデルのロードが完了している
	Position3f _pos;//現在の座標
	Vector3f _vel;//現在の速度ベクトル
	float _direction;//現在の向き(X軸を起点としたY軸中心回転角度)
	int _currentAttachedNo;//現在のメインのアタッチ番号
	float _totalTime;//現在アタッチ中のアニメーションの総時間
	float _time;//現在のアニメーション時間
public:
	Model(int h, int vsH = -1, int psH = -1);
	~Model();

	///モデルのハンドルを返す
	///@return ハンドル
	int GetHandle()const;

	///ロード済みである
	///@retval true ロード済み
	///@retval false ロード中
	bool IsLoadCompleted();

	///モデルのフレームを進める
	void Update();


	///モデルを描画する
	void Draw();

	///モデルの原点中心Y軸回転する
	///@param yrot 回転角(ラジアン)
	void RotateY(float yrot);

	///特定の座標に移動させる
	///@param pos いるべき座標
	void SetPosition(const Position3f& pos);
	const Position3f& GetPosition()const;

	///特定の座標から移動する
	///@param vel 今の座標からのオフセット
	void Move(const Vector3f& vel);

	///特定の座標から移動する(制限範囲付き)
	///@param vel 今の座標からのオフセット
	void Move(const Vector3f& vel, const RectF& range);


	///特定の座標から移動する(制限範囲および押し出し矩形付き)
	///@param vel 今の座標からのオフセット
	void Move(const Vector3f& vel, const RectF& range, const RectF& pushOutRect);

	///アニメーションをアタッチする
	///@param animno アニメーション番号
	void AttachAnimaton(const int animno);
};

