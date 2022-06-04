#include "ScreenDiffSelect.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleParticles.h"

#include <iostream>
using namespace std;

uint Time = 0;
uint delta_Time = 0;
uint last_TickTime = 0;

ScreenDiffSelect::ScreenDiffSelect(bool startEnabled) : Module(startEnabled)
{

}

ScreenDiffSelect::~ScreenDiffSelect()
{

}

// Load assets
bool ScreenDiffSelect::Start()
{
	LOG("Loading background assets");
	bool ret = true;

	bg_texture = App->textures->Load("Assets/Diff_Select.png");
	arrowleft_texture = App->textures->Load("Assets/arrow_left.png");
	arrowright_texture = App->textures->Load("Assets/arrow_right.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	p_pos.x = p_x;
	p_pos.y = p_y;

	p_pos2.x = p2_x;
	p_pos2.y = p2_y;

	yellow_rect_texture = App->textures->Load("Assets/Rect/yellow_rect.png");
	orange_rect_texture = App->textures->Load("Assets/Rect/orange_rect.png");
	white_rect_texture = App->textures->Load("Assets/Rect/white_rect.png");

	pink_rect_texture = App->textures->Load("Assets/Rect/pink_rect.png");
	red_rect_texture = App->textures->Load("Assets/Rect/red_rect.png");


	fxAdd_Press_L_R = App->audio->LoadFx("Assets/Audio/FX/diff_selection_arrow.wav");
	fxAdd_PressEnter = App->audio->LoadFx("Assets/Audio/FX/diff_selection_enter.wav");
	return ret;
}

update_status ScreenDiffSelect::Update()
{
	Time = SDL_GetTicks();
	delta_Time += Time - last_TickTime;
	last_TickTime = Time;

	App->render->Blit(bg_texture, 0, 3, NULL);


	//key commands 
	if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN) {

		switch (Index)
		{
		case 0:
			if (Index < 2)
			{
				Index++;
				p_x -= 235;
				p2_x -= 205;

				App->audio->PlayFx(fxAdd_Press_L_R);

				//Colour random
				pos_x = 245;
				pos_y = 130;

				pos_x1 = 245;
				pos_y1 = 244;

				pos_x2 = 245;
				pos_y2 = 365;

				pos_x3 = 422;
				pos_y3 = 365;

				pos_x4 = 422;
				pos_y4 = 365;

				pos_x5 = 422;
				pos_y5 = 130;
			}
			break;
		case 1:
			if (Index < 2)
			{
				Index++;
				p_x -= 210;
				p2_x -= 244;

				App->audio->PlayFx(fxAdd_Press_L_R);

				//Colour random
				pos_x = 22;
				pos_y = 130;

				pos_x1 = 22;
				pos_y1 = 244;

				pos_x2 = 22;
				pos_y2 = 365;

				pos_x3 = 200;
				pos_y3 = 365;

				pos_x4 = 200;
				pos_y4 = 365;

				pos_x5 = 200;
				pos_y5 = 130;

			}
			break;
		}
	}

	if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN) {
		switch (Index)
		{
		case 1:
			if (Index > 0)
			{
				Index--;
				p_x += 235;
				p2_x += 205;
				App->audio->PlayFx(fxAdd_Press_L_R);

				//Colour random
				pos_x = 470;
				pos_y = 130;

				pos_x1 = 470;
				pos_y1 = 244;

				pos_x2 = 470;
				pos_y2 = 365;

				pos_x3 = 648;
				pos_y3 = 365;

				pos_x4 = 648;
				pos_y4 = 365;

				pos_x5 = 648;
				pos_y5 = 130;

			}
		case 2:

			if (Index > 0)
			{
				Index--;
				p_x += 210;
				p2_x += 244;
				App->audio->PlayFx(fxAdd_Press_L_R);

				pos_x = 245;
				pos_y = 130;

				pos_x1 = 245;
				pos_y1 = 244;

				pos_x2 = 245;
				pos_y2 = 365;

				pos_x3 = 422;
				pos_y3 = 365;

				pos_x4 = 422;
				pos_y4 = 365;

				pos_x5 = 422;
				pos_y5 = 130;
			}
		}
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		if (Index == Easy)
		{
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
			App->audio->PlayFx(fxAdd_PressEnter);
		}
	}

	ColourRandom();
	return update_status::UPDATE_CONTINUE;
}

