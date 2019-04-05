#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Projectile.h"
#include "Constants.h"
#include "Guard.h"
#include "Praetor.h"
#include "Player.h"
#include <vector>


class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	Player player;
	std::vector<Actor> m_Actors;
	std::vector<Projectile> m_Proj;
	std::vector<Guard> m_Guards;
	std::vector<Praetor> m_Praetors;
	std::vector<Vector2> m_Locations;
	Vector2 m_Location1;
	Vector2 m_Location2;
	Vector2 m_Location3;
	Vector2 m_Location4;
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	enum Phase {	// GAME PHASES
		EStart,		// Start game phase: Start screen, intro, etc.
		EEarly,		// Early game: Pawns only
		EMid,		// Mid game: Guards take over with not as many pawns
		ELate,		// Late game: Praetors (3?)
		EBoss,		// Boss phase: King who can spawn other enemies
		EEnd,		// End game
		EPause		// Pause game
	};
	Phase m_Phase;
	template<typename T>
	void AddActor(std::vector<T>& vec, float xPos, float yPos, float xDir, float yDir)
	{
		T obj;
		obj.SetPos(xPos, yPos);
		obj.m_Dir.x = xDir;
		obj.m_Dir.y = yDir;
		vec.push_back(obj);
	}
	template<typename T>
	void RemoveActor(std::vector<T>& vec, int index)
	{
		vec.erase(vec.begin() + index);
	}
	void AddProj(Projectile& proj);
	void RemoveProj(int index);
	void SpawnPawns();
	void SpawnGuardTrio();
	void SpawnGuardDuo();
	void SpawnPraetors();
	void ChangeNum(int& input, int floor, int ceiling);

	Timer m_GameStart;
	Timer m_CurrentTime;
	unsigned int m_EnmShootTimer;
	unsigned int m_EnmSpawnTimer;
	unsigned int m_Score;
	unsigned int m_EnmKilled;
	bool m_FlipSide;
	unsigned int m_GuardCooldown;
	unsigned int m_PawnsKilled;
	unsigned int m_GuardsKilled;
	unsigned int m_PraetorsKilled;
	bool m_GuardDuo;
	bool m_PraetorQuart;
	unsigned int m_RevCount;


	float playerTop;
	float playerBot;
	float playerLeft;
	float playerRight;
	float enemyTop;
	float enemyBot;
	float enemyLeft;
	float enemyRight;
	float BulletXPos;
	float BulletYPos;
	float EnemyXPos;
	float EnemyYPos;
	float BulletDirY;

	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	bool m_IsRunning;
	Uint32 m_TicksCount;

	TTF_Font * m_font;
	SDL_Surface * m_surface;
	SDL_Texture * m_texture;
	SDL_Rect m_TextScore;
	SDL_Rect m_TextScoreNum;

};
