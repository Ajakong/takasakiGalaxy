#include"DxLib.h"
// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"
#include "GameManager.h"
#include"Camera.h"
#include"MyLib/Physics/Physics.h"
#include"Player.h"
#include"SpherePlanet.h"
#include"Enemy/Takobo.h"
#include"Enemy/Gorori.h"
#include"Object/Item.h"
#include<cassert>
#include"WorldTimer.h"
#include"Pad.h"

namespace
{
	//UI
	//ミッション
	constexpr int kUiText_SrkX = 150;
	constexpr int kUiText_SrkY = 170;
	constexpr int kUiText_Width = 1050;
	constexpr int kUiText_Height = 450;
	constexpr float kUiText_Exrate = 0.2f;

	//HPバーの枠
	constexpr int kUiHpbarFrame_PosX = 150;
	constexpr int kUiHpbarFrame_PosY = 50;
	constexpr int kUiHpbarFrame_SrkX = 2600;
	constexpr int kUiHpbarFrame_SrkY = 2560;
	constexpr int kUiHpbarFrame_Width = 1000;
	constexpr int kUiHpbarFrame_Height = 240;
	constexpr float kUiHpbarFrame_Exrate = 0.3f;

	//HPバー
	constexpr int kUiHpbar_PosX = 15;
	constexpr int kUiHpbar_PosY = 25;
	constexpr int kUiHpbar_Height = 23;
	constexpr float kUiHpbar_mag = 5.35f;//HPに掛けたらいい感じになる倍率

	//タイマーの枠
	constexpr int kUiTimeCountFrame_PosX = 1400;
	constexpr int kUiTimeCountFrame_PosY = 100;
	constexpr int kUiTimeCountFrame_SrkX = 1280;
	constexpr int kUiTimeCountFrame_SrkY = 130;
	constexpr int kUiTimeCountFrame_Width = 1040;
	constexpr int kUiTimeCountFrame_Height = 410;
	constexpr float kUiTimeCountFrame_Exrate = 0.3f;

	//タイマー
	constexpr int kUiTimeCount_PosX = 1350;
	constexpr int kUiTimeCount_PosY = 90;


}

