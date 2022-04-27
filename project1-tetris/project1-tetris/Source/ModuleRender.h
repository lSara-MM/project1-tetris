#ifndef __MODULE_RENDER_H__
#define __MODULE_RENDER_H__

#include "Module.h"
#include "SDL/include/SDL_Rect.h"
#include "SDL2_ttf-2.0.18/include/SDL_ttf.h"

#define MAX_ACTIVE_TEXT 100

// Modify the directories in the SDL_ttf.h to the corresponding ones
// Properties >> Linker >> Input >> Additional dependencies >> add "SDL2_ttf.lib"

#include <iostream>
using namespace std;

struct SDL_Texture;
struct SDL_Renderer;

class ModuleRender : public Module
{
public:
	//Constructor
	ModuleRender(bool startEnabled);

	//Destructor
	~ModuleRender();

	// Called on application start
	// Creates the rendering context using the program's window
	bool Init() override;

	// Called at the beginning of the application loop
	// Clears the rendering context to a background color
	update_status PreUpdate() override;

	// Called at the middle of the application loop
	// Handles camera movement
	update_status Update() override;

	// Called at the end of the application loop
	// Displays a rectangle in the rendering context
	// Updates the screen with the rendered content
	update_status PostUpdate() override;

	// Called on application exit
	// Destroys the rendering context
	bool CleanUp() override;

	// Prints a texture to the rendering context
	// Param texture	- A valid SDL Texture, validation checks are not performed
	// Param x,y		- Position x,y in the screen (upper left axis)
	// Param section	- The portion of the texture we want to copy. nullptr for the entire texture
	// Param speed		- The amount of effect that is applied to the sprite depending on the camera
	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = nullptr, float speed = 1.0f);

	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, float speed = 1.0f);

	// Render text
	bool TextDraw(const char* words, int x, int y, int red, int green, int blue, int alpha, int size);

	// ttf cleanup
	void ttfQuit();

public:
	// Rendering context used for any rendering action
	SDL_Renderer* renderer = nullptr;

	// Rendering text
	SDL_Surface* ttf_surface = nullptr;
	SDL_Texture* ttf_texture = nullptr;
	TTF_Font* ttf_font = nullptr;

	// A rectangle that represents the camera section
	// Sprites will be rendered to the screen depending on the camera position
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// The speed at which the camera will be moving
	int cameraSpeed = 0;


	//ttf_font* txt[MAX_ACTIVE_TEXT] = { nullptr };
};

#endif //__MODULE_RENDER_H__
