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
	else
	{
		App->audio->ResumeMusic();
	}
	

	// Debugging modes
	if (App->input->keys[SDL_SCANCODE_F1] == KEY_STATE::KEY_DOWN) {
		godMode != godMode;
		LOG("God mode");
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}
