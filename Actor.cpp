#include "Actor.h"

Actor::Actor():
	m_TimeOnWall(0),
	m_Health(PAWN_HEALTH)
{
}

// Setters
void Actor::SetPos(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}
void Actor::SetDir(float x, float y)
{
	m_Dir.x += x;
	m_Dir.y += y;
}
void Actor::StopMotion()
{
	m_Dir.x = 0;
	m_Dir.y = 0;
}
void Actor::TakeProjDamage(float damage)
{
	m_Health -= damage;
}

// Getters
Vector2& Actor::GetDir()
{
	return m_Dir;
}
Vector2& Actor::GetPos()
{
	return m_Pos;
}
float& Actor::GetHP()
{
	return m_Health;
}

// Other Actor Functions
void Actor::UpdateActor(float deltaTime, float speed)
{
	m_Pos.x += m_Dir.x*deltaTime*speed;
	m_Pos.y += m_Dir.y*deltaTime*speed;
}