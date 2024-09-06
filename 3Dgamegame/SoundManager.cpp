#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager info;
	return info;
}

void SoundManager::ChangeSoundVolume(int vol)
{
	for (auto& item : m_pathAndSoundInfoes)
	{
		SetVolumeSoundMem(vol,item.second.handle);
	}
}

int SoundManager::GetSoundData(const char* filepath)
{
	if (m_pathAndSoundInfoes.find(filepath) == m_pathAndSoundInfoes.end())
	{
		SoundInfo m = SoundInfo();
		m.handle = DxLib::LoadSoundMem(filepath);
		m.used = false;
		m_pathAndSoundInfoes[filepath] = m;
		return m.handle;
	}
	else {
		m_pathAndSoundInfoes[filepath].used = true;
		return m_pathAndSoundInfoes[filepath].handle;
	}
}
