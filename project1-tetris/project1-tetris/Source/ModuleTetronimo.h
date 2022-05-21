#ifndef __MODULE_TETRONIMO_H__
#define __MODULE_TETRONIMO_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

#include "SDL/include/SDL_Rect.h"
#include "SDL2_ttf-2.0.18/include/SDL_ttf.h"

//#define MAX_TETRONIMOS 50
//#define T_WIDTH 15
//#define T_HEIGHT 15


#define MAX_B 204
#define B_WIDTH 15
#define B_HEIGHT 15

enum BLOCK_TYPE
{
	NONE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	PINK,
	CYAN,
	ORANGE,

};		// to spawn 

class ModuleTetronimo : public Module
{
public:

	ModuleTetronimo(bool startEnabled);
	~ModuleTetronimo();

	bool Start() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	class Block
	{
	public:

	public:
		int id;
		int bX, bY;		// x, y in screen
		int tileX, tileY;		// tile position

		SDL_Rect section;	// { spritesheet coords, B_WIDTH, B_HEIGHT }

	private:
		SDL_Texture* blockTexture = nullptr;
	};

	int spawnTetronimo(int next);
	void blockUpdate(Block *block);
	bool blockFall();
	bool lineCheck();
	void Debugging();
	

public:
	
	int nextT;
	//int num;
	int combo;

	////int
	//int tileSetInt[22][10];
	//int bX, bY;
	//SDL_Rect section;	// { spritesheet coords, B_WIDTH, B_HEIGHT }


	//class
	Block tileSet[22][10];
	Block* b1, *b2, *b3, *b4;

private:
	SDL_Texture* grid_texture = nullptr;
	SDL_Texture* blockTexture = nullptr;
};

#endif //__MODULE_TETRONIMO_H__