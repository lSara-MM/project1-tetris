#ifndef __MODULE_TETRONIMO_H__
#define __MODULE_TETRONIMO_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

//#include "Tetronimo.h"

#include "SDL/include/SDL_Rect.h"
#include "SDL2_ttf-2.0.18/include/SDL_ttf.h"

#define MAX_TETRONIMOS 50
#define T_WIDTH 15
#define T_HEIGHT 15

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
	void Change();
	void Spawn();
	void AddTetronimo(const SDL_Rect& tetronimo, const int& r, const int& g, const int& b);

	void Debugging();
	void Print();

	//bool CleanUp() override; 

	//void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
	//{
	//	//Holds offsets
	//	SDL_Rect offset;

	//	//Get offsets
	//	offset.x = x;
	//	offset.y = y;

	//	//Blit
	//	SDL_BlitSurface(source, clip, destination, &offset);
	//}

public:

	int arr[10][22];
	char type[10][22];

	iPoint position;

	int speed = 1;
	int move = 0;
	int next = 0;

	SDL_Rect rectIdle;
	SDL_Rect rectIdle1;
	SDL_Rect rectIdle2;
	SDL_Rect rectIdle3;

	int r = 0;
	int g = 0;
	int b = 0;

	int red[10][22];
	int green[10][22];
	int blue[10][22];

	// animations
	/*Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation turnAnim;
	Animation turn2Anim;
	Animation turn3Anim;*/

	int combo;

	// Debugging
	bool lvl_instaLose;
	bool lvl_instaWin;


	// la sucia
	void lvl_win();
	void lvl_lose(const char* ch_loseContinue);

	int v_loseContinue;
	int v_WinLose;

private:
	SDL_Texture* texture = nullptr;
	SDL_Texture* grid_texture = nullptr;

	SDL_Texture* img_tetronimo = nullptr;
	SDL_Rect tetronimos[10][22];
};

#endif //__MODULE_TETRONIMO_H__