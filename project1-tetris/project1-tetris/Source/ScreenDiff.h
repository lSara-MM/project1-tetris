#ifndef __ScreenDiff_H__
#define __ScreenDiff_H__

#include "Module.h"
#include "Animation.h"
#include "ModuleWindow.h"

struct SDL_Texture;

class ScreenDiff : public Module
{
public:
	//Constructor
	ScreenDiff(bool startEnabled);

	//Destructor
	~ScreenDiff();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate() override;

public:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bg_texture = nullptr;

	SDL_Texture* green1_texture = nullptr;
	SDL_Texture* pink1_texture = nullptr;
	SDL_Texture* sky1_texture = nullptr;

	SDL_Texture* green_block_texture = nullptr;
	SDL_Texture* pink_block_texture = nullptr;
	SDL_Texture* sky_block_texture = nullptr;
	SDL_Texture* orange_block_texture = nullptr;
	SDL_Texture* yellow_block_texture = nullptr;

	SDL_Texture* yellow4_texture = nullptr;
	SDL_Texture* orange2_texture = nullptr;
	SDL_Texture* gray_block_texture = nullptr;
	SDL_Texture* pink3_texture = nullptr;

	SDL_Texture* arrowleft_texture = nullptr;
	SDL_Texture* arrowright_texture = nullptr;


	// Start Scene animation

	// Credit (coins to add 0-9)
	int press = 0;
	uint fxAdd_Press_L_R = 0;

	int enter = 0;
	uint fxAdd_PressEnter = 0;


};

#endif
