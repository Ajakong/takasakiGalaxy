#pragma once

class Camera;
class Player;
class Planet;
class Takobo;

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

	struct UserData
	{
		float dissolveY;	// �f�B�]��������������
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
	std::vector<std::shared_ptr<Takobo>> takobo;

	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;

	int skyDomeH;
	int modelH ;
	
	int sphMapH ;
	
	int roughH;
	
	int metalH;
	
	int toonH;
	
	int psH;
	
	int vsH;
	
	int outlinePsH;
	
	int outlineVsH;
	
	int dissolveH;
	
	int postEffectH;
	
	// �ʏ��RT
	int RT;
	int RT2;
	// �A�E�g���C����RT
	int outlineRT;
	// �@������RT
	int normRT;
	// �ڂ����pRT
	int blurRT;
	int shrinkRT;

	
	int depthRT;
};