GameManager::GameManager() :
	/*modelH(MV1LoadModel("Player/knight.mv1")),
	roughH(LoadGraph("Model/Sphere/roughness.png")),
	metalH(LoadGraph("Model/Sphere/metalness.png")),
	toonH(LoadGraph("Image/toon01.bmp")),
	psH(LoadPixelShader("PixelShader.pso")),
	vsH(LoadVertexShader("VertexShader.vso")),
	outlinePsH(LoadPixelShader("OutlinePS.pso")),
	outlineVsH(LoadVertexShader("OutlineVS.vso")),
	dissolveH(LoadGraph("Image/dissolve.png")),
	postEffectH(LoadPixelShader("PostEffect.pso")),*/
	textureUIHandle(LoadGraph("image/Elements-02.png")),
	
	// 通常のRT
	RT(MakeScreen(640, 480, true)),
	RT2(MakeScreen(640, 480, true)),
	// アウトラインのRT
	outlineRT(MakeScreen(640, 480, true)),
	// 法線情報のRT
	normRT(MakeScreen(640, 480)),
	// ぼかし用RT
	blurRT(MakeScreen(640, 480, true)),
	shrinkRT(MakeScreen(320, 240, true)),
	depthRT(MakeScreen(640, 480)),
	/*skyDomeH(MV1LoadModel("Model/Skydome/universe_skydome.mv1")),*/
	m_isClearFlag(false),
	fadeCount(100),
	itemNum(0)
{
	assert(modelH != -1);
	assert(roughH != -1);
	assert(metalH != -1);
	assert(toonH != -1);
	assert(psH != -1);
	assert(vsH != -1);
	assert(outlinePsH != -1);
	assert(outlineVsH != -1);
	assert(dissolveH != -1);
	assert(postEffectH != -1);
	player = std::make_shared<Player>(modelH);
	camera = std::make_shared<Camera>();
	planet = std::make_shared<SpherePlanet>(Vec3(0,-500,0));
	planet2 = std::make_shared<SpherePlanet>(Vec3(3000,0,1000));
	takobo = { std::make_shared<Takobo>(Vec3(300,0,500)),std::make_shared<Takobo>(Vec3(-300,0,500)),std::make_shared<Takobo>(Vec3(0,0,700)) };
	gorori= { std::make_shared<Gorori>(Vec3(500,0,300)),std::make_shared<Gorori>(Vec3(500,0,-300)),std::make_shared<Gorori>(Vec3(700,0,0)) };
	poworStone.push_back( std::make_shared<Item>(Vec3(0, -1000, 0)));
	poworStone.push_back(std::make_shared<Item>(Vec3(-1000, 0, 0)));
	poworStone.push_back(std::make_shared<Item>(Vec3(1000, 0, 0)));
	poworStone.push_back(std::make_shared<Item>(Vec3(0, 1000, 0)));
	poworStone.push_back(std::make_shared<Item>(Vec3(0, 0, 1000)));
	poworStone.push_back(std::make_shared<Item>(Vec3(0, 0, -1000)));

	//使用するフォントを準備する
	if (AddFontResourceEx("Font/disital.TTF", FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}
	fontHandle = CreateFontToHandle("Pocket Calculator", 60,7, DX_FONTTYPE_NORMAL);

	m_managerUpdate = &GameManager::IntroUpdate;
	m_managerDraw = &GameManager::IntroDraw;
}

GameManager::~GameManager()
{
	DeleteGraph(textureUIHandle);
}

void GameManager::Init()
{
	/*MATERIALPARAM Material;

	Material.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.5f);
	Material.Specular = GetColorF(0.5f, 0.5f, 0.5f, 0.5f);
	Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Power = 20.0f;
	SetMaterialParam(Material);
	SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.0f));*/
	SetGlobalAmbientLight(GetColorF(1.0f, 0.0f, 0.0f, 0.0f));
	player->SetMatrix();


	// メッシュの数を取ってくる
	auto meshNum = MV1GetMeshNum(modelH);

	VECTOR maxPos = { 0, 0, 0 };
	VECTOR minPos = { 1000, 1000, 1000 };
	bool hasNormalMap = false;
	for (int i = 0; i < meshNum; ++i)
	{
		// 輪切りの時は裏側も描画しないと変になる
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_NONE);

		// モデルの大きさを取得
		auto modelMaxPos = MV1GetMeshMaxPosition(modelH, i);
		maxPos.x = std::max(maxPos.x, modelMaxPos.x);
		maxPos.y = std::max(maxPos.y, modelMaxPos.y);
		maxPos.z = std::max(maxPos.z, modelMaxPos.z);

		auto modelMinPos = MV1GetMeshMinPosition(modelH, i);
		minPos.x = std::min(minPos.x, modelMinPos.x);
		minPos.y = std::min(minPos.y, modelMinPos.y);
		minPos.z = std::min(minPos.z, modelMinPos.z);

		auto vtype = MV1GetTriangleListVertexType(modelH, i);
		if (vtype == DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
		{
			hasNormalMap = true;
		}
	}
	userData->minY = minPos.y;
	userData->maxY = maxPos.y;
	userData->clickedU = 0.0f;
	userData->clickedV = 0.0f;

	// 深度値記録バッファ用RT
	DxLib::SetCreateGraphChannelBitDepth(32);
	DxLib::SetCreateDrawValidGraphChannelNum(1);

	MyEngine::Physics::GetInstance().Entry(player);
	MyEngine::Physics::GetInstance().Entry(planet);
	MyEngine::Physics::GetInstance().Entry(planet2);
	for (auto& item : poworStone)MyEngine::Physics::GetInstance().Entry(item);
	for (auto& item : takobo)
	{
		MyEngine::Physics::GetInstance().Entry(item);
		item->SetTarget(player);
	}
	for (auto& item : gorori)
	{
		MyEngine::Physics::GetInstance().Entry(item);
		item->SetTarget(player);
	}
}

void GameManager::Update()
{
	(this->*m_managerUpdate)();
}

void GameManager::Draw()
{
	(this->*m_managerDraw)();
}

void GameManager::IntroUpdate()
{
	
	if (m_isFadeIntroFlag)
	{
		fadeCount++;
	}
	else
	{
		fadeCount-=3;
		if (fadeCount < 0)fadeCount = 0;
	}
	MyEngine::Physics::GetInstance().Update();
	Vec3 planetToPlayer = player->GetPos() - planet->PlanetOnlyGetRigid().GetPos();
	Vec3 sideVec = GetCameraRightVector();
	Vec3 front = Cross(planetToPlayer, sideVec).GetNormalized() * -1;
	player->SetSideVec(sideVec);
	player->SetFrontVec(front);
	player->SetUpVec(planetToPlayer);

	{
		//本当はカメラとプレイヤーの角度が90度以内になったときプレイヤーの頭上を見たりできるようにしたい。
		//camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * 100 - Vec3(GetCameraFrontVector())* 300));
		camera->SetUpVec(planet->GetNormVec(player->GetPos()));
		camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * 100 - front * (300+300*player->GetJumpFlag())));
	}

	camera->Update(player->GetPos());

	// カリング方向の反転
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_RIGHT);
	}
	/*SetUseVertexShader(outlineVsH);
	SetUsePixelShader(outlinePsH);*/
	// カリング方向を元に戻す
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_LEFT);
	}
	if (Pad::IsTrigger(PAD_INPUT_1))m_isFadeIntroFlag = true;
	if (fadeCount > 100)
	{
		m_managerUpdate = &GameManager::GamePlayingUpdate;
		m_managerDraw = &GameManager::GamePlayingDraw;
	}

}

