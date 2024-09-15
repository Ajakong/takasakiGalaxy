#pragma once
#include<map>
#include<string>
#include"DxLib.h"

#include"HandleManager.h"
struct ModelInfo {
	int handle;//�n���h��
	bool used;//��x�ȏ�g�p�ς݂��H
};
class ModelManager : public HandleManager
{
private:
	std::map<std::string, ModelInfo> m_pathAndModelInfoes;

public:
	ModelManager();
	~ModelManager();

	static ModelManager& GetInstance();

	int GetModelData(const char* filepath);
	void DeleteModelData(const char* filepath);
};

