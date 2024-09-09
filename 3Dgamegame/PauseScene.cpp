#include "PauseScene.h"
#include <DxLib.h>
#include"Pad.h"
#include "Application.h"
#include "SceneManager.h"
#include "PauseScene.h"
#include"TitleScene.h"
#include"Game.h"
#include"SoundManager.h"
#include"MyLib/Physics/Physics.h"

namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 50;

	/// <summary>
	/// 一文字ずつのOffsetX(文字列を中心座標を指定してDrawしたいときに使う。中心座標-kCharPosOffSetX*文字数と使う)
	/// </summary>
	constexpr int kCharPosOffSetX = 4;
	/// <summary>
	/// 一文字ずつのOffsetX
	/// </summary>
	constexpr int kCharPosOffSetY = 6;

}

PauseScene::PauseScene(SceneManager& mgr) : Scene(mgr),
m_soundVol(SoundManager::GetInstance().GetSoundVol())
{
	m_updateFunc = &PauseScene::AppearUpdate;
	m_drawFunc = &PauseScene::ExpandDraw;
	m_tutoHandle = LoadGraph("data/image/tuto/controller_format.png");
}

void PauseScene::Load()
{
}

void PauseScene::Update()
{
	(this->*m_updateFunc)();
	if (Pad::IsPress(PAD_INPUT_Z))
	{
		m_soundVol++;
	}
	if (Pad::IsPress(PAD_INPUT_Y))//XBoxコントローラーのL
	{
		m_soundVol--;
	}

	SoundManager::GetInstance().ChangeSoundVolume(m_soundVol);


	Pad::Update();
}

void PauseScene::Draw()
{
	(this->*m_drawFunc)();
}

void PauseScene::AppearUpdate()
{
	m_frame += 2;
	if (kAppeaInterval <= m_frame)
	{
		m_updateFunc = &PauseScene::NormalUpdate;
		m_drawFunc = &PauseScene::NormalDraw;
	}
}

void PauseScene::NormalUpdate()
{
	if (!m_tutoFlag)
	{
		if (Pad::IsTrigger(PAD_INPUT_R))
		{
			m_updateFunc = &PauseScene::DisappearUpdate;
			m_drawFunc = &PauseScene::ExpandDraw;

		}
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			m_select++;
		}
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			m_select--;
		}
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		if (m_select % 3 == 0)
		{
			m_updateFunc = &PauseScene::DisappearUpdate;
			m_drawFunc = &PauseScene::ExpandDraw;
		}
		else
		{
			if (m_tutoFlag)
			{
				m_tutoFlag = false;
			}
			else
			{
				m_tutoFlag = true;

			}
		}
		if(m_select % 3 == 1|| m_select % 3 == -2)
		{
			if (m_tutoFlag)
			{
				m_tutoFlag = false;
			}
			else
			{
				m_tutoFlag = true;
			}

		}
		if (m_select % 3 == 2 || m_select % 3 == -1)
		{
			m_manager.PopScene();

			StopSoundMem(SoundManager::GetInstance().GetSoundData("GamePlaying.mp3"));
			m_manager.ResetScene(std::make_shared<TitleScene>(m_manager));
			MyEngine::Physics::GetInstance().Clear();
		}
	}

}

void PauseScene::DisappearUpdate()
{
	m_frame -= 2;
	if (m_frame == 0)
	{
		m_manager.PopScene();
	}
}

void PauseScene::ExpandDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();

	int halfHeight = (size.h - 100) / 2;
	int centerY = size.h / 2;

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// 現在の時間の割合(0.0～1.0)
	int currentHalfHeight = static_cast<int>(rate * halfHeight);

	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void PauseScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawRotaString(670, 300, 6, 6, 0, 0, 0, 0xffffbb, 0, 0, "Pause");

	DrawString(750, 500, "ゲームに戻る", 0xffffff);
	DrawString(770, 600, "操作説明", 0xffffff);
	DrawString(760, 700, "タイトルへ", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255 / 3);
	if (m_select % 3 == 0)
	{
		DrawBox(700, 480, 900, 550, 0xffffff, true);
	}
	if (m_select % 3 == 1 || m_select % 3 == -2)
	{
		DrawBox(700, 580, 900, 650, 0xffffff, true);
	}
	if (m_select % 3 == 2 || m_select % 3 == -1)
	{
		DrawBox(700, 680, 900, 750, 0xffffff, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	if (m_tutoFlag)
	{
		DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0.8f, 0, m_tutoHandle, false);

	}

	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0xffffff, false);

}