void GameManager::IntroDraw()
{
	
	

	MV1SetPosition(skyDomeH, player->GetPos().VGet());

	MV1DrawModel(skyDomeH);
	planet->Draw();
	player->Draw();

	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : takobo)item->Draw();
	for (auto& item : gorori)item->Draw();
	camera->DebagDraw();
	SetRenderTargetToShader(1, -1);		// RTの解除
	SetRenderTargetToShader(2, -1);		// RTの解除

	//タイマー
	DrawRectRotaGraph(kUiTimeCountFrame_PosX, kUiTimeCountFrame_PosY, kUiTimeCountFrame_SrkX, kUiTimeCountFrame_SrkY, kUiTimeCountFrame_Width, kUiTimeCountFrame_Height, kUiTimeCountFrame_Exrate, 0, textureUIHandle, 1,1);
	
	DrawFormatStringToHandle(kUiTimeCount_PosX, kUiTimeCount_PosY, 0xffffff, fontHandle, "%d.%d m/s", WorldTimer::GetMinute(), WorldTimer::GetTimer());
	//HPバー
	DrawRectRotaGraph(kUiHpbarFrame_PosX, kUiHpbarFrame_PosY, kUiHpbarFrame_SrkX, kUiHpbarFrame_SrkY, kUiHpbarFrame_Width, kUiHpbarFrame_Height,0.3f, 0, textureUIHandle, true);
	DrawBox(15, 25,static_cast<int>(15 + player->WatchHp() * kUiHpbar_mag), kUiHpbar_PosY + kUiHpbar_Height, 0x00ffff, true);
	//ミッション
	DrawRectRotaGraph(kUiText_SrkX + (100 * 7 - fadeCount * 7), static_cast<int>(kUiText_SrkY + (100 * 2.2f - fadeCount * 2.2f)), kUiText_SrkX, kUiText_SrkY, kUiText_Width, kUiText_Height, kUiText_Exrate * 1 + 1.25f * ((100.f - fadeCount) / 100.f), 0, textureUIHandle, true);
}

