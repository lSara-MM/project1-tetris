#ifndef __SCREENCREDITS1_H__
#define __SCREENCREDITS1_H__

#include "Module.h"
#include "Animation.h"
#include "ModuleWindow.h"

struct SDL_Texture;

class ScreenCredits1 : public Module
{
public:
	//Constructor
	ScreenCredits1(bool startEnabled);

	//Destructor
	~ScreenCredits1();

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

	// Start Scene animation
	Animation start_screen;

	// Credit (coins to add 0-9)
	int credits = 0;
	uint fxAdd_Credits = 0;



};

#endif //__SCREENCREDITS1_H__
