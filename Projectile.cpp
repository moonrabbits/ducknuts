#include "Projectile.h"

Projectile::Projectile(Vector2& actorPos, float xdir, float ydir) :
	m_Speed(PROJECTILE_SPEED),
	m_Pos(actorPos)
{
	m_Dir.x = xdir;
	m_Dir.y = ydir;
}

void Projectile::SetPos(float xPos, float yPos)
{
	m_Pos.x = xPos;
	m_Pos.y = yPos;
}
void Projectile::SetDir(float xDir, float yDir)
{
	m_Dir.x = xDir;
	m_Dir.y = yDir;
}
Vector2& Projectile::GetPos()
{
	return m_Pos;
}
void Projectile::UpdateProj(float deltaTime)
{
	m_Pos.x -= m_Dir.x * deltaTime*m_Speed;
	m_Pos.y -= m_Dir.y * deltaTime*m_Speed;
}

Vector2& Projectile::GetDir()
{
	return m_Dir;
}