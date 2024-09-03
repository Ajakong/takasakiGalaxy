#include"DxLib.h"
// EffekseerForDXLib.h���C���N���[�h���܂��B
#include "EffekseerForDXLib.h"
#include "GameManager.h"
#include"Camera.h"
#include"MyLib/Physics/Physics.h"
#include"Player.h"
#include"SpherePlanet.h"
#include"Enemy/Takobo.h"
#include<cassert>

GameManager::GameManager() :
	modelH(MV1LoadModel("Player/knight.mv1")),
	roughH(LoadGraph("Model/Sphere/roughness.png")),
	metalH(LoadGraph("Model/Sphere/metalness.png")),
	toonH(LoadGraph("Image/toon01.bmp")),
	psH(LoadPixelShader("PixelShader.pso")),
	vsH(LoadVertexShader("VertexShader.vso")),
	outlinePsH(LoadPixelShader("OutlinePS.pso")),
	outlineVsH(LoadVertexShader("OutlineVS.vso")),
	dissolveH(LoadGraph("Image/dissolve.png")),
	postEffectH(LoadPixelShader("PostEffect.pso")),
	// �ʏ��RT
	RT(MakeScreen(640, 480, true)),
	RT2(MakeScreen(640, 480, true)),
	// �A�E�g���C����RT
	outlineRT(MakeScreen(640, 480, true)),
	// �@������RT
	normRT(MakeScreen(640, 480)),
	// �ڂ����pRT
	blurRT(MakeScreen(640, 480, true)),
	shrinkRT(MakeScreen(320, 240, true)),
	depthRT(MakeScreen(640, 480))
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
	
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	player->SetMatrix();


	// ���b�V���̐�������Ă���
	auto meshNum = MV1GetMeshNum(modelH);

	VECTOR maxPos = { 0, 0, 0 };
	VECTOR minPos = { 1000, 1000, 1000 };
	bool hasNormalMap = false;
	for (int i = 0; i < meshNum; ++i)
	{
		// �֐؂�̎��͗������`�悵�Ȃ��ƕςɂȂ�
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_NONE);

		// ���f���̑傫�����擾
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

	// �[�x�l�L�^�o�b�t�@�pRT
	SetCreateGraphChannelBitDepth(32);
	SetCreateDrawValidGraphChannelNum(1);

	MyEngine::Physics::GetInstance().Entry(player);
	MyEngine::Physics::GetInstance().Entry(planet);
	MyEngine::Physics::GetInstance().Entry(planet2);
	for(auto& item : takobo)MyEngine::Physics::GetInstance().Entry(item);
}

void GameManager::Update()
{
	FillGraph(depthRT, 0, 0, 0, 0);
	FillGraph(shrinkRT, 0, 0, 0, 0);
	FillGraph(normRT, 0, 0, 0, 0);

	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}
	// �g�p����V�F�[�_���Z�b�g���Ă���
	SetUseVertexShader(vsH);
	SetUsePixelShader(psH);

	UpdateShaderConstantBuffer(cbuffH);
	SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_PIXEL, 4);

	// �V�F�[�_�[����Ă镔��
	SetUseTextureToShader(3, dissolveH);
	SetUseTextureToShader(4, sphMapH);
	SetUseTextureToShader(5, roughH);
	SetUseTextureToShader(6, metalH);
	SetUseTextureToShader(7, toonH);
	//		SetRenderTargetToShader(0, RT);	// 0�Ԃ�RT��ݒ�
	SetRenderTargetToShader(1, depthRT);
	SetRenderTargetToShader(2, normRT);

	MV1SetUseOrigShader(true);

	/* �J�����̐ݒ�
	 RT��ݒ肷��ƃJ�����̏������������Ă邩���Ȃ̂ŁART�̐ݒ��ɃJ�����̐ݒ���s��*/
	
	/*camera->SetUpVec(planet->GetNormVec(player->GetPos()));
	camera->Update(player->GetPos());*/
	planet->Update();
	planet2->Update();
	//player->SetCameraToPlayer(camera->cameraToPlayer(player->GetPos()));

	
	player->Update();
	
	for (auto& item : takobo)item->Update();
	
	userData->dissolveY = player->GetRegenerationRange();

	MyEngine::Physics::GetInstance().Update();
	for (int i=0;i<takobo.size();i++)
	{
		if (takobo[i]->WatchHp() < 0)
		{
			MyEngine::Physics::GetInstance().Exit(takobo[i]);

			takobo.erase(takobo.begin()+i);//�������̗�����̂܂܎g����(1,2,5,3,4)��it�ɂ�5�܂œ����Ă�̂Ŏ�菜������3,4��it����end()�܂łŎw��ł���
			i--;
		}
	}
	Vec3 planetToPlayer = player->GetPos() - planet->PlanetOnlyGetRigid().GetPos();
	Vec3 playerToCamera = camera->GetPos() -player->GetPos();
	float a = acos(Dot(planetToPlayer.GetNormalized(), playerToCamera.GetNormalized())) * 180 / DX_PI_F;
	if ( a+32> 90)
	{
		camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * 100 - Vec3(GetCameraFrontVector()).GetNormalized() * 300));
	}
	camera->SetUpVec(planet->GetNormVec(player->GetPos()));
	camera->Update(player->GetPos());
	//camera->SetCameraPos(player->GetPos());

	player->SetMatrix();
	for (auto& item : takobo)item->SetMatrix();
	// �J�����O�����̔��]
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_RIGHT);
	}
	SetUseVertexShader(outlineVsH);
	SetUsePixelShader(outlinePsH);
	MV1DrawModel(modelH);
	// �J�����O���������ɖ߂�
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
	//// �ʏ�`��
	//DrawGraph(0, 0, RT, true);
	//// �ڂ����`��
	//DrawGraph(0, 0, blurRT, true);
	//// �[�x��`��
	//DrawExtendGraph(0, 0, 160, 120, depthRT, false);
	SetRenderTargetToShader(1, outlineRT);
	MyEngine::DrawGraph(0, 0, RT, postEffectH, blurRT, depthRT, normRT, true);

	SetRenderTargetToShader(1, -1);

	SetUseTextureToShader(0, -1);		// �e�N�X�`���̉���
	SetUseTextureToShader(1, -1);		// �e�N�X�`���̉���
	SetUseTextureToShader(2, -1);		// �e�N�X�`���̉���
	SetUseTextureToShader(3, -1);		// �e�N�X�`���̉���

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


}

void GameManager::Draw()
{
	planet->Draw();
	player->Draw();
	for (auto& item : takobo)item->Draw();
	camera->DebagDraw();
	SetRenderTargetToShader(1, -1);		// RT�̉���
	SetRenderTargetToShader(2, -1);		// RT�̉���

}
