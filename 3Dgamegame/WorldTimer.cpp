#include "WorldTimer.h"

namespace
{
	int m_frame = 0;
	int m_timeCount = 0;
}

int WorldTimer::GetTimer()
{
	return m_timeCount;
}

int WorldTimer::GetFrame()
{
	return m_frame;
}

void WorldTimer::Update()
{
	m_frame++;
	if (m_frame % 60 == 0)
	{
		m_timeCount++;
	}
}

void WorldTimer::Reset()
{
	m_frame = 0;
	m_timeCount = 0;
}
