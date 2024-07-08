#pragma once
#include<iostream>

using namespace std;

class Camera;
class Physics;

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Init();
	void Update();
	void Draw();

	bool GetGameOver() { return m_isGameOverFlag; }
	bool GetClear() { return m_isClearFlag; }
private:

	std::shared_ptr<Camera> camera;
	std::shared_ptr<MyEngine::Physics> physics;
	std::shared_ptr<Player> player;

	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;

	

};

