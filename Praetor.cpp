#include "Praetor.h"



Praetor::Praetor()
{
	m_Theta = 0.0f;
	m_Health = PRAETOR_HEALTH;
	m_Radius = 50.0f;
	// if health lower than x, move to center and exploderd
}

void Praetor::UpdateActor(float deltaTime, float speed, Vector2 center, float increment)
{
	m_Pos.x = center.x + m_Radius * cos(m_Theta);
	m_Pos.y = center.y + m_Radius * sin(m_Theta);
	if (!m_Shifting)
	{
		m_Theta += increment;
	}
	if (m_Shifting)
	{
		//ShiftLocation();
	}
	if (m_Theta >= 2.0f * PI)
	{
		m_Theta = 0.0f;
		m_RevCount++;
	}
	if (m_RevCount >= 5)
	{
		//m_Shifting = true;
	}
}

void Praetor::ShiftLocation(Praetor& aPraetor, Vector2& currentPos, Vector2& targetPos)
{
	//Find direction vector from current to target
	float deltaY = (targetPos.y - currentPos.y) *-1;
	float deltaX = targetPos.x - currentPos.x;
	float distance = sqrt((deltaY*deltaY) + (deltaX*deltaX));
	// if current pos != target pos
	//aPraetor.SetDir.x()

	// else 
	// m_Shifting = false;
	// m_Praetors[i].m_Dir.x = NULL;
	// m_Praetors[i].m_Dir.y = NULL;
}

float& Praetor::GetTheta()
{
	return m_Theta;
}