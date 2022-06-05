#ifndef __SCREENLVL1_H__
#define __SCREENLVL1_H__

#include "Module.h"
#include "Animation.h"
#include "Application.h"


#include "ScreenStart.h"

struct SDL_Texture;

class ScreenLvl_1 : public Module
{
public:
	//Constructor
	ScreenLvl_1(bool startEnabled);

	//Destructor
	~ScreenLvl_1();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate() override;

	int ColourRandom();

	// Intento de solucion a leaks
	bool CleanUp() override;

	// Lvl conditions
	void lvl_win();
	void lvl_lose(const char* ch_loseContinue);

public:

	class Low 
	{
	public:
		Animation OnelowBonus;

		int y;
	};

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bg_texture = nullptr;

	// Music and fx
	uint fxBlock_Fall = 0;
	uint fxLine = 0;
	uint fxLvl = 0;

	uint fxYou_DidIt = 0;
	uint fxClearing_Bars = 0;
	uint fxGameOver = 0;
	uint fxruso3 = 0;
	uint fxruso6 = 0;
	uint fxBonus = 0;


	// Curtain texture and animation pointer
	SDL_Texture* curtain_texture = nullptr;
	SDL_Texture* lowBonus_texture = nullptr; 

	Animation openCurtain;
	Animation closeCurtain; 

	Low LowBonus[22];

	// Dancing rus texture
	SDL_Texture* ruso_texture3 = nullptr;
	SDL_Texture* ruso_texture6 = nullptr;
	SDL_Texture* ruso_texture9 = nullptr;
	Animation dancingRus3;
	Animation dancingRus6;
	Animation dancingRus9;

	bool lvl_instaWin;
	bool lvl_instaLose;

	//int lines;
	int linesleft;
	int linesObjective;

	int value;
	int scr;
	int Bonus;

	// counters / timers
	bool v_fall;

	int v_insertCoin;
	int v_points;
	int v_message;

	int v_loseContinue;
	int v_WinLose;


	SDL_Rect p_pos;

	int colour;


	SDL_Rect p_pos2;

	int pos_x = 55;
	int pos_y = 110;

	int pos_x1 = 55;
	int pos_y1 = 170;

	int pos_x2 = 55;
	int pos_y2 = 220;

	int pos_x3 = 55;
	int pos_y3 = 270;

	int pos_x4 = 55;
	int pos_y4 = 320;

	int pos_x5 = 55;
	int pos_y5 = 370;

	SDL_Texture* green_rect_texture = nullptr;
	SDL_Texture* red_rect_texture = nullptr;
	SDL_Texture* yellow_rect_texture = nullptr;
	SDL_Texture* orange_rect_texture = nullptr;
	SDL_Texture* white_rect_texture = nullptr;
	SDL_Texture* blue_rect_texture = nullptr;
	SDL_Texture* bluedark_rect_texture = nullptr;
	SDL_Texture* pink_rect_texture = nullptr;
};

#endif
