#include "Game.h"
#include <iostream>

Game::Game() :
	m_IsRunning(true),
	m_Window(nullptr),
	m_Renderer(nullptr),
	m_surface(nullptr),
	m_texture(nullptr),
	m_TicksCount(0),
	m_EnmShootTimer(35),
	m_EnmSpawnTimer(0),
	m_Score(0),
	m_EnmKilled(0),
	m_FlipSide(false),
	//m_Phase(EEarly),
	m_Phase(EStart),
	// Have m_GameStart record the start time when phase changes from EStart to EEarly
	m_GameStart(std::chrono::high_resolution_clock::now()),
	m_PawnsKilled(0),
	m_GuardsKilled(0),
	m_PraetorsKilled(0),
	m_GuardCooldown(40),
	m_GuardDuo(false),
	m_PraetorQuart(false),
	m_RevCount(0)
{
	m_Actors.reserve(20);
	m_Proj.reserve(150);
	player.SetPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
	player.SetDir(0.0f, 0.0f);
	m_Location1.x = SCREEN_WIDTH / 4;
	m_Location1.y = SCREEN_HEIGHT / 6;
	m_Location2.x = SCREEN_WIDTH * (3.0f / 4);
	m_Location2.y = SCREEN_HEIGHT / 6;
	m_Location3.x = SCREEN_WIDTH / 4;
	m_Location3.y = SCREEN_HEIGHT * .5;
	m_Location4.x = SCREEN_WIDTH * (3.0f / 4);
	m_Location4.y = SCREEN_HEIGHT * .5;
	m_Locations.push_back(m_Location1);
	m_Locations.push_back(m_Location2);
	m_Locations.push_back(m_Location3);
	m_Locations.push_back(m_Location4);
}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO); 
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	/*
	TTF_Init();
	if (TTF_Init() < 0) {
		SDL_Log("Unable to initialize TTF_Init: %s", TTF_GetError());
		return false;
	}
	*/
	// Create Window
	m_Window = SDL_CreateWindow(
		"Wyri",
		20, 
		200, 
		SCREEN_WIDTH, 
		SCREEN_HEIGHT, 
		0 
	);
	// Create Renderer
	m_Renderer = SDL_CreateRenderer(
		m_Window,												
		-1,														
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC	
	);
	if (!m_Window) { 
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	// Create TTF Text
	/*
	m_font = TTF_OpenFont("C:\\Dev\\Wyri\\Debug\\arial.ttf", 25);
	int texW = 100;
	int texH = 25;
	SDL_QueryTexture(m_texture, NULL, NULL, &texW, &texH);
	m_TextScore = { 0, 0, texW, texH };
	m_TextScoreNum = { texW + 10, texH };
	if (!m_font) {
		SDL_Log("Unable to initialize TTF: %s", TTF_GetError());
		std::cin.get();
		return false;
	}
	SDL_Color color = { 255, 255, 255 };
	m_surface = TTF_RenderText_Solid(m_font, "SCORE: ", color);
	m_texture = SDL_CreateTextureFromSurface(m_Renderer, m_surface);
	return true;
	*/
}

