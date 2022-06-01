#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"

#include "ModulePlayer.h"
#include "ModuleTetronimo.h"
#include "Points.h"

// Screens
#include "ScreenCredits1.h"
#include "ScreenCredits2.h"
#include "ScreenStart.h"
#include "ScreenDiffSelect.h"
#include "ScreenLvl_1.h"
#include "ScreenLvl_2.h"
#include "ScreenLvl_3.h"
#include "ScreenLvl_4.h"
#include "ScreenLvl_5.h"
#include "ScreenLvl_6.h"
#include "ScreenLvl_7.h"
#include "ScreenLvl_10.h"


Application::Application()
{
	// The order in which the modules are added is very important.
	// It will define the order in which Pre/Update/Post will be called
	// Render should always be last, as our last action should be updating the screen

	modules[0] = window = new ModuleWindow(true);
	modules[1] = input = new ModuleInput(true);
	modules[2] = textures = new ModuleTextures(true);
	modules[3] = audio = new ModuleAudio(true);

	modules[4] = sCredits1 = new ScreenCredits1(false);
	modules[5] = sCredits2 = new ScreenCredits2(false);
	modules[6] = sStart = new ScreenStart(false);
	modules[7] = sDiff = new ScreenDiffSelect(false);
	modules[8] = sLvl_1 = new ScreenLvl_1(true);
	modules[9] = sLvl_2 = new ScreenLvl_2(false);//Gameplay scene starts disabled
	modules[10] = sLvl_3 = new ScreenLvl_3(false);
	modules[11] = sLvl_4 = new ScreenLvl_4(false);
	modules[12] = sLvl_5 = new ScreenLvl_5(false);
	modules[13] = sLvl_6 = new ScreenLvl_6(false);
	modules[14] = sLvl_7 = new ScreenLvl_7(false);
	modules[15] = sLvl_10 = new ScreenLvl_10(false);


	modules[16] = player = new ModulePlayer(true);	
	modules[17] = tetronimo = new ModuleTetronimo(false);	//Player starts disabled
	modules[18] = points = new Points(false);

	modules[19] = particles = new ModuleParticles(true);
	modules[20] = fade = new ModuleFadeToBlack(true);

	modules[21] = render = new ModuleRender(true);
}

Application::~Application()
{
	for (int i = 0; i < NUM_MODULES; ++i)
	{
		//Important: when deleting a pointer, set it to nullptr afterwards
		//It allows us for null check in other parts of the code
		delete modules[i];
		modules[i] = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	// All modules (active and disabled) will be initialized
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->Init();

	// Only active modules will be 'started'
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;

	return ret;
}

update_status Application::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : update_status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : update_status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : update_status::UPDATE_CONTINUE;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret; --i)
		ret = modules[i]->IsEnabled() ? modules[i]->CleanUp() : true;

	return ret;
}
