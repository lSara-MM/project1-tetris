#include "Points.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

#include "ScreenLvl_1.h"

Points::Points(bool startEnabled) : Module(startEnabled)
{

}
Points::~Points()
{}

bool Points::Start()
{
	// Points
	p_drop = 1;
	p_stack = 0;
	score = 0;
	h = 0;

	lvl = 10;
	credits = 0;
	lines = 0;

	LOG("Loading credits fx");
	fxAdd_Credits = App->audio->LoadFx("Assets/Audio/FX/add_credit.wav");
	return true;
}

int Points::addCreditsStart()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		if (credits < 9) { credits++; }
		LOG("credits: %d", credits);
		App->audio->PlayFx(fxAdd_Credits);
	}
	return credits;
}

int Points::addCreditsLvl()
{
	if (credits < 9) { credits++; }
	return credits;
}

void Points::calcScore(int value)
{
	if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT) { p_drop = 2; }
	score = score + App->sLvl_1->value;

	App->sLvl_1->v_fall = true;
}

void Points::RainbowStack()
{
	// Color bars
	if (App->points->p_stack >= 1)		// blue
	{
		App->render->DrawQuad({ 16, 461, 17, 4 }, 0, 0, 255, 255, 0);
	}
	if (App->points->p_stack >= 2)		// cyan
	{
		App->render->DrawQuad({ 16, 457, 17, 4 }, 0, 255, 255, 255, 0);
	}
	if (App->points->p_stack >= 3)		// green
	{
		App->render->DrawQuad({ 16, 453, 17, 4 }, 0, 255, 0, 255, 0);
	}
	if (App->points->p_stack >= 4)		// "lighter" green
	{
		App->render->DrawQuad({ 16, 449, 17, 4 }, 75, 255, 0, 255, 0);
	}
	if (App->points->p_stack >= 5)		// yellow
	{
		App->render->DrawQuad({ 16, 445, 17, 4 }, 255, 255, 0, 255, 0);
	}
	if (App->points->p_stack >= 6)		// dark yellow
	{
		App->render->DrawQuad({ 16, 441, 17, 4 }, 255, 221, 0, 255, 0);
	}
	if (App->points->p_stack >= 7)		// orange
	{
		App->render->DrawQuad({ 16, 437, 17, 4 }, 255, 147, 0, 255, 0);
	}
	if (App->points->p_stack >= 8)		// dark orange
	{
		App->render->DrawQuad({ 16, 433, 17, 4 }, 255, 75, 0, 255, 0);
	}
	if (App->points->p_stack >= 9)		// red
	{
		App->render->DrawQuad({ 16, 429, 17, 4 }, 255, 0, 0, 255, 0);
	}
}

void Points::Reset()
{
	p_drop = 1;
	p_stack = 0;
	score = 0;
	h = -1;
	credits = 0;
	lvl = 1;
	lines = 0;
}