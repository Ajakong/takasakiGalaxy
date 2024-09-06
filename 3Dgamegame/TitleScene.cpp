#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include"GamePlayingScene.h"
#include"Game.h"
#include"Pad.h"
#include"Quaternion.h"



namespace
{
	constexpr int kFadeFrameMax = 60;
	constexpr int kStandByFrame = 120;

	constexpr int kLightningFrameMax = 200;

	const char* kTitleBGMPath = "Sound/Title.mp3";
	const char* kGameStartSEPath = "Sound/StartGame2.mp3";
	const char* kGameBGMPath = "Sound/GamePlaying.mp3";
	
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	m_angle(0.05f),
	m_titleHandle(LoadGraph("Image/galaxy_titleLogo_pro.png")),
	m_titleBGMHandle(SoundManager::GetInstance().GetSoundData(kTitleBGMPath)),
	m_gameStartSEHandle(SoundManager::GetInstance().GetSoundData(kGameStartSEPath))
{
	PlaySoundMem(m_titleBGMHandle,DX_PLAYTYPE_LOOP);
	Sphere sphere1;
	sphere1.pos = Vec3(0, 200, 100);
	sphere1.axis = Vec3(1, 0, 0);
	sphere1.color = 0x11ffff;
	m_spherePos.push_back(sphere1);
	Sphere sphere2;
	sphere2.pos = Vec3(100, 200, 0);
	sphere2.axis = Vec3(0, 0, 1);
	sphere2.color = 0x00ff00;
	m_spherePos.push_back(sphere2);
	Sphere sphere3;
	sphere3.pos = Vec3(100, -100, 100);
	sphere3.axis = Vec3(1, 0, 1);
	sphere3.color = 0xff0000;
	m_spherePos.push_back(sphere3);
	Sphere sphere4;
	sphere4.pos = Vec3(100, 100, -100);
	sphere4.axis = Vec3(-1, 0, 1);
	sphere4.color = 0x0000ff;
	m_spherePos.push_back(sphere4);
	Sphere sphere5;
	sphere5.pos = Vec3(100, 100, 100);
	sphere5.axis = Vec3(-1, 1, 0);
	sphere5.color = 0xffff00;
	m_spherePos.push_back(sphere5);
	Sphere sphere6;
	sphere6.pos = Vec3(-100, 100, 100);
	sphere6.axis = Vec3(0, 0, 1);
	sphere6.color = 0xff00ff;
	m_spherePos.push_back(sphere6);


	m_updateFunc = &TitleScene::NormalUpdate;
	m_drawFunc = &TitleScene::NormalDraw;

	SetCameraPositionAndTargetAndUpVec(Vec3(0, 0, -500).VGet(), Vec3(0, 0, 0).VGet(), GetCameraUpVector());
	//‰œs0.1`1000‚Ü‚Å‚ðƒJƒƒ‰‚Ì•`‰æ”ÍˆÍ‚Æ‚·‚é
	SetCameraNearFar(100, 6000);
	SetLightDirection(GetCameraFrontVector());
}

TitleScene::~TitleScene()
{
	StopSoundMem(m_titleBGMHandle);
	DeleteGraph(m_titleHandle);
}

void TitleScene::Load()
{
}

void TitleScene::Update()
{

	(this->*m_updateFunc)();

	Pad::Update();
}

void TitleScene::Draw()
{

	(this->*m_drawFunc)();
}

void TitleScene::FadeInUpdate()
{
	m_fps = GetFPS();

	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &TitleScene::NormalUpdate;
		m_drawFunc = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate()
{
	m_fps = GetFPS();

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		PlaySoundMem(m_gameStartSEHandle,DX_PLAYTYPE_BACK);
		m_updateFunc = &TitleScene::FadeOutUpdate;
		m_drawFunc = &TitleScene::FadeDraw;
	}
	

	m_btnFrame++;
}

void TitleScene::FadeOutUpdate()
{

	m_fps = GetFPS();
	m_frame++;
	if (m_frame >= 120) {
		m_isGamePlaying = true;
	}
	
}

void TitleScene::ChangeScene(std::shared_ptr<Scene> next)
{
	StopSoundMem(m_stageBgm);
	PlaySoundMem(SoundManager::GetInstance().GetSoundData(kGameBGMPath),DX_PLAYTYPE_LOOP);
	m_manager.ChangeScene(next);

}

void TitleScene::FadeDraw()
{
	for (auto& item : m_spherePos)
	{
		Vec3 zero = Vec3(0, 0, 0);
		Quaternion myQ;
		myQ.SetQuaternion(item.pos);
		myQ.SetMove(m_angle, item.axis);
		item.pos = myQ.Move(item.pos, zero);
		DrawSphere3D(item.pos.VGet(), 20, 60, item.color, item.color, true);
	}
	DrawFormatString(0, 0, 0xffffff, "TitleScene");
	DrawGraph(130, 0, m_titleHandle, true);
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / kFadeFrameMax));
	
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	if (m_isGamePlaying)
	{
		ChangeScene(std::make_shared<GamePlayingScene>(m_manager));
	}
}

void TitleScene::NormalDraw()
{
	
	for (auto& item : m_spherePos)
	{
		Vec3 zero = Vec3(0, 0, 0);
		Quaternion myQ;
		myQ.SetQuaternion(item.pos);
		myQ.SetMove(m_angle, item.axis);
		item.pos = myQ.Move(item.pos, zero);
		DrawSphere3D(item.pos.VGet(), 20, 60, item.color, item.color, true);
	}

	DrawFormatString(0, 0, 0xffffff, "TitleScene");

	DrawFormatString(730, 650, 0xffffff, "Push A to Start");
	DrawGraph(130, 0, m_titleHandle, true);
}
