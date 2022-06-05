#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_KEYS; ++i) keys[i] = KEY_IDLE;

	/*memset(&pads[0], 0, sizeof(GamePad) * MAX_PADS)*/;
}

ModuleInput::~ModuleInput()
{}

bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) < 0)
	{

		LOG("SDL_EVENTS could not initialize SDL_Error: %s\n", SDL_GetError());
		ret = false; 
	}

	num_controllers = SDL_NumJoysticks();
	for (int i = 0; i < num_controllers; ++i)
		if (SDL_IsGameController(i))
			sdl_controllers[i] = SDL_GameControllerOpen(i);


	return ret;
}

update_status ModuleInput::PreUpdate()
{
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);


	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return update_status::UPDATE_STOP;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				return update_status::UPDATE_STOP;

			}
			break;
		}
	}
	//Lee los datos del teclado i actualiza el custom array
	SDL_PumpEvents();

	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	SDL_GameControllerUpdate();

	for (int i = 0; i < num_controllers; ++i) {

		for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j) {

			if (SDL_GameControllerGetButton(sdl_controllers[i], (SDL_GameControllerButton)j))
				pads[i].buttons[j] = (pads[i].buttons[j] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
			else
				pads[i].buttons[j] = (pads[i].buttons[j] == KEY_REPEAT || pads[i].buttons[j] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
		}

		pads[i].left_x = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_LEFTX);
		pads[i].left_y = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_LEFTY);
		pads[i].right_x = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_RIGHTX);
		pads[i].right_y = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_RIGHTY);
		pads[i].right_dz = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		pads[i].left_dz = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	}

	return update_status::UPDATE_CONTINUE;
}
	

bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");

	// Stop rumble from all gamepads and deactivate SDL functionallity
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

/*GAMEPAD RELATED*/
//void ModuleInput::HandleDeviceConnection(int index)
//{
//	if (SDL_IsGameController(index))
//	{
//		for (int i = 0; i < MAX_PADS; ++i)
//		{
//			GamePad& pad = pads[i];
//
//			if (pad.enabled == false)
//			{
//				if (pad.controller = SDL_GameControllerOpen(index))
//				{
//					LOG("Found a gamepad with id %i named %s", i, SDL_GameControllerName(pad.controller));
//					pad.enabled = true;
//					pad.left_dz = pad.right_dz = 0.1f;
//					if (pad.haptic != nullptr) LOG("... gamepad has force feedback capabilities");
//					pad.index = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad.controller));
//				}
//			}
//		}
//	}
//}
//
//void ModuleInput::HandleDeviceRemoval(int index)
//{
//	// If an existing gamepad has the given index, deactivate all SDL device functionallity
//	for (int i = 0; i < MAX_PADS; ++i)
//	{
//		GamePad& pad = pads[i];
//		if (pad.enabled && pad.index == index)
//		{
//			SDL_GameControllerClose(pad.controller);
//			memset(&pad, 0, sizeof(GamePad));
//		}
//	}
//}
//
//void ModuleInput::UpdateGamepadsInput()
//{
//	// Iterate through all active gamepads and update all input data
//	for (int i = 0; i < MAX_PADS; ++i)
//	{
//		GamePad& pad = pads[i];
//
//		if (pad.enabled == true)
//		{
//			pad.a = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_A) == 1;
//			pad.left = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1;
//			pad.right = SDL_GameControllerGetButton(pad.controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1;
//
//			pad.left_x = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_LEFTX)) / 32767.0f;
//			pad.left_y = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_LEFTY)) / 32767.0f;
//			pad.right_x = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_RIGHTX)) / 32767.0f;
//			pad.right_y = float(SDL_GameControllerGetAxis(pad.controller, SDL_CONTROLLER_AXIS_RIGHTY)) / 32767.0f;
//
//			// Apply deadzone. All values below the deadzone will be discarded
//			pad.left_x = (fabsf(pad.left_x) > pad.left_dz) ? pad.left_x : 0.0f;
//			pad.left_y = (fabsf(pad.left_y) > pad.left_dz) ? pad.left_y : 0.0f;
//			pad.right_x = (fabsf(pad.right_x) > pad.right_dz) ? pad.right_x : 0.0f;
//			pad.right_y = (fabsf(pad.right_y) > pad.right_dz) ? pad.right_y : 0.0f;
//
//		}
//	}
//}
