#pragma once
#include "Enemy.h"
/// <summary>
/// ¶‰E‚ÉˆÚ“®‚µAˆê’èŠÔŠu‚ÅËŒ‚‚µ‚Ä‚­‚é“G
/// “¥‚İ‚Â‚¯‚ç‚ê‚é‚Æ‚Â‚Ô‚ê‚é
/// </summary>
class Takobo : public Enemy
{
public:
	Takobo();
	virtual ~Takobo();

	void Init();
	void Update();
	void SetMatrix();
	void Draw();

private:
	Vec3 m_position;
	Vec3 m_moveShaftPos;

	Vec3 m_vec;
};

