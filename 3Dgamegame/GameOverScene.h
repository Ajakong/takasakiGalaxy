#pragma once
#include "Scene.h"
#include"MyLib//Vec3.h"

class GamePlayingScene;
class Player;

class GameOverScene : public Scene
{
public:
    GameOverScene(SceneManager& mgr);

    ~GameOverScene();

    void Load();
    void Update();
    void Draw();

private:

    int m_btnFrame = 0;
    int m_frame;
    int m_fadeSoundFrame = 0;

    int m_selectNumber = 0;

    int m_modelHandle = -1;

    Vec3 m_handleVelo;
    // �����o�֐��|�C���^�̐錾
    using UpdateFunc_t = void (GameOverScene::*)();
    using DrawFunc_t = void (GameOverScene::*)();
    UpdateFunc_t m_updateFunc;
    DrawFunc_t m_drawFunc;

    // �X�V��Ԃ�\���֐�
    void FadeInUpdate();
    void NormalUpdate();
    void FadeOutUpdate();

    // �`���Ԃ�\���֐�
    void FadeDraw();
    void NormalDraw();
};

