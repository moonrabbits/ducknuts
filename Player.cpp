#include "Player.h"



Player::Player()
{
}

void Player::TakeCollisionDamage(float damage)
{
	m_Health -= damage;
}