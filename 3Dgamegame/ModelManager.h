#pragma once
#include<map>
#include<string>
#include"DxLib.h"

#include"HandleManager.h"
struct ModelInfo {
	int handle;//ハンドル
	bool used;//一度以上使用済みか？
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