int ScreenDiffSelect::ColourRandom()
{
	Time = SDL_GetTicks();
	delta_Time += Time - last_TickTime;
	last_TickTime = Time;


	colour = rand() % 9;
	if ((delta_Time >= 0) && (delta_Time <= 50))
	{
		LOG("Loading");
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x, pos_y, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x, pos_y, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x, pos_y, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x, pos_y, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x, pos_y, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x, pos_y, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x, pos_y, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x, pos_y, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta_Time >= 50) && (delta_Time <= 100))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x1, pos_y1, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x1, pos_y1, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x1, pos_y1, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x1, pos_y1, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x1, pos_y1, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x1, pos_y1, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x1, pos_y1, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x1, pos_y1, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta_Time >= 100) && (delta_Time <= 150))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x2, pos_y2, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x2, pos_y2, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x2, pos_y2, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x2, pos_y2, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x2, pos_y2, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x2, pos_y2, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x2, pos_y2, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x2, pos_y2, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta_Time >= 150) && (delta_Time <= 200))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x3, pos_y3, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x3, pos_y3, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x3, pos_y3, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x3, pos_y3, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x3, pos_y3, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x3, pos_y3, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x3, pos_y3, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x3, pos_y3, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta_Time >= 200) && (delta_Time <= 250))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x4, pos_y4, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x4, pos_y4, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x4, pos_y4, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x4, pos_y4, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x4, pos_y4, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x4, pos_y4, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x4, pos_y4, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x4, pos_y4, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta_Time >= 250) && (delta_Time <= 300))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x5, pos_y5, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x5, pos_y5, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x5, pos_y5, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x5, pos_y5, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x5, pos_y5, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(bluedark_rect_texture, pos_x5, pos_y5, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(pink_rect_texture, pos_x5, pos_y5, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x5, pos_y5, NULL);
		}
	}


	if ((delta_Time >= 350) && (delta_Time <= 400))
	{
		delta_Time = 400;
	}

	if ((delta_Time > 400))
	{
		delta_Time = 0;
	}
	return 0;
}

// Update: draw background
update_status ScreenDiffSelect::PostUpdate()
{
	App->render->Blit(arrowleft_texture, p_x, p_y, NULL);
	App->render->Blit(arrowright_texture, p2_x, p2_y, NULL);

	App->render->TextDraw("DIFFICULTY SELECT", 185, 12, 255, 0, 0, 255, 20);

	App->render->TextDraw("EASY", 82, 64, 255, 255, 255, 255, 15);
	App->render->TextDraw("MEDIUM", 292, 64, 255, 255, 255, 255, 15);
	App->render->TextDraw("HARD", 528, 64, 255, 255, 255, 255, 15);

	App->render->TextDraw("HANDICAP", 274, 208, 250, 250, 255, 255, 15);
	App->render->TextDraw("START", 306, 240, 255, 250, 250, 255, 15);
	App->render->TextDraw("RANDOM", 512, 208, 255, 250, 250, 255, 15);
	App->render->TextDraw("BLOCKS", 530, 240, 255, 250, 250, 255, 15);

	App->render->TextDraw("ROUND  1", 50, 432, 255, 255, 255, 255, 15);
	App->render->TextDraw("NO BONUS", 50, 448, 255, 255, 255, 255, 15);


	App->render->TextDraw("ROUND  4", 272, 432, 255, 255, 255, 255, 15);
	App->render->TextDraw("20000  BONUS", 242, 448, 255, 255, 255, 255, 15);


	App->render->TextDraw("ROUND  7", 496, 432, 255, 255, 255, 255, 15);
	App->render->TextDraw("40000  BONUS", 466, 448, 255, 255, 255, 255, 15);
	return update_status::UPDATE_CONTINUE;
}

bool ScreenDiffSelect::CleanUp()
{
	App->textures->Unload(bg_texture);
	App->textures->Unload(arrowleft_texture);
	App->textures->Unload(arrowright_texture);

	App->textures->Unload(green_rect_texture);
	App->textures->Unload(blue_rect_texture);
	App->textures->Unload(bluedark_rect_texture);

	App->textures->Unload(yellow_rect_texture);
	App->textures->Unload(orange_rect_texture);
	App->textures->Unload(white_rect_texture);
	
	//audio?
	return true;
}
