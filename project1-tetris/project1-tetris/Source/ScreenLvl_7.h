#ifndef __SCREENLVL7_H__
#define __SCREENLVL7_H__

#include "Module.h"
#include "Animation.h"
#include "Application.h"


#include "ScreenStart.h"

struct SDL_Texture;

class ScreenLvl_7 : public Module
{
public:
	//Constructor
	ScreenLvl_7(bool startEnabled);

	//Destructor
	~ScreenLvl_7();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate() override;

	// Intento de solucion a leaks
	bool CleanUp() override;

	// Lvl conditions
	void lvl_win();
	void lvl_lose(const char* ch_loseContinue);

public:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bg_texture = nullptr;

	// Music and fx
	uint fxBlock_Fall = 0;
	uint fxLine = 0;
	uint fxLvl = 0;

	uint fxYou_DidIt = 0;
	uint fxClearing_Bars = 0;
	uint fxGameOver = 0;


	// Curtain texture and animation pointer
	SDL_Texture* curtain_texture = nullptr;

	Animation openCurtain;
	Animation closeCurtain;

	// Dancing rus texture
	SDL_Texture* ruso_texture3 = nullptr;
	SDL_Texture* ruso_texture6 = nullptr;
	SDL_Texture* ruso_texture9 = nullptr;
	Animation dancingRus3;
	Animation dancingRus6;
	Animation dancingRus9;

	bool lvl_instaWin;
	bool lvl_instaLose;

	int lines;
	int linesleft;
	int linesObjective;

	int value;

	// counters / timers
	bool v_stack;

	int v_insertCoin;
	int v_points;
	int v_message;

	int v_loseContinue;
	int v_WinLose;
};

#endif
