#include "ScreenDiff.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleParticles.h"

#include <iostream>
using namespace std;
#include <sstream>


ScreenDiff::ScreenDiff(bool startEnabled) : Module(startEnabled)
{

}

ScreenDiff::~ScreenDiff()
{

}

// Load assets
bool ScreenDiff::Start()
{
	LOG("Loading background assets");
	bool ret = true;

	bg_texture = App->textures->Load("Assets/ss_difficultyBg.png");

	return ret;
}

update_status ScreenDiff::Update()
{
	start_screen.Update();

	// If easy mode selected change screen
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
	}
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenDiff::PostUpdate()
{

	App->render->Blit(bg_texture, 0, 0, NULL);

	return update_status::UPDATE_CONTINUE;
}