void Game::RunLoop() {
	while (m_IsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::Shutdown() {
	//TTF_CloseFont(m_font);
	//SDL_DestroyTexture(m_texture);
	//SDL_FreeSurface(m_surface);
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);
	//TTF_Quit();
	SDL_Quit();
}
void Game::ProcessInput() {
	SDL_Event evnt;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
	{
		Projectile proj(player.GetPos(), NONE, UP);
		AddProj(proj);
	}
	while (SDL_PollEvent(&evnt)) { 
		player.StopMotion();
		switch (evnt.type) {
		// Mouse
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		}
		// Keyboard
		
		if (state[SDL_SCANCODE_DOWN])
		{
			Projectile proj(player.GetPos(), NONE, UP);
			AddProj(proj);
		}
		if (state[SDL_SCANCODE_ESCAPE]) {
			m_IsRunning = false;
		}
		if (state[SDL_SCANCODE_W])
		{
			player.SetDir(0,-1);
		}
		if (state[SDL_SCANCODE_S])
		{
			player.SetDir(0, 1);
		}
		if (state[SDL_SCANCODE_A])
		{
			player.SetDir(-1, 0);
		}
		if (state[SDL_SCANCODE_D])
		{
			player.SetDir(1, 0);
		}
	}
}

void Game::UpdateGame() 
{
	//m_Phase = EEarly;
	
	// 1.Check position for collision, 2. Change Position, 3. Draw

	// Handle Tickrate
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16)) {}
	float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
	while (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	m_TicksCount = SDL_GetTicks();
	// Move Player
	player.UpdateActor(deltaTime, PLAYER_SPEED);
	if (player.GetPos().x <= 0)
	{
		player.SetPos(0, player.GetPos().y);
	}
	if (player.GetPos().x >= SCREEN_WIDTH - THICCNESS)
	{
		player.SetPos(SCREEN_WIDTH - THICCNESS, player.GetPos().y); 
	}
	if (player.GetPos().y <= 0)
	{
		player.SetPos(player.GetPos().x, 0);
	}
	if (player.GetPos().y >= SCREEN_HEIGHT - THICCNESS)
	{
		player.SetPos(player.GetPos().x, SCREEN_HEIGHT - THICCNESS); 
	}
	// Change Game Phase
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	auto duration = m_CurrentTime - m_GameStart;
	if (duration < 30s)
	{
		m_Phase = EEarly;
	}
	else if (duration >= 30s)
	{
		m_Phase = EMid;
	}
	else if (duration >= 60s)
	{
		m_Phase = ELate;
	}
	/*
	else if () // If praetors killed >= x
	{
		m_Phase = EBoss;
	}
	*/

	// EARLY GAME
	// Spawn new pawns
	if (m_Phase == EEarly)
	{
		SpawnPawns();
	}
	// MID GAME	
	if (m_Phase == EMid)
	{
		// Pawns
		SpawnPawns();
		// Guards
		m_GuardCooldown--;
		if (m_Guards.empty() && m_GuardsKilled == 0)
		{
			SpawnGuardTrio();
		}
		else if (m_Guards.empty() && m_GuardsKilled == 3)
		{
			SpawnGuardDuo();
			m_GuardDuo = true;
		}
		for (int i = 0; i < m_Guards.size(); i++)
		{
			m_Guards[i].UpdateActor(deltaTime, GUARD_SPEED);
		}
		if (!m_GuardCooldown)
		{
			for (int i = 0; i < m_Guards.size(); i++)
			{
				Projectile projLeft(m_Guards[i].GetPos(), LEFT, DOWN);
				Projectile projCenter(m_Guards[i].GetPos(), NONE, DOWN);
				Projectile projRight(m_Guards[i].GetPos(), RIGHT, DOWN);
				AddProj(projLeft);
				AddProj(projCenter);
				AddProj(projRight);
			}
			m_GuardCooldown = 60;
		}
		// Make a const int constants.h that stores that start positions, dont have to base the movement off of a specific actor
		if (!m_Guards.empty())
		{
			if (m_Guards[0].GetPos().y < 150.0f)
			{
				for (int i = 0; i < m_Guards.size(); i++)
				{					
					m_Guards[i].m_Dir.y = DOWN * -1;
				}
			}
			if (m_Guards[0].GetPos().y > 450.0f)
			{
				for (int i = 0; i < m_Guards.size(); i++)
				{
					m_Guards[i].m_Dir.y = UP * -1;
				}
			}
		}
		
		for (int i = 0; i < m_Guards.size(); i++)
		{
			if (m_Guards[i].GetPos().x <= THICCNESS/2 || m_Guards[i].GetPos().x >= SCREEN_WIDTH - THICCNESS/2)
			{
				m_Guards[i].m_Dir.x *= -1;
			}
		}
	}
	// LATE GAME
	int m_RevSpeed = 32;
	if (m_GuardsKilled == 5)
	{
		m_Phase = ELate;
	}
	if (m_Phase == ELate && !m_PraetorQuart)
	{
		SpawnPraetors();
		m_PraetorQuart = true;
	}
	for (int i = 0; i < m_Praetors.size(); i++)
	{
		m_Praetors[i].UpdateActor(deltaTime, PRAETOR_SPEED, m_Locations[i], PI / m_RevSpeed);
	}
	/*
	Oscillate speed by inc/dec m_RevSpeed
	if (m_RevSpeed < 128)
	{
		m_RevSpeed++;
	}
	*/
	


	// Move Enemies
	for (int i = 0; i < m_Actors.size(); i++)
	{
		// Simplify these if statements
		if (m_Actors[i].GetPos().x >= SCREEN_WIDTH - THICCNESS)
		{
			m_Actors[i].m_Dir.x = 0.0f;
			m_Actors[i].m_Dir.y = 1.0f;
			m_Actors[i].m_TimeOnWall++;
		}
		if (m_Actors[i].GetPos().x <= 0)
		{
			m_Actors[i].m_Dir.x = 0.0f;
			m_Actors[i].m_Dir.y = 1.0f;
			m_Actors[i].m_TimeOnWall++;
		}
		if (m_Actors[i].m_TimeOnWall > 20 && m_Actors[i].GetPos().x <= 0)
		{
			m_Actors[i].m_TimeOnWall = 0;
			m_Actors[i].m_Dir.y = 0.0f;
			m_Actors[i].m_Dir.x = 1.0f;
		}
		if (m_Actors[i].m_TimeOnWall > 20 && m_Actors[i].GetPos().x >= SCREEN_WIDTH - THICCNESS)
		{
			m_Actors[i].m_TimeOnWall = 0;
			m_Actors[i].m_Dir.y = 0.0f;
			m_Actors[i].m_Dir.x = -1.0f;
		}
		m_Actors[i].UpdateActor(deltaTime, PAWN_SPEED);
	}
	// Fire Enemy Projectiles
	m_EnmShootTimer--;
	if (!(m_EnmShootTimer))
	{
		for (int i = 0; i < m_Actors.size(); i++)
		{
			Projectile proj(m_Actors[i].GetPos(), NONE, DOWN);
			AddProj(proj);
		}
		m_EnmShootTimer = 35;
	}
	for (int i = 0; i < m_Praetors.size(); i++)
	{
		Projectile proj(m_Praetors[i].GetPos(), NONE, DOWN);
		if (m_Praetors[i].GetTheta() == 0 || (m_Praetors[i].GetTheta() >= 3.13f && m_Praetors[i].GetTheta() <= 3.15f))
		{
			AddProj(proj);
		}
	}
	// Move Projectiles
	for (int i = 0; i < m_Proj.size(); i++)
	{
		for (int i = 0; i < m_Proj.size(); i++)
		{
			if (m_Proj[i].GetPos().x <= THICCNESS / 2 || m_Proj[i].GetPos().x >= SCREEN_WIDTH - THICCNESS / 2)
			{
				m_Proj[i].m_Dir.x *= -1;
			}
		}
		if (m_Proj[i].GetPos().y < 0)
		{
			RemoveProj(i);
		}
		else if (m_Proj[i].GetPos().y > SCREEN_HEIGHT)
		{
			RemoveProj(i);
		}
		else 
		{
			m_Proj[i].UpdateProj(deltaTime);
		}
	}

	// COLLISION -----
	playerTop = player.GetPos().y - THICCNESS / 2;
	playerBot = player.GetPos().y + THICCNESS / 2;
	playerLeft = player.GetPos().x - THICCNESS / 2;
	playerRight = player.GetPos().x + THICCNESS / 2;
	// This is N^2 boi, could later just have a container of x/y enemy coordinates, see if current
	// projectile is at same position
	for (int i = 0; i < m_Proj.size(); i++)
	{
		BulletXPos = m_Proj[i].GetPos().x;
		BulletYPos = m_Proj[i].GetPos().y;
		BulletDirY = m_Proj[i].GetDir().y;
		// Projectile -> Enemy Collision
		for (int j = 0; j < m_Actors.size(); j++)
		{
			EnemyXPos = m_Actors[j].GetPos().x;
			EnemyYPos = m_Actors[j].GetPos().y;
			enemyTop = m_Actors[j].GetPos().y - THICCNESS / 2;
			enemyBot = m_Actors[j].GetPos().y + THICCNESS / 2;
			enemyLeft = m_Actors[j].GetPos().x - THICCNESS / 2;
			enemyRight = m_Actors[j].GetPos().x + THICCNESS / 2;
			if (!(BulletYPos < enemyTop || BulletYPos > enemyBot || BulletXPos < enemyLeft || BulletXPos > enemyRight)
				&& BulletDirY == UP)
			{
				std::cout << "HIT on enemy " << j << std::endl;
				m_Actors[j].TakeProjDamage(NORMAL_DAMAGE);
				if (m_Actors[j].GetHP() <= 0.0f)
				{
					RemoveActor(m_Actors, j);
					m_Score += 50;
				}
				RemoveProj(i);
			}
		}
		for (int j = 0; j < m_Guards.size(); j++)
		{
			EnemyXPos = m_Guards[j].GetPos().x;
			EnemyYPos = m_Guards[j].GetPos().y;
			enemyTop = m_Guards[j].GetPos().y - THICCNESS / 2;
			enemyBot = m_Guards[j].GetPos().y + THICCNESS / 2;
			enemyLeft = m_Guards[j].GetPos().x - THICCNESS / 2;
			enemyRight = m_Guards[j].GetPos().x + THICCNESS / 2;
			if (!(BulletYPos < enemyTop || BulletYPos > enemyBot || BulletXPos < enemyLeft || BulletXPos > enemyRight)
				&& BulletDirY == UP)
			{
				std::cout << "HIT on enemy " << j << std::endl;
				m_Guards[j].TakeProjDamage(NORMAL_DAMAGE);
				if (m_Guards[j].GetHP() <= 0.0f)
				{
					RemoveActor(m_Guards, j);
					m_Score += 200;
					m_GuardsKilled++;
				}
				RemoveProj(i);
			}
		}
		for (int j = 0; j < m_Praetors.size(); j++)
		{
			EnemyXPos = m_Praetors[j].GetPos().x;
			EnemyYPos = m_Praetors[j].GetPos().y;
			enemyTop = m_Praetors[j].GetPos().y - THICCNESS / 2;
			enemyBot = m_Praetors[j].GetPos().y + THICCNESS / 2;
			enemyLeft = m_Praetors[j].GetPos().x - THICCNESS / 2;
			enemyRight = m_Praetors[j].GetPos().x + THICCNESS / 2;
			if (!(BulletYPos < enemyTop || BulletYPos > enemyBot || BulletXPos < enemyLeft || BulletXPos > enemyRight)
				&& BulletDirY == UP)
			{
				std::cout << "HIT on enemy " << j << std::endl;
				m_Praetors[j].TakeProjDamage(NORMAL_DAMAGE);
				if (m_Praetors[j].GetHP() <= 0.0f)
				{
					RemoveActor(m_Praetors, j);
					m_Score += 500;
				}
				RemoveProj(i);
			}
		}
		// Enemy -> Player Collision
		for (int i = 0; i < m_Actors.size(); i++)
		{
			enemyTop = m_Actors[i].GetPos().y - THICCNESS / 2;
			enemyBot = m_Actors[i].GetPos().y + THICCNESS / 2;
			enemyLeft = m_Actors[i].GetPos().x - THICCNESS / 2;
			enemyRight = m_Actors[i].GetPos().x + THICCNESS / 2;
			if (!(playerTop > enemyBot || playerBot < enemyTop || playerRight < enemyLeft || playerLeft > enemyRight)) {
				std::cout << "Contacted Enemy" << std::endl;
				player.TakeProjDamage(20.0f);
				std::cout << "Player HP: " << player.GetHP() << std::endl;
			}
		}
		for (int i = 0; i < m_Guards.size(); i++)
		{
			enemyTop = m_Guards[i].GetPos().y - THICCNESS / 2;
			enemyBot = m_Guards[i].GetPos().y + THICCNESS / 2;
			enemyLeft = m_Guards[i].GetPos().x - THICCNESS / 2;
			enemyRight = m_Guards[i].GetPos().x + THICCNESS / 2;
			if (!(playerTop > enemyBot || playerBot < enemyTop || playerRight < enemyLeft || playerLeft > enemyRight)) {
				std::cout << "Contacted Enemy" << std::endl;
				player.TakeProjDamage(20.0f);
				std::cout << "Player HP: " << player.GetHP() << std::endl;
			}
		}
		for (int i = 0; i < m_Praetors.size(); i++)
		{
			enemyTop = m_Praetors[i].GetPos().y - THICCNESS / 2;
			enemyBot = m_Praetors[i].GetPos().y + THICCNESS / 2;
			enemyLeft = m_Praetors[i].GetPos().x - THICCNESS / 2;
			enemyRight = m_Praetors[i].GetPos().x + THICCNESS / 2;
			if (!(playerTop > enemyBot || playerBot < enemyTop || playerRight < enemyLeft || playerLeft > enemyRight)) {
				std::cout << "Contacted Enemy" << std::endl;
				player.TakeProjDamage(20.0f);
				std::cout << "Player HP: " << player.GetHP() << std::endl;
			}
		}
	}
	// Projectile - > Player Collision
	for (int i = 0; i < m_Proj.size(); i++)
	{
		BulletXPos = m_Proj[i].GetPos().x;
		BulletYPos = m_Proj[i].GetPos().y;
		BulletDirY = m_Proj[i].GetDir().y;
		if (!(BulletYPos < playerTop || BulletYPos > playerBot || BulletXPos < playerLeft || BulletXPos > playerRight)
			&& BulletDirY == DOWN)
		{
			std::cout << "DAMAGE from enemy " << std::endl;
			RemoveProj(i);
		}
	}
}