void GameManager::GamePlayingUpdate()
{
	/*FillGraph(depthRT, 0, 0, 0, 0);
	FillGraph(shrinkRT, 0, 0, 0, 0);
	FillGraph(normRT, 0, 0, 0, 0);*/

	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}
	//// 使用するシェーダをセットしておく
	//SetUseVertexShader(vsH);
	//SetUsePixelShader(psH);

	//UpdateShaderConstantBuffer(cbuffH);
	//SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_PIXEL, 4);

	//// シェーダーやってる部分
	//SetUseTextureToShader(3, dissolveH);
	//SetUseTextureToShader(4, sphMapH);
	//SetUseTextureToShader(5, roughH);
	//SetUseTextureToShader(6, metalH);
	//SetUseTextureToShader(7, toonH);
	////		SetRenderTargetToShader(0, RT);	// 0番にRTを設定
	//SetRenderTargetToShader(1, depthRT);
	//SetRenderTargetToShader(2, normRT);

	//MV1SetUseOrigShader(true);

	/* カメラの設定
	 RTを設定するとカメラの初期化が入ってるかもなので、RTの設定後にカメラの設定を行う*/

	 /*camera->SetUpVec(planet->GetNormVec(player->GetPos()));
	 camera->Update(player->GetPos());*/
	planet->Update();
	planet2->Update();
	//player->SetCameraToPlayer(camera->cameraToPlayer(player->GetPos()));
	for (auto& item : poworStone)
	{
		item->Update();
	}
	player->Update();

	for (auto& item : takobo)item->Update();
	for (auto& item : gorori)item->Update();

	userData->dissolveY = player->GetRegenerationRange();

	MyEngine::Physics::GetInstance().Update();
	for (int i = 0; i < takobo.size(); i++)
	{
		takobo[i]->DeleteManage();
		if (takobo[i]->WatchHp() < 0)
		{
			MyEngine::Physics::GetInstance().Exit(takobo[i]);

			takobo.erase(takobo.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}
	for (int i = 0; i < gorori.size(); i++)
	{
		if (gorori[i]->WatchHp() < 0)
		{
			MyEngine::Physics::GetInstance().Exit(gorori[i]);

			gorori.erase(gorori.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}
	for (int i = 0; i < poworStone.size(); i++)
	{
		if (poworStone[i]->GetDeleteFlag())
		{
			MyEngine::Physics::GetInstance().Exit(poworStone[i]);

			poworStone.erase(poworStone.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}

	Vec3 planetToPlayer = player->GetPos() - planet->PlanetOnlyGetRigid().GetPos();
	Vec3 sideVec = GetCameraRightVector();
	Vec3 front = Cross(planetToPlayer, sideVec).GetNormalized() * -1;
	player->SetSideVec(sideVec);
	player->SetFrontVec(front);
	player->SetUpVec(planetToPlayer);
	/*Vec3 playerToCamera = camera->GetPos() -player->GetPos();
	float a = acos(Dot(planetToPlayer.GetNormalized(), playerToCamera.GetNormalized())) * 180 / DX_PI_F;

	if ( a> 68)*/
	{
		//本当はカメラとプレイヤーの角度が90度以内になったときプレイヤーの頭上を見たりできるようにしたい。
		//camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * 100 - Vec3(GetCameraFrontVector())* 300));
		camera->SetUpVec(planet->GetNormVec(player->GetPos()));
		camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * 100 - front * (300 + 200 * player->GetJumpFlag())));
	}


	camera->Update(player->GetPos());
	//camera->SetCameraPos(player->GetPos());

	player->SetMatrix();
	for (auto& item : takobo)item->SetMatrix();
	// カリング方向の反転
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_RIGHT);
	}
	/*SetUseVertexShader(outlineVsH);
	SetUsePixelShader(outlinePsH);*/
	// カリング方向を元に戻す
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_LEFT);
	}

