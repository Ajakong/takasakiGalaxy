#pragma once

class Camera;
class Player;
class Planet;
class Takobo;
class Gorori;
class Item;
class WarpGate;

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

	using managerState_t = void(GameManager::*)();
	managerState_t m_managerUpdate;

	using managerState_t = void(GameManager::*)();
	managerState_t m_managerDraw;
private:
	void IntroUpdate();
	void IntroDraw();

	void GamePlayingUpdate();
	void GamePlayingDraw();

private:

	struct UserData
	{
		float dissolveY;	// ディゾルヴしたい高さ
		float minY;
		float maxY;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};
	int cbuffH = CreateShaderConstantBuffer(sizeof(UserData));
	UserData* userData = static_cast<UserData*>(GetBufferShaderConstantBuffer(cbuffH));

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Player> player;
	std::shared_ptr<Planet> planet;
	std::shared_ptr<Planet> planet2;
	std::vector<std::shared_ptr<WarpGate>> warpGate;
	std::vector<std::shared_ptr<Takobo>> takobo;
	std::vector<std::shared_ptr<Gorori>> gorori;
	std::vector<std::shared_ptr<Item>> poworStone;
	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;

	bool m_isFadeIntroFlag = false;
	int skyDomeH;
	int modelH ;
	int textureUIHandle;
	int fontHandle;
	int sphMapH ;
	int itemNum;
	
	int roughH;
	
	int metalH;
	
	int toonH;
	
	int psH;
	
	int vsH;
	
	int outlinePsH;
	
	int outlineVsH;
	
	int dissolveH;
	
	int postEffectH;
	
	// 通常のRT
	int RT;
	int RT2;
	// アウトラインのRT
	int outlineRT;
	// 法線情報のRT
	int normRT;
	// ぼかし用RT
	int blurRT;
	int shrinkRT;

	
	int depthRT;

	int fadeCount;
};

