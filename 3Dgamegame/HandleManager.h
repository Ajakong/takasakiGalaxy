#pragma once
#include<map>
#include<string>
#include"DxLib.h"
struct HandleInfo {
	int handle;//ハンドル
	bool used;//一度以上使用済みか？
};
class HandleManager
{
private:
	std::map<std::string, HandleInfo> m_pathAndModelInfoes;

public:
	HandleManager();
	~HandleManager();

	static HandleManager& GetInstance();

	int GetHandleData(const char* filepath);
	void DeleteHandleData(const char* filepath);
};

