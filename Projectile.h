#pragma once
#include "Constants.h"
#include "Actor.h"

class Projectile
{
public:
	Projectile(Vector2& actorPos, float xdir, float ydir);
	void SetPos(float xPos, float yPos);
	void SetDir(float xDir, float yDir);
	void UpdateProj(float deltaTime);
	Vector2& GetPos();
	Vector2& GetDir();
	Vector2 m_Dir;

private:
	Vector2 m_Pos;
	float m_Speed;

};
