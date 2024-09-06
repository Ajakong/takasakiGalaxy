#include "GraphManager.h"

GraphManager::GraphManager()
{
}

GraphManager::~GraphManager()
{
}

GraphManager& GraphManager::GetInstance()
{
	static GraphManager info;
	return info;
}

int GraphManager::GetGraphData(const char* filepath)
{
	if (m_pathAndGraphInfoes.find(filepath) == m_pathAndGraphInfoes.end())
	{
		GraphInfo m = GraphInfo();
		m.handle = DxLib::LoadGraph(filepath);
		m.used = false;
		m_pathAndGraphInfoes[filepath] = m;
		return m.handle;
	}
	else {
		m_pathAndGraphInfoes[filepath].used = true;
		return m_pathAndGraphInfoes[filepath].handle;
	}
}
