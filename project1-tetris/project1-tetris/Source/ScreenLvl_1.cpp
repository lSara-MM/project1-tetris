#include "ScreenLvl_1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleTetronimo.h"
#include "Points.h"

#include <iostream>
using namespace std;
#include <sstream>

uint Time_ = 0;
uint delta__Time = 0;
uint last__TickTime = 0;

uint Time1_ = 0;
uint delta1__Time = 0;
uint last1__TickTime = 0;

uint runTimeSl1 = 0;
uint deltaTimeSl1 = 0;
uint lastTickTimeSl1 = 0;

int i = 0; 
int j = 0;

//	TO CHANGE CURTAIN CLOSING ANIMATION (WIN)
ScreenLvl_1::ScreenLvl_1(bool startEnabled) : Module(startEnabled)
{
	// curtain animation
	openCurtain.PushBack({ 816, 3, 157, 126 });
	openCurtain.PushBack({ 653, 3, 157, 126 });
	openCurtain.PushBack({ 491, 3, 157, 126 });
	openCurtain.PushBack({ 328, 3, 157, 126 });
	openCurtain.PushBack({ 166, 3, 157, 126 });
	openCurtain.PushBack({ 3, 3, 157, 126 });
	//openCurtain.speed = 0.2f;
	openCurtain.loop = true;

	closeCurtain.PushBack({ 3, 3, 157, 126 });
	closeCurtain.PushBack({ 166, 3, 157, 126 });
	closeCurtain.PushBack({ 328, 3, 157, 126 });
	closeCurtain.PushBack({ 491, 3, 157, 126 });
	closeCurtain.PushBack({ 653, 3, 157, 126 });
	closeCurtain.PushBack({ 816, 3, 157, 126 });
	//closeCurtain.speed = 0.2f;
	closeCurtain.loop = true;

	//Bonus
	for (int i = 0; i < 22; i++) {
		LowBonus[i].OnelowBonus.PushBack({ 1, 119, 163, 16 });
	}

	// rus 3
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 11; j++) 
		{
			// the animation has only 8 sprites in a 3x3
			if (j == 4 && i == 29)
			{
				break;
			}
			dancingRus3.PushBack({ 65 * j, 78 * i, 65, 78 });
		}
	} 
	dancingRus3.loop = false;
	dancingRus3.speed = 1.3;
	
	// rus 6
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			dancingRus6.PushBack({ 62 * j, 82 * i, 62, 82 });
		}
	}
	dancingRus6.loop = false;
	dancingRus6.speed = 1.5;
}

ScreenLvl_1::~ScreenLvl_1()
{

}

