#pragma once
#include "Constants.h"

class Actor
{
public:
	Actor();

	void SetDir(float x, float y);
	void SetPos(float x, float y);
	void StopMotion();

	virtual void UpdateActor(float deltaTime, float speed);

	Vector2& GetPos();
	Vector2& GetDir();
	Vector2 m_Dir;
	float& GetHP();
	void TakeProjDamage(float damage);
	int m_TimeOnWall;
protected:
	Vector2 m_Pos;
	float m_Health;
};
