#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
//#include "ModuleCollisions.h"

#include "ModuleFadeToBlack.h"
#include "ScreenCredits1.h"
#include "ScreenCredits2.h"
#include "ScreenStart.h"


#include "SDL/include/SDL_scancode.h"

#include <iostream>
using namespace std;
#include <sstream>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading ARMY");

	bool ret = true;
	//lachimolala = App->audio->LoadFx("Assets/Audio/buttah.wav");
	return ret;
}

update_status ModulePlayer::Update()
{
	if (App->input->keys[SDL_SCANCODE_B] == KEY_STATE::KEY_REPEAT &&
		App->input->keys[SDL_SCANCODE_T] == KEY_STATE::KEY_REPEAT &&
		App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	{
		App->audio->PauseMusic();
		App->audio->PlayFx(lachimolala, 0);	
	}
	else if (App->input->keys[SDL_SCANCODE_T] == KEY_STATE::KEY_UP)
		/*(App->input->keys[SDL_SCANCODE_B] == KEY_STATE::KEY_UP &&
			App->input->keys[SDL_SCANCODE_T] == KEY_STATE::KEY_UP &&
			App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_UP)*/

	{
		App->audio->ResumeMusic();
	}

	if (App->input->keys[SDL_SCANCODE_F5] == KEY_STATE::KEY_REPEAT)		// to change when have more levels
	{
		App->fade->FadeToBlack(App->sCredits1, (Module*)App->sLvl_1, 0);
		App->fade->FadeToBlack(App->sCredits2, (Module*)App->sLvl_1, 0);
		App->fade->FadeToBlack(App->sStart, (Module*)App->sLvl_1, 0);
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}