// Load assets
bool ScreenLvl_1::Start()
{
	App->tetronimo->Enable();
	App->points->Enable();
	LOG("Loading lvl 1 background assets");
	LOG("Loading curtain assets\n");

	bool ret = true;
	if (App->points->lvl <= 3 || App->points->lvl==10)
	{
		bg_texture = App->textures->Load("Assets/ss_easyBg.png");
	}
	if (App->points->lvl > 3 && App->points->lvl <= 6)
	{
		bg_texture = App->textures->Load("Assets/ss_mediumBg.png");
	}
	if (App->points->lvl > 6 && App->points->lvl <=  9)
	{
		bg_texture = App->textures->Load("Assets/ss_hardBg.png");
	}
	
	curtain_texture = App->textures->Load("Assets/curtain.png");
	lowBonus_texture = App->textures->Load("Assets/Sprites/lowBonus.png");

	LOG("Loading ruso sexy moves");
	ruso_texture3 = App->textures->Load("Assets/Sprites/Rus/3_spritesheet2.png");
	ruso_texture6 = App->textures->Load("Assets/Sprites/Rus/6_spritesheet2.png");
	//ruso_texture9 = App->textures->Load("Assets/Sprites/Rus/9_spritesheet.png");

		//Tetro_rect random

	green_rect_texture = App->textures->Load("Assets/Rect/green_rect.png");
	blue_rect_texture = App->textures->Load("Assets/Rect/blue_rect.png");
	bluedark_rect_texture = App->textures->Load("Assets/Rect/bluedark_rect.png");

	yellow_rect_texture = App->textures->Load("Assets/Rect/yellow_rect.png");
	orange_rect_texture = App->textures->Load("Assets/Rect/orange_rect.png");
	white_rect_texture = App->textures->Load("Assets/Rect/white_rect.png");

	pink_rect_texture = App->textures->Load("Assets/Rect/pink_rect.png");
	red_rect_texture = App->textures->Load("Assets/Rect/red_rect.png");

	LOG("Loading sound effects");
	fxBlock_Fall = App->audio->LoadFx("Assets/Audio/FX/block_fall.wav");
	fxLine = App->audio->LoadFx("Assets/Audio/FX/line.wav");


	// fxLvl = load different depending on what is needed (save space?)
	fxBlock_Fall = App->audio->LoadFx("Assets/Audio/FX/block_fall.wav");
	fxYou_DidIt = App->audio->LoadFx("Assets/Audio/FX/you_did_it.wav");
	fxClearing_Bars = App->audio->LoadFx("Assets/Audio/FX/clearing_bars.wav");
	fxGameOver = App->audio->LoadFx("Assets/Audio/FX/gameover.wav");
	fxLine = App->audio->LoadFx("Assets/Audio/FX/line.wav");
	fxruso3 = App->audio->LoadFx("Assets/Audio/FX/03_Hopak__Round_3_.wav");
	fxruso6= App->audio->LoadFx("Assets/Audio/FX/06__Round_6_.wav");
	fxBonus = App->audio->LoadFx("Assets/Audio/FX/ShortBonus.wav");
	
	App->points->v_r = 0;
	App->points->v_g = 0;
	App->points->v_b = 0;
	App->points->v_y = 0;
	App->points->v_p = 0;
	App->points->v_c = 0;
	App->points->v_o = 0;

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			App->tetronimo->colorCount[j][i].id = -1;
		}
	}

	// Variables
	switch (App->points->lvl)
	{
	case 1:
		linesObjective = 5;
		break;
	case 2:
		linesObjective = 10;
		break;
	case 3:
		linesObjective = 12;
		break;
	case 4:
		linesObjective = 10;
		break;
	case 5:
		linesObjective = 13;
		break;
	case 6:
		linesObjective = 16;
		break;
	case 7:
		linesObjective = 12;
		break;
	case 10:
		linesObjective = 12;
		break;
	default:
		break;
	}
	linesleft = linesObjective;

	if (App->points->credits < 0)
	{
		App->points->Reset();
		App->points->credits = 1;
	}

	// Counter
	v_fall = true;
	v_message = 0;
	v_insertCoin = 0;
	v_points = 0;
	value = 0;
	// debug
	lvl_instaWin = false;
	lvl_instaLose = false;

	v_loseContinue = 9;
	v_WinLose = 0;
	// Game
	App->tetronimo->Start();

	openCurtain.loopCount = 0;
	openCurtain.speed = 0.85f;
	closeCurtain.loopCount = 0;
	/*closeCurtain.speed = 0.2f;*/
	return ret;
}

