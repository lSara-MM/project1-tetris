#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "SDL/include/SDL.h"

#define NUM_MODULES 22

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ModuleParticles;
class ModuleCollisions;
class ModuleFadeToBlack;
class ModuleRender;

class ModulePlayer;
class ModuleTetronimo;
class Points;


//screens
class ScreenCredits1;
class ScreenCredits2;
class ScreenStart;
class ScreenDiffSelect;
class ScreenLvl_1;
class ScreenLvl_2;
class ScreenLvl_3;
class ScreenLvl_4;
class ScreenLvl_5;
class ScreenLvl_6;
class ScreenLvl_7;
class ScreenLvl_10;

class Application
{

public:

	//Constructor. Creates all necessary modules for the application
	Application();

	//Destructor. Removes all module objects
	~Application();

	//Initializes all modules
	bool Init();

	//Updates all modules (PreUpdate, Update and PostUpdate)
	update_status Update();

	//Releases all the application data
	bool CleanUp();

public:
	// An array to store all modules
	Module* modules[NUM_MODULES];

	// All the modules stored individually
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;

	ModulePlayer* player = nullptr;
	ModuleTetronimo* tetronimo = nullptr;
	Points* points = nullptr;


	ScreenCredits1* sCredits1 = nullptr;
	ScreenCredits2* sCredits2 = nullptr;
	ScreenStart* sStart = nullptr;
	ScreenDiffSelect* sDiff = nullptr;
	ScreenLvl_1* sLvl_1 = nullptr;
	ScreenLvl_2* sLvl_2 = nullptr;
	ScreenLvl_3* sLvl_3 = nullptr;
	ScreenLvl_4* sLvl_4 = nullptr;
	ScreenLvl_5* sLvl_5 = nullptr;
	ScreenLvl_6* sLvl_6 = nullptr;
	ScreenLvl_7* sLvl_7 = nullptr; 
	ScreenLvl_10* sLvl_10 = nullptr;

	ModuleParticles* particles = nullptr;

	ModuleCollisions* collisions = nullptr;
	ModuleFadeToBlack *fade = nullptr;

	ModuleRender* render = nullptr;
};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__