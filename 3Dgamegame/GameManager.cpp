#include "GameManager.h"
#include"Camera.h"
#include"MyLib/Physics/Physics.h"
#include"Player.h"

GameManager::GameManager()
{
	camera = std::make_shared<Camera>();
	
	player = std::make_shared<Player>();

	physics = std::make_shared<MyEngine::Physics>();
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	
}

void GameManager::Update()
{
	camera->Update();
	

	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}

	physics->Update();


}

void GameManager::Draw()
{

	
}