update_status ScreenLvl_1::Update()
{
	runTimeSl1 = SDL_GetTicks();
	deltaTimeSl1 += runTimeSl1 - lastTickTimeSl1;
	lastTickTimeSl1 = runTimeSl1;

	if (openCurtain.GetLoopCount() == 1) {
		openCurtain.speed = 0;
	}
	else { openCurtain.Update(); }

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->points->addCreditsLvl();
		LOG("Curtain loop count: %d", openCurtain.GetLoopCount());
	}
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenLvl_1::PostUpdate()
{
	App->render->Blit(bg_texture, 0, 10, NULL);

	//Curtain animation
	if (openCurtain.GetLoopCount() == 0)
	{
		if (App->points->lvl == 1 || App->points->lvl == 2 || App->points->lvl == 3 || App->points->lvl == 10) {
			App->render->Blit(curtain_texture, 258, 194, &(openCurtain.GetCurrentFrame()), 0.85f);
		}
		else if (App->points->lvl == 4 || App->points->lvl == 5 || App->points->lvl == 6) {
			App->render->Blit(curtain_texture, 258, 192, &(openCurtain.GetCurrentFrame()), 0.85f);
		}
		else if (App->points->lvl == 7) {
			App->render->Blit(curtain_texture, 257, 192, &(openCurtain.GetCurrentFrame()), 0.85f);
		}
	}

	// strings to const char*
	string s_round = std::to_string(App->points->lvl);
	const char* c_round = s_round.c_str();

	string s_score = std::to_string(App->points->score);
	const char* ch_score = s_score.c_str();

	string s_lines = std::to_string(App->points->lines);
	const char* ch_lines = s_lines.c_str();

	string s_linesleft = std::to_string(linesleft);
	const char* ch_linesleft = s_linesleft.c_str();

	string s_credits = to_string(App->points->credits);
	const char* ch_credits = s_credits.c_str();

	// Player 1 section
	if (lvl_instaLose == false) { App->render->TextDraw("next", 16, 25, 255, 0, 0, 255, 16); } // TO CHANGE CONDITION

	App->render->TextDraw("Score", 49, 433, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_score, 137, 433, 255, 0, 0, 255, 15);

	App->render->TextDraw("lines", 49, 451, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_lines, 137, 451, 255, 0, 0, 255, 15);


	// Mid tower section
	//Complete x lines to win
	if (openCurtain.GetLoopCount() > 0 && v_message < 100 && v_message != 0)
	{
		App->render->TextDraw("complete", 272, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw(ch_linesleft, 272, 242, 255, 255, 255, 255, 16);
		App->render->TextDraw("lines", 320, 240, 252, 255, 255, 255, 16);
		App->render->TextDraw("to go to", 272, 272, 255, 255, 255, 255, 16);
		App->render->TextDraw("next round", 257, 305, 255, 255, 255, 255, 16);
	}

	else if (v_message == 100)
	{
		if (App->points->lvl == 1 || App->points->lvl == 5)
		{
			LOG("Loading background music: Loginska");
			App->audio->PlayMusic("Assets/Audio/Music/01_Loginska.ogg", 0);
		}
		if (App->points->lvl == 2 || App->points->lvl == 6 || App->points->lvl == 10)
		{
			LOG("Loading background music: Loginska");
			App->audio->PlayMusic("Assets/Audio/Music/04_Bradinsky.ogg", 0);
		}
		if (App->points->lvl == 3 || App->points->lvl == 7)
		{
			LOG("Loading background music: Loginska");
			App->audio->PlayMusic("Assets/Audio/Music/07_Karinka.ogg", 0);
		}
		if (App->points->lvl == 4)
		{
			LOG("Loading background music: Loginska");
			App->audio->PlayMusic("Assets/Audio/Music/10_Troika.ogg", 0);
		}

		App->tetronimo->spawnTetronimo(App->tetronimo->nextT);
	}

	//Lines left
	else if (linesleft > 0 && linesleft <= linesObjective && v_message > 100)
		// Appears when spawns first block (SONIA)
	{
		App->render->TextDraw("0", 270, 225, 255, 0, 0, 255, 15);
		App->render->TextDraw(ch_linesleft, 285, 225, 255, 0, 0, 255, 15);
		App->render->TextDraw("lines", 305, 255, 255, 255, 255, 255, 16);
		App->render->TextDraw("left", 305, 285, 255, 255, 255, 255, 16);
	}
	v_message++;

	// Bottom tower section
	App->render->TextDraw("high score", 255, 370, 0, 0, 150, 255, 16);
	App->render->TextDraw("10000", 288, 385, 0, 0, 150, 255, 16);
	App->render->TextDraw("round", 255, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw(c_round, 353, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw("credits", 255, 450, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_credits, 385, 450, 0, 0, 150, 255, 16);


	// Player 2 section
	App->render->TextDraw("next", 605, 25, 137, 137, 235, 255, 16);
	if (App->points->y_ > 5) { App->render->TextDraw("stats", 480, 110, 255, 255, 255, 255, 16); }

	if (v_insertCoin >= 0 && v_insertCoin < 130)
	{
		App->render->TextDraw("Join in ", 475, 433, 148, 151, 255, 255, 15);
		App->render->TextDraw("any time", 475, 451, 148, 151, 255, 255, 15);
	}
	if (v_insertCoin >= 130)
	{
		App->render->TextDraw("Insert", 480, 433, 148, 151, 255, 255, 15);
		App->render->TextDraw("coin", 496, 450, 148, 151, 255, 255, 15);
		if (v_insertCoin == 260) { v_insertCoin = 0; }
	}
	v_insertCoin++;

	// Points
	//Tetromino placed = p_drop*p_stack(p_stack+h) 
	//p_drop->1 normal gravity 2 if soft drop (if (App->input->keys[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_DOWN) cuando colisiona con linia)
	//p_stack->number of bars on the rainbow + 1, rainbow llena cada cuatro linias completadas
	//h->row on which tetomino placed minus 1 (bottom = 0)
	if (lvl_instaLose == false && v_fall == false)
	{
		v_points = 0;

		if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT) { App->points->p_drop = 2; }

		value = (App->points->p_drop * (App->points->p_stack + 1) * (App->points->p_stack + 1 + App->points->h));
		App->points->calcScore(value);

	}
	if (v_points < 30 && value != 0 && App->tetronimo->v_lines == 0)
	{
		string s_points = to_string(value);
		const char* ch_points = s_points.c_str();

		(value < 100) ? App->render->TextDraw(ch_points, 195, 405, 21, 11, 134, 255, 16) : App->render->TextDraw(ch_points, 185, 405, 21, 11, 134, 255, 16);
		v_points++;
	}

	// Rainbow bar			
	App->points->RainbowStack();

	if (linesleft == 0) { ColourRandom(); }
	
	if (linesleft == 5)
	{
		App->render->TextDraw("5", 14, 138, 0, 0, 255, 255, 28);
	}
	else if (linesleft == 4)
	{
		App->render->TextDraw("4", 14, 192, 0, 0, 255, 255, 28);
	}
	else if (linesleft == 3)
	{
		App->render->TextDraw("3", 14, 246, 0, 0, 255, 255, 28);
	}
	else if (linesleft == 2)
	{
		App->render->TextDraw("2", 14, 300, 0, 0, 255, 255, 28);
	}
	else if (linesleft == 1)
	{
		Time1_ = SDL_GetTicks();
		delta1__Time += Time1_ - last1__TickTime;
		last1__TickTime = Time1_;

		if ((delta1__Time >= 0) & (delta1__Time <= 150))
		{
			App->render->TextDraw("1", 14, 354, 0, 0, 255, 255, 28);
		}
		if ((delta1__Time >= 150) & (delta1__Time <= 250))
		{
			App->render->TextDraw("1", 14, 354, 255, 255, 255, 255, 28);
		}

		if ((delta1__Time >= 250) && (delta1__Time <= 250))
		{
			delta1__Time = 250;
		}

		if ((delta1__Time > 250))
		{
			delta1__Time = 0;
		}
	}

	// Win conditions
	if (linesleft <= 0)
	{
		v_message = 0;
		App->audio->PauseMusic();
		lvl_win();
	}

	// Instant win
	if (App->input->keys[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN) {
		lvl_instaWin = true;
		v_WinLose = 0;
	}

	// Instant lose
	if (App->input->keys[SDL_SCANCODE_F4] == KEY_STATE::KEY_DOWN) {
		lvl_instaLose = true;
		v_WinLose = 0;
	}

	if (App->input->keys[SDL_SCANCODE_F5] == KEY_REPEAT) 
	{

		if (App->input->keys[SDL_SCANCODE_1] == KEY_STATE::KEY_DOWN)//saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 1;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		else if (App->input->keys[SDL_SCANCODE_2] == KEY_STATE::KEY_DOWN)//saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 2;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);

		}

		else if (App->input->keys[SDL_SCANCODE_3] == KEY_STATE::KEY_DOWN)//saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 3;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);

		}

		else if (App->input->keys[SDL_SCANCODE_4] == KEY_STATE::KEY_DOWN) //saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 4;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		else if (App->input->keys[SDL_SCANCODE_5] == KEY_STATE::KEY_DOWN)//saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 5;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		else if (App->input->keys[SDL_SCANCODE_6] == KEY_STATE::KEY_DOWN)//saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 6;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		else if (App->input->keys[SDL_SCANCODE_7] == KEY_STATE::KEY_DOWN)//saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 7;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		else if (App->input->keys[SDL_SCANCODE_0] == KEY_STATE::KEY_DOWN)//saltar al nivell que es vulgui
		{
			App->audio->PauseMusic();
			App->points->lvl = 10;
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}
	}

	if (lvl_instaLose == true)
	{
		string s_loseContinue = to_string(v_loseContinue);
		const char* ch_loseContinue = s_loseContinue.c_str();

		//LOG("Instant Lost!");
		App->audio->PauseMusic();
		lvl_lose(ch_loseContinue);
	}

	if (lvl_instaWin == true)
	{
		v_message = 0;
		//LOG("Level complete (Instant Win)!");
		App->audio->PauseMusic();
		lvl_win();
	}

	return update_status::UPDATE_CONTINUE;
}

void ScreenLvl_1::lvl_win()
{
	App->tetronimo->b1->id = -1;
	App->tetronimo->b2->id = -1;
	App->tetronimo->b3->id = -1;
	App->tetronimo->b4->id = -1;
	App->tetronimo->pause = true;

	if (v_WinLose >= 0 && v_WinLose < 85)
	{
		if (v_WinLose == 0) App->audio->PlayFx(fxYou_DidIt, 0);
		else { App->audio->PauseMusic(); }
		//You did it
		App->render->TextDraw("you", 305, 250, 255, 255, 255, 255, 16);
		App->render->TextDraw("did it", 290, 280, 255, 255, 255, 255, 15);
	}

	if (v_WinLose >= 90 && v_WinLose < 320)		// depende de las lineas vacias al final
	{
		//Bonus
		App->render->TextDraw("bonus for", 272, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw("low", 304, 227, 255, 255, 255, 255, 16);
		App->render->TextDraw("puzzle", 288, 244, 255, 255, 255, 255, 16);

		if (v_WinLose >= 115) 
		{
			//if (v_WinLose == 120) 
			//{
			//	App->audio->PlayFx(fxBonus, 0);
			//}

			string b_bonus = std::to_string(Bonus);
			const char* ch_bonus = b_bonus.c_str();

			int fila = 0;
			LowBonus[0].y = 82;

			for (int i = 0; i <= 22; i++) {
				for (int j = 0; j < 10; j++) {
					if ((App->tetronimo->tileSet[j][i].id != -1) && (fila == 0))
					{
						fila = i - 1;
					}
				}
			}

			if (i > fila - 2)
			{
				j = 1;
			}

			if (j == 0)
			{
				App->render->TextDraw("bonus", 66, 405, 0, 0, 150, 255, 15);
				App->render->TextDraw(ch_bonus, 170, 405, 0, 0, 150, 255, 15);

				if (deltaTimeSl1 > 60)
				{
					App->audio->PlayFx(fxBonus, 0);

					Bonus += 10 * (i + 1);

					if (i != 0)
					{
						LowBonus[i].y = LowBonus[i - 1].y;
						LowBonus[i].y += 16;
					}

					App->render->Blit(lowBonus_texture, 63, LowBonus[i].y, &(LowBonus[i].OnelowBonus.GetCurrentFrame()), 1.5f);

					i++;

					deltaTimeSl1 = 0;
				}

				for (int r = 0; r < i; r++)
				{
					App->render->Blit(lowBonus_texture, 63, LowBonus[r].y, &(LowBonus[r].OnelowBonus.GetCurrentFrame()), 1.5f);
				}
			}

			//if (scr == 0)
			//{
			//	for (int i = 1; i < fila; i++)
			//	{
			//		Bonus += 10 * i;
			//		
			//		App->render->TextDraw("bonus", 65, 396, 0, 0, 150, 255, 15);
			//		App->render->TextDraw(ch_bonus, 65, 396, 0, 0, 150, 255, 15);
			//	}

			//	scr = 1;
			//}

			if (j == 1)
			{
				for (int i = 0; i < fila - 1; i++)
				{
					App->render->TextDraw("bonus", 66, 405, 0, 0, 150, 255, 15);
					App->render->TextDraw(ch_bonus, 170, 405, 0, 0, 150, 255, 15);

					App->render->Blit(lowBonus_texture, 63, LowBonus[i].y, &(LowBonus[i].OnelowBonus.GetCurrentFrame()), 1.5f);
				}
			}
		}
	}

	if (App->points->lvl == 3 || App->points->lvl == 6)
	{
		if (v_WinLose >= 150)
		{
			// Ruso
			// cuando acaba la cortinita esa rara de los bloques grises
			if (App->points->lvl == 3 && dancingRus3.HasFinished() == false)
			{
				if (v_WinLose == 150)
				{
					App->audio->PlayFx(fxruso3, 0);
				}
				dancingRus3.Update();
				App->render->Blit(ruso_texture3, 272, 104, &(dancingRus3.GetCurrentFrame()), 0.85f);
			}

			if (App->points->lvl == 6)
			{
				if (v_WinLose == 150)
				{
					App->audio->PlayFx(fxruso6, 0);
				}
				dancingRus6.Update();
				App->render->Blit(ruso_texture6, 272, 98, &(dancingRus6.GetCurrentFrame()), 0.85f);
			}

		}
		if ((App->points->lvl == 3 && dancingRus3.HasFinished() == true) ||
			(App->points->lvl == 6 && dancingRus6.HasFinished() == true))
		{
			if (App->points->lvl == 3) {
				LOG("Close Curtain");
				if (closeCurtain.GetLoopCount() == 0) { App->render->Blit(curtain_texture, 258, 194, &(closeCurtain.GetCurrentFrame()), 0.1f); }
				closeCurtain.Update();
			}
			else if (App->points->lvl == 6) {
				LOG("Close Curtain");
				if (closeCurtain.GetLoopCount() == 0) { App->render->Blit(curtain_texture, 258, 192, &(closeCurtain.GetCurrentFrame()), 0.1f); }
				closeCurtain.Update();
			}
			
			if (closeCurtain.GetLoopCount() > 0)		// cambiar (depende del bonus)
			{
				lvl_instaWin = false;
				App->points->lvl++;
				App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
			}
		}
	}
	else
	{
		if (v_WinLose > 170)
		{
			LOG("Close Curtain");
			if (closeCurtain.GetLoopCount() == 0) { App->render->Blit(curtain_texture, 258, 194, &(closeCurtain.GetCurrentFrame()), 0.1f); }
			closeCurtain.Update();
		}

		if (v_WinLose == 175)		// cambiar (depende del bonus)
		{
			i = 0;
			j = 0;
			scr = 0;
			value += Bonus;
			App->points->calcScore(value);

			lvl_instaWin = false;

			// Counter
			v_message = 0;
			v_insertCoin = 0;
			v_points = 0;

			// debug
			lvl_instaWin = false;
			lvl_instaLose = false;

			v_loseContinue = 9;
			v_WinLose = 0;

			if (App->points->lvl == 7)
			{
				App->points->lvl = 10;
			}
			else if (App->points->lvl == 10)
			{
				App->points->Reset();
				App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
			}
			else
			{
				App->points->lvl++;
			}
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}
	}

	LOG("win counter %d", v_WinLose);
	v_WinLose++;
}

void ScreenLvl_1::lvl_lose(const char* ch_loseContinue)
{
	LOG("v_WinLose: %d", v_WinLose);

	// Game Over
	App->tetronimo->pause = true;

	if (v_WinLose >= 0 && v_WinLose < 100)
	{
		if (v_WinLose == 5) App->audio->PlayFx(fxGameOver, 0);
		else { App->audio->PauseMusic(); }

		App->render->DrawQuad({ 63, 0, 131, 66 }, 255, 0, 0, 255);
		App->render->DrawQuad({ 70, 5, 118, 56 }, 0, 0, 255, 255);
		App->render->DrawQuad({ 80, 15, 98, 34 }, 37, 37, 85, 255);
		App->render->TextDraw("Game", 95, 16, 255, 255, 255, 255, 16);
		App->render->TextDraw("Over", 95, 32, 255, 255, 255, 255, 16);
	}
	else if (v_WinLose > 100)
	{
		App->tetronimo->Disable();
		if (App->input->keys[SDL_SCANCODE_R] == KEY_STATE::KEY_DOWN)
		{
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		App->render->TextDraw("Press", 96, 146, 255, 255, 255, 255, 16);
		App->render->TextDraw("Start", 96, 177, 255, 255, 255, 255, 16);
		App->render->TextDraw("To", 128, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw("Continue", 79, 242, 255, 255, 255, 255, 16);

		App->render->TextDraw(ch_loseContinue, 141, 369, 255, 255, 255, 255, 16);

		if (v_WinLose % 25 == 0)
		{
			v_loseContinue--;
		}
		LOG("%d win lose", v_WinLose);
	}

	if (v_loseContinue == 0)
	{
		lvl_instaLose = false;
		App->points->Reset();
		App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
	}

	v_WinLose++;
}

int ScreenLvl_1::ColourRandom()
{
	Time_ = SDL_GetTicks();
	delta__Time += Time_ - last__TickTime;
	last__TickTime = Time_;

	srand(time(NULL));
	colour = rand() % 9;

	/*App->render->Blit(green_rect_texture, 55, 165, NULL);
	App->render->Blit(green_rect_texture, 55, 215, NULL);
	App->render->Blit(green_rect_texture, 55, 265, NULL);
	App->render->Blit(green_rect_texture, 55, 315, NULL);
	App->render->Blit(green_rect_texture, 55, 365, NULL);

	App->render->Blit(green_rect_texture, 232, 110, NULL);
	App->render->Blit(green_rect_texture, 232, 165, NULL);
	App->render->Blit(green_rect_texture, 232, 215, NULL);
	App->render->Blit(green_rect_texture, 232, 265, NULL);
	App->render->Blit(green_rect_texture, 232, 315, NULL);
	App->render->Blit(green_rect_texture, 232, 365, NULL);*/


	if ((delta__Time >= 0) && (delta__Time <= 150))
	{
		LOG("Loading");
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(green_rect_texture, pos_x + 176, pos_y, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(blue_rect_texture, pos_x + 176, pos_y, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(orange_rect_texture, pos_x + 176, pos_y, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(white_rect_texture, pos_x + 176, pos_y, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(yellow_rect_texture, pos_x + 176, pos_y, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(pink_rect_texture, pos_x + 176, pos_y, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(bluedark_rect_texture, pos_x + 176, pos_y, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x, pos_y, NULL);
			App->render->Blit(red_rect_texture, pos_x + 176, pos_y, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta__Time >= 150) && (delta__Time <= 250))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(green_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(blue_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(orange_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(white_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(yellow_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(pink_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(bluedark_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x1, pos_y1, NULL);
			App->render->Blit(red_rect_texture, pos_x1 + 176, pos_y1, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta__Time >= 250) && (delta__Time <= 350))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(green_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(blue_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(orange_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(white_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(yellow_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(pink_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(bluedark_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x2, pos_y2, NULL);
			App->render->Blit(red_rect_texture, pos_x2 + 176, pos_y2, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta__Time >= 350) && (delta__Time <= 450))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(green_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(blue_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(orange_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(white_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(yellow_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(pink_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(bluedark_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x3, pos_y3, NULL);
			App->render->Blit(red_rect_texture, pos_x3 + 176, pos_y3, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta__Time >= 450) && (delta__Time <= 550))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(green_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(blue_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(orange_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(white_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(yellow_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(pink_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(pink_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(bluedark_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(bluedark_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x4, pos_y4, NULL);
			App->render->Blit(red_rect_texture, pos_x4 + 176, pos_y4, NULL);
		}
	}

	colour = rand() % 9;

	if ((delta__Time >= 550) && (delta__Time <= 650))
	{
		if (colour == 1)
		{
			App->render->Blit(green_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(green_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
		else if (colour == 2)
		{
			App->render->Blit(blue_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(blue_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
		else if (colour == 3)
		{
			App->render->Blit(orange_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(orange_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
		else if (colour == 4)
		{
			App->render->Blit(white_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(white_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
		else if (colour == 5)
		{
			App->render->Blit(yellow_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(yellow_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
		else if (colour == 6)
		{
			App->render->Blit(bluedark_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(bluedark_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
		else if (colour == 7)
		{
			App->render->Blit(pink_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(pink_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
		else if (colour == 8)
		{
			App->render->Blit(red_rect_texture, pos_x5, pos_y5, NULL);
			App->render->Blit(red_rect_texture, pos_x5 + 176, pos_y5, NULL);
		}
	}

	if ((delta__Time >= 650) & (delta__Time <= 750))
	{
		delta__Time = 750;
	}

	if ((delta__Time > 750))
	{
		delta__Time = 0;
	}
	return 0;
}

bool ScreenLvl_1::CleanUp()
{
	LOG("Disable Tetronimo");
	App->tetronimo->Disable();
	//App->render->ttfQuit();	// excepcio xd
	//App->audio->CleanUp();	// excepcio xd
	App->textures->Unload(bg_texture);
	App->textures->Unload(curtain_texture);
	App->textures->Unload(ruso_texture3);
	App->textures->Unload(ruso_texture6);

	App->textures->Unload(green_rect_texture);
	App->textures->Unload(blue_rect_texture);
	App->textures->Unload(bluedark_rect_texture);

	App->textures->Unload(yellow_rect_texture);
	App->textures->Unload(orange_rect_texture);
	App->textures->Unload(white_rect_texture);

	App->textures->Unload(pink_rect_texture);
	App->textures->Unload(red_rect_texture);
	
	openCurtain.Reset();	//  no fa res xd
	closeCurtain.Reset();

	// Counter
	v_fall = true;
	v_message = 0;
	v_insertCoin = 0;
	v_points = 0;

	// debug
	lvl_instaWin = false;
	lvl_instaLose = false;

	v_loseContinue = 9;
	v_WinLose = 0;

	return true;
}
