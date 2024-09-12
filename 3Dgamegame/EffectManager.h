#pragma once
#include<map>
#include<string>
#include<EffekseerForDXLib.h>
struct EffectInfo {
	int handle;//�n���h��
	bool used;//��x�ȏ�g�p�ς݂��H
};
class EffectManager
{
private:
	std::map<std::string, EffectInfo> m_pathAndEffectInfoes;
	
public:
	EffectManager();
	~EffectManager();

	static EffectManager& GetInstance();
	int GetEffectData(const char* filepath);
	void DeleteEffectData(const char* effectname);
};

