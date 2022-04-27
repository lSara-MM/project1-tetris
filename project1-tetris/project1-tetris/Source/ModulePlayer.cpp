#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

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
	lachimolala = App->audio->LoadFx("Assets/Audio/buttah.wav");
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
	else if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_UP)
		/*(App->input->keys[SDL_SCANCODE_B] == KEY_STATE::KEY_UP &&
			App->input->keys[SDL_SCANCODE_T] == KEY_STATE::KEY_UP &&
			App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_UP)*/

	{
		App->audio->ResumeMusic();
	}
	return update_status::UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}