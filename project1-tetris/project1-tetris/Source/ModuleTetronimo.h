#ifndef __MODULE_TETRONIMO_H__
#define __MODULE_TETRONIMO_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

#define MAX_TETRONIMOS 50

struct SDL_Texture;

enum class TETRONIMO_TYPE {

	NO_TYPE,
	RED,
	YELLOW,
	PINK,
	ORANGE,
	BLUE,
	SKY,
	P,	// player block
	B,	// static block
	W,	// wall
	G,	// ground
	N,	// null
};

class ModuleTetronimo : public Module
{
public:

	ModuleTetronimo(bool startEnabled);

	~ModuleTetronimo();

	bool Start() override;

	update_status Update() override;

	update_status PostUpdate() override;

	bool CleanUp() override;

	bool lineCompleted(int n, int f, int s);

	int blockRB();

	void SpawnTetronimo();

	void Print();

	void Change();

	void Spawn();

	void Debugging();
	//bool CleanUp() override; 

public:

	int arr[10][22];
	char type[10][22];

	iPoint position;

	int speed = 1;

	Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation turnAnim;
	Animation turn2Anim;
	Animation turn3Anim;

	int combo;

	// Debugging
	//bool lvl_continue = true;

	bool lvl_instaLose = false;
	bool lvl_instaWin = false;

	int v_WinLose;
	int v_loseContinue;
private:

	SDL_Texture* texture = nullptr;
};

#endif //__MODULE_TETRONIMO_H__