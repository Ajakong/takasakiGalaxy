#include "DxLib.h"
#include "Application.h"
#include <cassert>
#include <algorithm>
#include <array>

namespace MyEngine
{
	int DrawGraph(float x, float y, int handle, int psH, int secondH = -1, int thirdH = -1, int forthH = -1, bool isTrans = false)
	{
		int w, h;
		GetGraphSize(handle, &w, &h);

		std::array<VERTEX2DSHADER, 4> vertices;

		for (auto& v : vertices)
		{
			v.dif = GetColorU8(255, 255, 255, 255);
			v.spc = GetColorU8(0, 0, 0, 0);
			v.rhw = 1.0f;
			v.su = 0.f;
			v.sv = 0.f;
		}

		//��`�ɂȂ�悤�ɕ��ׂĂ���
		//Z�̏��ԂɂȂ�悤�ɒ��_����ׂĂ���
		//����
		vertices[0].pos = VGet(x, y, 0.0f);
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;
		//�E��
		vertices[1].pos = VGet(x + w, y, 0.0f);
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;
		//����
		vertices[2].pos = VGet(x, y + h, 0.0f);
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;
		//�E��
		vertices[3].pos = VGet(x + w, y + h, 0.0f);
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		if (isTrans)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}

		//������̕`��Ɏg���摜��ݒ肷��
		SetUseTextureToShader(0, handle);
		if (secondH != -1)
		{
			SetUseTextureToShader(1, secondH);
		}
		if (thirdH != -1)
		{
			SetUseTextureToShader(2, thirdH);
		}
		if (forthH != -1)
		{
			SetUseTextureToShader(3, forthH);
		}

		//������g���V�F�[�_�[��ݒ�
		SetUsePixelShader(psH);

		std::array<uint16_t, 6> indces;
		indces = { 0,1,3,0,3,2 };

		return DrawPrimitiveIndexed2DToShader(vertices.data(),			//�A�h���X
			static_cast<int>(vertices.size()),			//���_��
			indces.data(),
			static_cast<int>(indces.size()),
			DX_PRIMTYPE_TRIANGLELIST);//�g�|���W�|
	}
}


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Application& application = Application::GetInstance();

	if (!application.Init())
	{
		return -1;// ���̃A�v���͕s���I�����܂����B
	}
	application.Run();


	return 0;
}