void Game::GenerateOutput() {
	// Background
	SDL_SetRenderDrawColor(m_Renderer, 75, 99, 94, 255);
	SDL_RenderClear(m_Renderer);	
	SDL_SetRenderDrawColor(m_Renderer, 160, 213, 229, 255);
	// Text
	//SDL_RenderCopy(m_Renderer, m_texture, NULL, &m_TextScore);
	//SDL_RenderCopy(m_Renderer, m_texture, NULL, &m_TextScoreNum);

	// Player
	SDL_Rect thePlayer
	{
		player.GetPos().x,
		player.GetPos().y,
		THICCNESS,
		THICCNESS
	};
	SDL_RenderFillRect(m_Renderer, &thePlayer);
	// Pawns
	SDL_SetRenderDrawColor(m_Renderer, 255, 119, 235, 255);
	for (int i = 0; i < m_Actors.size(); i++)
	{
		SDL_Rect thePawns
		{
			m_Actors[i].GetPos().x,
			m_Actors[i].GetPos().y,
			THICCNESS,
			THICCNESS
		};
		SDL_RenderFillRect(m_Renderer, &thePawns);
	}
	// Guards
	SDL_SetRenderDrawColor(m_Renderer, 226, 180, 86, 255);
	for (int i = 0; i < m_Guards.size(); i++)
	{
		SDL_Rect theGuards
		{
			m_Guards[i].GetPos().x,
			m_Guards[i].GetPos().y,
			THICCNESS,
			THICCNESS
		};
		SDL_RenderFillRect(m_Renderer, &theGuards);
	}
	// Praetors
	SDL_SetRenderDrawColor(m_Renderer, 225, 255, 76, 255);
	for (int i = 0; i < m_Praetors.size(); i++)
	{
		SDL_Rect thePraetors
		{
			m_Praetors[i].GetPos().x,
			m_Praetors[i].GetPos().y,
			THICCNESS,
			THICCNESS
		};
		SDL_RenderFillRect(m_Renderer, &thePraetors);
	}
	// Projectiles
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
	for (int i = 0; i < m_Proj.size(); i++)
	{
		SDL_Rect Bullet
		{
			m_Proj[i].GetPos().x,
			m_Proj[i].GetPos().y,
			THICCNESS/2,
			THICCNESS/2
		};
		SDL_RenderFillRect(m_Renderer, &Bullet);
	}
	// Swap front and back buffers 
	SDL_RenderPresent(m_Renderer);  
}

