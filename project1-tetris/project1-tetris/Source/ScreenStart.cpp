#include "ScreenStart.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

#include "ModuleParticles.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

#include <iostream>
using namespace std;
#include <sstream>

ScreenStart::ScreenStart(bool startEnabled) : Module(startEnabled)
{
	// Start Screen
	for (int i = 0; i < 11; i++)
	{
		start_screen.PushBack({ SCREEN_WIDTH * i, 1, SCREEN_WIDTH, SCREEN_HEIGHT });
	}

	start_screen.loop = false;
	start_screen.speed = 0.09f;
}

ScreenStart::~ScreenStart()
{

}

// Load assets
bool ScreenStart::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	bg_texture = App->textures->Load("Assets/ss_startBg.png");
	fxAdd_Credits = App->audio->LoadFx("Assets/Audio/FX/add_credit.wav");

	return ret;
}

update_status ScreenStart::Update()
{

	start_screen.Update();
	
	App->render->camera.x += 0;

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		if (credits < 9) { credits++; }
		LOG("credits: %d", credits);
		App->audio->PlayFx(fxAdd_Credits);
	}

	if ( credits > 0 && App->input->keys[SDL_SCANCODE_RETURN] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sDiff, 0);
		App->particles->Disable();
	}

	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenStart::PostUpdate()
{
	ModuleParticles fireworks(true);
	fireworks.FwTiming(start_screen.GetCurrentFrame().x);

	App->render->Blit(bg_texture, 0, 0, &(start_screen.GetCurrentFrame()), 0.3f);

	if (credits == 1)
	{
		// Rect, r, g, b, alpha (0-255) "opacity"
		App->render->DrawQuad({ 160, 0, 352, 48 }, 255, 0, 0, 255);
		App->render->DrawQuad({ 164, 4, 346, 40 }, 0, 0, 0, 255);
		App->render->DrawQuad({ 166, 6, 340, 36 }, 0, 0, 255, 255);
		App->render->DrawQuad({ 174, 16, 324, 16 }, 0, 0, 0, 255);
		App->render->TextDraw("Press 1 Player  Start", 176, 17, 255, 255, 255, 255, 15);

		//LOG("Press 1 Player Start");
	}

	if (credits >= 2)
	{
		App->render->DrawQuad({ 128, 0, 432, 48 }, 255, 0, 0, 255);
		App->render->DrawQuad({ 132, 4, 424, 40 }, 0, 0, 0, 255);
		App->render->DrawQuad({ 134, 6, 420, 36 }, 0, 0, 255, 255);
		App->render->DrawQuad({ 142, 16, 404, 16 }, 0, 0, 0, 255);
		App->render->TextDraw("Press 1 or 2 Player Start", 144, 17, 255, 255, 255, 255, 15);

		//LOG("Press 1 or 2 Player Start");
	}

	if (credits != 0)
	{
		// Convert int to const char*
		string s_credits = to_string(credits);
		const char* ch_credits = s_credits.c_str();

		App->render->DrawQuad({ 256, 448, 114, 18 }, 0, 0, 0, 255);
		App->render->DrawQuad({ 384, 448, 16, 18 }, 0, 0, 0, 255);
		App->render->TextDraw("Credits ", 256, 448, 0, 0, 255, 255, 15);
		App->render->TextDraw(ch_credits, 384, 448, 0, 0, 255, 255, 15);
	}



	// how to draw the copyright symbol?
	// how to render it after the credits rect?
	App->render->TextDraw("1988 Atari Games", 242, 462, 255, 0, 0, 255, 15);
	// Draw everything --------------------------------------
	
	return update_status::UPDATE_CONTINUE;
}


bool ScreenStart::CleanUp()
{

	App->textures->Unload(bg_texture);

	App->particles->CleanUp();

	//Elminar textos?


	return true;
}