#pragma once
#include "Actor.h"
#include "Constants.h"
#include <vector>
#include <cmath>
class Praetor : public Actor
{
private:
	float m_Speed;
	float m_Radius;
	float m_Theta;
	bool m_Shifting;
	Vector2 m_Location1;
	Vector2 m_Location2;
	Vector2 m_Location3;
	Vector2 m_Location4;
	unsigned int m_RevCount;
public:
	Praetor();
	void UpdateActor(float deltaTime, float speed, Vector2 center, float increment);
	void ShiftLocation(Praetor& aPraetor, Vector2& currentLocation, Vector2& targetLocation);
	void DeathBoom();
	float& GetTheta();
	std::vector<Vector2> m_Locations;


};