// Other Game Functions
void Game::AddProj(Projectile& proj)
{
	m_Proj.push_back(proj);
}
void Game::RemoveProj(int index)
{
	m_Proj.erase(m_Proj.begin() + index);
}
void Game::SpawnGuardTrio()
{
	AddActor<Guard>(m_Guards, SCREEN_WIDTH/6.0f, 50.0f, 0.0f, 1.0f);
	AddActor<Guard>(m_Guards, SCREEN_WIDTH*.5f, 150.0f, 0.0f, 1.0f);
	AddActor<Guard>(m_Guards, SCREEN_WIDTH*(5.0f/6.0f), 50.0f, 0.0f, 1.0f);
}
void Game::SpawnGuardDuo()
{
	AddActor<Guard>(m_Guards, SCREEN_WIDTH / 3.0f, 50.0f, -1.0f, 1.0f);
	AddActor<Guard>(m_Guards, SCREEN_WIDTH*(2.0f/3.0f), 50.0f, 1.0f, 1.0f);
}
void Game::SpawnPawns()
{
	if (m_EnmSpawnTimer)
	{
		m_EnmSpawnTimer--;
	}
	if (!m_EnmSpawnTimer)
	{
		switch (m_FlipSide)
		{
		case(false):
			AddActor<Actor>(m_Actors, 120.0f, 200.0f, 1.0f, 0.0f);
			m_FlipSide = true;
			break;

		case(true):
			AddActor<Actor>(m_Actors, SCREEN_WIDTH - 120.0f, 200.0f, -1.0f, 0.0f);
			m_FlipSide = false;
			break;
		}
		m_EnmSpawnTimer = 70;
	}
}
void Game::SpawnPraetors()
{
	for (int i = 0; i < 4; i++)
	{
		AddActor<Praetor>(m_Praetors, m_Locations[i].x + 50.0f, m_Locations[i].y + 50.0f, NULL, NULL); // Change to m_Radius rather than magic number
	}
}

void Game::ChangeNum(int& input, int floor, int ceiling)
{
	int increment = 1;
	bool increase = true;
	if (increase && input < ceiling)
	{
		input += increment;
	}
	if (input == ceiling)
	{
		increase = false;
	}
	if (!increase && input > floor)
	{
		input -= increment;
	}
	if (input == floor)
	{
		increase = true;
	}
	
}
