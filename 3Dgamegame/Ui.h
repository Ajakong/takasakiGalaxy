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
   /// 0:���� 1:���� 2:�E�� 3:�E��
   /// </summary>
	std::vector<Vec3> m_graphVelocity;
	/// <summary>
	/// 0:���� 1:���� 2:�E�� 3:�E�� 
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

