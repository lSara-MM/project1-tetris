#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput(bool startEnabled) : Module(startEnabled)
{

}

ModuleInput::~ModuleInput()
{}

bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) return update_status::UPDATE_STOP;
	}
	//Lee los datos del teclado i actualiza el custom array
	SDL_PumpEvents();

	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}


	return update_status::UPDATE_CONTINUE;
}
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");

	// Stop rumble from all gamepads and deactivate SDL functionallity

	return true;
}
