#pragma once
#include "Actor.h"
#include <vector>
class Player: public Actor
{
public:
	Player();
	void TakeCollisionDamage(float damage);
	void SpecialAttack(std::vector<class Projectile>& vec, float playerXPos, float playerYPos);
private:
	bool InvFrames;
};

