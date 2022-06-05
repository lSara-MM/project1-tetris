#include "Points.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

#include "ScreenLvl_1.h"
#include "ModuleTetronimo.h"

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

	lvl = 1;
	credits = 0;
	lines = 0;

	v_r = 0, v_g = 0, v_b = 0, v_y = 0, v_p = 0, v_c = 0, v_o = 0;

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			App->tetronimo->colorCount[j][i].id = -1;
		}
	}

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


void Points::colorCount(int v, int c)
{
	y_ = 21-v;
	switch (c)
	{
	case BLOCK_TYPE::RED:
		x_ = 0;
		break;
	case BLOCK_TYPE::GREEN:
		x_ = 1;
		break;
	case BLOCK_TYPE::BLUE:
		x_ = 2;
		break;
	case BLOCK_TYPE::YELLOW:
		x_ = 3;
		break;
	case BLOCK_TYPE::PINK:
		x_ = 4;
		break;
	case BLOCK_TYPE::CYAN:
		x_ = 5;
		break;
	case BLOCK_TYPE::ORANGE:
		x_ = 6;
		break;
	default:
		break;
	}
	if (v == 1)
	{
		switch (c)
		{
		case BLOCK_TYPE::RED:
			App->tetronimo->colorCount[x_][20].id = 4;
			break;
		case BLOCK_TYPE::GREEN:
			App->tetronimo->colorCount[x_][20].id = 15;
			break;
		case BLOCK_TYPE::BLUE:
			App->tetronimo->colorCount[x_][20].id = 105;
			break;
		case BLOCK_TYPE::YELLOW:
			App->tetronimo->colorCount[x_][20].id = 48;
			break;
		case BLOCK_TYPE::PINK:
			App->tetronimo->colorCount[x_][20].id = 55;
			break;
		case BLOCK_TYPE::CYAN:
			App->tetronimo->colorCount[x_][20].id = 75;
			break;
		case BLOCK_TYPE::ORANGE:
			App->tetronimo->colorCount[x_][20].id = 85;
			break;
		default:
			break;
		}
	}
	else
	{
		if (y_ > 0)
		{
			App->tetronimo->colorCount[x_][y_].id = App->tetronimo->colorCount[x_][y_ + 1].id;
			switch (c)
			{
			case BLOCK_TYPE::RED:
				App->tetronimo->colorCount[x_][y_ + 1].id = 5;
				break;
			case BLOCK_TYPE::GREEN:
				App->tetronimo->colorCount[x_][y_ + 1].id = 100;
				break;
			case BLOCK_TYPE::BLUE:
				App->tetronimo->colorCount[x_][y_ + 1].id = 101;
				break;
			case BLOCK_TYPE::YELLOW:
				App->tetronimo->colorCount[x_][y_ + 1].id = 46;
				break;
			case BLOCK_TYPE::PINK:
				App->tetronimo->colorCount[x_][y_ + 1].id = 56;
				break;
			case BLOCK_TYPE::CYAN:
				App->tetronimo->colorCount[x_][y_ + 1].id = 102;
				break;
			case BLOCK_TYPE::ORANGE:
				App->tetronimo->colorCount[x_][y_ + 1].id = 103;
				break;
			default:
				break;
			}
		}
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