#pragma once
#include<vector>
#include"MyLib/Vec3.h"
class Ui
{
public:
	Ui();
	~Ui();

	void FadeUpdate();
	void Update();
	void Draw(int fontHandle,float playerHP,int SearchRemainTime);
	int GetFadeCount() { return m_fadeCount; }
	
private:
	/// <summary>
   /// 0:左上 1:左下 2:右上 3:右下
   /// </summary>
	std::vector<Vec3> m_graphVelocity;
	/// <summary>
	/// 0:左上 1:左下 2:右上 3:右下 
	/// </summary>
	std::vector<Vec3> m_graphPos;

	int m_leftBottonHandle;
	int m_RightBottonHandle;

	int m_textureUIHandle;
	int m_fadeCount;
	float m_materialXAngle;
	bool m_isFadeIntroFlag;
	bool m_isFadeEnd;
	float m_angle;
};

