//#include<DxLib.h>
//#include "ModelLoader.h"
//#include"Model.h"
//#include <cassert>
//
//
//ModelLoader::ModelLoader()
//{
//	vsH_ = LoadVertexShader("MV1VertexShader.vso");
//	assert(vsH_ != -1);
//	psH_ = LoadPixelShader("MV1PixelShader.pso");
//	assert(psH_ != -1);
//	SetUseASyncLoadFlag(true);
//}
//
//
//ModelLoader::~ModelLoader()
//{
//}
//
//
/////���f���f�[�^�̃v�����[�h
/////@param filepath �t�@�C���p�X
//void
//ModelLoader::PreloadModelData(const char* filepath) {
//	if (_pathAndModelInfoes.find(filepath) == _pathAndModelInfoes.end()) {
//		ModelInfo m = ModelInfo();
//		m.handle = DxLib::MV1LoadModel(filepath);
//		m.used = false;
//		_pathAndModelInfoes[filepath] = m;
//	}
//}
//
/////���f���f�[�^�����[�h���ă��f���I�u�W�F�N�g�����
/////@param filepath �t�@�C���p�X
/////@return ���f���N���X�|�C���^
//Model*
//ModelLoader::CreateModel(const char* filepath) {
//	if (_pathAndModelInfoes.find(filepath) == _pathAndModelInfoes.end()) {
//		ModelInfo m = ModelInfo();
//		m.handle = DxLib::MV1LoadModel(filepath);
//		m.used = false;
//		_pathAndModelInfoes[filepath] = m;
//	}
//	if (_pathAndModelInfoes[filepath].used) {
//		return new Model(DxLib::MV1DuplicateModel(_pathAndModelInfoes[filepath].handle), vsH_, psH_);
//	}
//	else {
//		_pathAndModelInfoes[filepath].used = true;
//		return new Model(_pathAndModelInfoes[filepath].handle, vsH_, psH_);
//	}
//}