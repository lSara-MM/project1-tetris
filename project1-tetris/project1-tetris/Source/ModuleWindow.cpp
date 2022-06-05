#include "ModuleWindow.h"

#include "ModuleInput.h"
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"


ModuleWindow::ModuleWindow(bool startEnabled) : Module(startEnabled)
{}

ModuleWindow::~ModuleWindow()
{}

bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;

		if (WIN_FULLSCREEN == true)
			flags |= SDL_WINDOW_FULLSCREEN;

		if (WIN_BORDERLESS == true)
			flags |= SDL_WINDOW_BORDERLESS;

		if (WIN_RESIZABLE == true)
			flags |= SDL_WINDOW_RESIZABLE;

		if (WIN_FULLSCREEN_DESKTOP == true)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow("Tetris 1988", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE, flags);

		if (window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 255, 255));		// Bg is basically always black
		}
	}

	return ret;
}

update_status ModuleWindow::PreUpdate()
{
	if (App->input->keys[SDL_SCANCODE_F] == KEY_DOWN)
	{
		App->FullScreenDesktop = !App->FullScreenDesktop;

		if (App->FullScreenDesktop)
		{
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else
		{
			SDL_SetWindowFullscreen(App->window->window, 0);
		}
	}
		return update_status::UPDATE_CONTINUE;

}

bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != nullptr)
		SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return true;
}