#if false
	//MV1SetRotationXYZ(modelH, VGet(0, angle, 0));
	//MV1SetPosition(modelH, VGet(100.0f, 50.0f, 300.0f));
	//MV1DrawModel(modelH);
	SetDrawScreen(RT2);
	ClearDrawScreen();
	GraphFilterBlt(RT, blurRT, DX_GRAPH_FILTER_GAUSS, 8, 1400);
	GraphFilterBlt(blurRT, shrinkRT, DX_GRAPH_FILTER_DOWN_SCALE, 2);
	GraphFilter(shrinkRT, DX_GRAPH_FILTER_GAUSS, 8, 1400);
	//// 通常描画
	//DrawGraph(0, 0, RT, true);
	//// ぼかし描画
	//DrawGraph(0, 0, blurRT, true);
	//// 深度を描画
	//DrawExtendGraph(0, 0, 160, 120, depthRT, false);
	SetRenderTargetToShader(1, outlineRT);
	MyEngine::DrawGraph(0, 0, RT, postEffectH, blurRT, depthRT, normRT, true);

	SetRenderTargetToShader(1, -1);

	SetUseTextureToShader(0, -1);		// テクスチャの解除
	SetUseTextureToShader(1, -1);		// テクスチャの解除
	SetUseTextureToShader(2, -1);		// テクスチャの解除
	SetUseTextureToShader(3, -1);		// テクスチャの解除

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	GraphFilterBlt(outlineRT, blurRT, DX_GRAPH_FILTER_GAUSS, 8, 1400);
	GraphFilterBlt(blurRT, shrinkRT, DX_GRAPH_FILTER_DOWN_SCALE, 2);
	GraphFilter(shrinkRT, DX_GRAPH_FILTER_GAUSS, 8, 1400);

	DrawGraph(0, 0, RT2, true);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawGraph(0, 0, outlineRT, true);
	DrawGraph(0, 0, blurRT, true);
	DrawGraph(0, 0, blurRT, true);
	DrawExtendGraph(0, 0, 640, 480, shrinkRT, true);
	DrawExtendGraph(0, 0, 640, 480, shrinkRT, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#endif

	if (player->GetHp() <= 0)
	{
		m_isGameOverFlag = true;
	}
	if (planet->GetClearFlag())
	{
		m_isClearFlag = true;
		MyEngine::Physics::GetInstance().Clear();
	}
	if (poworStone.size() == 0)
	{
		m_isClearFlag = true;
	}

	WorldTimer::Update();
}

void GameManager::GamePlayingDraw()
{
	DrawRectRotaGraph(kUiText_SrkX, kUiText_SrkY, kUiText_SrkX, kUiText_SrkY, kUiText_Width, kUiText_Height, kUiText_Exrate, 0, textureUIHandle, true);

	MV1SetPosition(skyDomeH, player->GetPos().VGet());

	MV1DrawModel(skyDomeH);
	planet->Draw();
	player->Draw();
	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : takobo)item->Draw();
	for (auto& item : gorori)item->Draw();
	camera->DebagDraw();
	SetRenderTargetToShader(1, -1);		// RTの解除
	SetRenderTargetToShader(2, -1);		// RTの解除

	//タイマー
	DrawRectRotaGraph(kUiTimeCountFrame_PosX, kUiTimeCountFrame_PosY, kUiTimeCountFrame_SrkX, kUiTimeCountFrame_SrkY, kUiTimeCountFrame_Width, kUiTimeCountFrame_Height, kUiTimeCountFrame_Exrate, 0, textureUIHandle,1,1);
	DrawFormatStringToHandle(kUiTimeCount_PosX, kUiTimeCount_PosY, 0xffffff, fontHandle, "%d.%d m/s", WorldTimer::GetMinute(), WorldTimer::GetTimer());

	//ミッション
	DrawRectRotaGraph(kUiText_SrkX, kUiText_SrkY, kUiText_SrkX, kUiText_SrkY, kUiText_Width, kUiText_Height, kUiText_Exrate, 0, textureUIHandle, true);

	//HPバー
	DrawRectRotaGraph(kUiHpbarFrame_PosX, kUiHpbarFrame_PosY, kUiHpbarFrame_SrkX, kUiHpbarFrame_SrkY, kUiHpbarFrame_Width, kUiHpbarFrame_Height, 0.3f, 0, textureUIHandle, true);
	DrawBox(kUiHpbar_PosX, kUiHpbar_PosY, static_cast<int>(kUiHpbar_PosX + player->WatchHp() * kUiHpbar_mag), kUiHpbar_PosY +kUiHpbar_Height, 0x00ffff, true);

	
}
