#ifndef __SCREENDIFFSELECT_H__
#define __SCREENDIFFSELECT_H__

#include "Module.h"
#include "Animation.h"
#include "ModuleWindow.h"
#include "p2Point.h"

struct SDL_Texture;
struct SDL_GetKeyState;
struct SDL_SetTimer;

class ScreenDiffSelect : public Module
{
public:
	//Constructor
	ScreenDiffSelect(bool startEnabled);

	//Destructor
	~ScreenDiffSelect();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update() override;

	int ColourRandom();
	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate() override;

	//Leaks
	bool CleanUp() override;

public:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bg_texture = nullptr;

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* arrowleft_texture = nullptr;
	SDL_Texture* arrowright_texture = nullptr;

	SDL_Rect p_pos;

	int p_x = 64;
	int p_y = 64;

	SDL_Rect p_pos2;

	int p2_x = 144;
	int p2_y = 64;

	//Colour Random
	int pos_x = 22;
	int pos_y = 130;

	int pos_x1 = 22;
	int pos_y1 = 244;

	int pos_x2 = 22;
	int pos_y2 = 365;

	int pos_x3 = 200;
	int pos_y3 = 244;

	int pos_x4 = 200;
	int pos_y4 = 365;

	int pos_x5 = 200;
	int pos_y5 = 130;


	int colour;

	const int Easy = 2;
	int Index = 2;

	SDL_Texture* green_rect_texture = nullptr;
	SDL_Texture* red_rect_texture = nullptr;
	SDL_Texture* yellow_rect_texture = nullptr;
	SDL_Texture* orange_rect_texture = nullptr;
	SDL_Texture* white_rect_texture = nullptr;
	SDL_Texture* blue_rect_texture = nullptr;
	SDL_Texture* bluedark_rect_texture = nullptr;
	SDL_Texture* pink_rect_texture = nullptr;

	int press = 0;
	uint fxAdd_Press_L_R = 0;

	int enter = 0;
	uint fxAdd_PressEnter = 0;

};

#endif
