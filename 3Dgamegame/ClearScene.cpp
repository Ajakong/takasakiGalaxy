#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "SceneManager.h"
#include "ClearScene.h"
#include "TitleScene.h"
#include"WorldTimer.h"
#include"FontManager.h"

#include"Game.h"
#include"Pad.h"

namespace
{

}

ClearScene::ClearScene(SceneManager& mgr) :
	Scene(mgr),
	m_numFontHandle(FontManager::GetInstance().GetFontData("disital.TTF", "Pocket Calculator", 60, 7, DX_FONTTYPE_NORMAL)),
	m_fontHandle(FontManager::GetInstance().GetFontData("SF_font.ttf", "廻想体 ネクスト UP B", 60, 7, DX_FONTTYPE_NORMAL))
{
	m_frame = 60;
	m_updateFunc = &ClearScene::FadeInUpdate;
	m_drawFunc = &ClearScene::FadeDraw;


}

ClearScene::~ClearScene()
{

}

void ClearScene::Load()
{
}

void ClearScene::Update()
{
	(this->*m_updateFunc)();

	Pad::Update();
}

void ClearScene::Draw()
{
	(this->*m_drawFunc)();
}

void ClearScene::FadeInUpdate()
{
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &ClearScene::NormalUpdate;
		m_drawFunc = &ClearScene::NormalDraw;
	}
}

void ClearScene::NormalUpdate()
{
	m_btnFrame++;
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_updateFunc = &ClearScene::FadeOutUpdate;
		m_drawFunc = &ClearScene::FadeDraw;
	}
}

void ClearScene::FadeOutUpdate()
{
	m_frame++;
	if (60 <= m_frame)
	{
		StopSoundMem(SoundManager::GetInstance().GetSoundData("GamePlaying.mp3"));
		WorldTimer::Reset();
		m_manager.ResetScene(std::make_shared<TitleScene>(m_manager));
	}
}

void ClearScene::ChangeScene(std::shared_ptr<Scene> nextScene)
{
	WorldTimer::Reset();
}

void ClearScene::FadeDraw()
{
	DrawFormatStringToHandle(200, Game::kScreenHeight / 2, 0xffffff,m_numFontHandle,"%02d.%02d",WorldTimer::GetMinute(),WorldTimer::GetTimer());

	DrawFormatStringToHandle(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0x00ffff, m_fontHandle , "Clear");
	DrawFormatStringToHandle(Game::kScreenWidth / 2, Game::kScreenHeight / 200, 0x00ffff, m_fontHandle, "タイトルに戻る");
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 2000, 2000, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ClearScene::NormalDraw()
{
	DrawFormatStringToHandle(200, Game::kScreenHeight / 2, 0xffffff, m_numFontHandle, "%02d.%02d", WorldTimer::GetMinute(), WorldTimer::GetTimer());

	DrawFormatStringToHandle(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0x00ffff, m_fontHandle, "Clear");
	DrawFormatStringToHandle(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0x00ffff, m_fontHandle, "タイトルに戻る"); 
	auto& app = Application::GetInstance();
	auto size = app.GetWindowSize();
	int idx = m_btnFrame / 10 % 3;
	constexpr int kButtonSize = 16;
	constexpr float kBtnScale = 3.0f;
	
}
