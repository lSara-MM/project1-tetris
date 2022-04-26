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

#include <iostream>
using namespace std;
#include <sstream>


//	TO CHANGE CURTAIN CLOSING ANIMATION (WIN)

ScreenLvl_1::ScreenLvl_1(bool startEnabled) : Module(startEnabled)
{
	openCurtain.PushBack({ 816, 3, 157, 126 });
	openCurtain.PushBack({ 653, 3, 157, 126 });
	openCurtain.PushBack({ 491, 3, 157, 126 });
	openCurtain.PushBack({ 328, 3, 157, 126 });
	openCurtain.PushBack({ 166, 3, 157, 126 });
	openCurtain.PushBack({ 3, 3, 157, 126 });
	openCurtain.speed = 0.2f;
	openCurtain.loop = true;

	closeCurtain.PushBack({ 3, 3, 157, 126 });
	closeCurtain.PushBack({ 166, 3, 157, 126 });
	closeCurtain.PushBack({ 328, 3, 157, 126 });
	closeCurtain.PushBack({ 491, 3, 157, 126 });
	closeCurtain.PushBack({ 653, 3, 157, 126 });
	closeCurtain.PushBack({ 816, 3, 157, 126 });
	closeCurtain.speed = 0.2f;
	closeCurtain.loop = true;
	
}

ScreenLvl_1::~ScreenLvl_1()
{

}

// Load assets
bool ScreenLvl_1::Start()
{
	//App->collisions->Enable();
	App->tetronimo->Enable();

	LOG("Loading lvl 1 background assets");
	LOG("Loading curtain assets\n");

	bool ret = true;

	bg_texture = App->textures->Load("Assets/ss_easyBg.png");
	curtain_texture = App->textures->Load("Assets/curtain.png");
	

	LOG("Loading sound effects");
	fxBlock_Fall = App->audio->LoadFx("Assets/Audio/FX/block_fall.wav");
	fxLine = App->audio->LoadFx("Assets/Audio/FX/line.wav");


	// fxLvl = load different depending on what is needed (save space?)
	fxBlock_Fall = App->audio->LoadFx("Assets/Audio/FX/block_fall.wav");
	fxYou_DidIt = App->audio->LoadFx("Assets/Audio/FX/you_did_it.wav");
	fxClearing_Bars = App->audio->LoadFx("Assets/Audio/FX/clearing_bars.wav");
	fxGameOver = App->audio->LoadFx("Assets/Audio/FX/gameover.wav");
	fxLine = App->audio->LoadFx("Assets/Audio/FX/line.wav"); 


	// Variables

	// points
	p_drop = 1;
	score = 0;
	h = -1;

	lines = 0;
	linesObjective = 12;
	linesleft = linesObjective;

	lvl_credits = App->sStart->credits - 1;
	v_message = 0;
	v_insertCoin = 0;


	// Game
	App->tetronimo->Start();

	return ret;
}

update_status ScreenLvl_1::Update()
{

	if (openCurtain.GetLoopCount() == 1) { openCurtain.speed = 0; }
	else { openCurtain.Update(); }
	
	if (App->input->keys[SDL_SCANCODE_O] == KEY_STATE::KEY_REPEAT)
	{
		score++;
	}

	if (App->input->keys[SDL_SCANCODE_M] == KEY_STATE::KEY_REPEAT)
	{
		lines++;
	}

	if (App->input->keys[SDL_SCANCODE_M] == KEY_STATE::KEY_DOWN)
	{
		linesleft--;
		LOG("Lines left: %d", linesleft);
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		if (lvl_credits < 9) { lvl_credits++; }
		LOG("Curtain loop count: %d", openCurtain.GetLoopCount());
	}


	
	//// Debugging
	//if (App->input->keys[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN) {
	//	lvl_instaWin = true;
	//	v_WinLose = 0;
	//}

	//if (App->input->keys[SDL_SCANCODE_F4] == KEY_STATE::KEY_DOWN) {
	//	lvl_instaLose = true;
	//	v_WinLose = 0;
	//}
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenLvl_1::PostUpdate()
{
	App->render->Blit(bg_texture, 0, 10, NULL);
	
	//Curtain animation
	if (openCurtain.GetLoopCount() == 0)
	{
		App->render->Blit(curtain_texture, 258, 194, &(openCurtain.GetCurrentFrame()), 0.85f);
	}
	

	// strings to const char*
	string s_score = std::to_string(score);
	const char* ch_score = s_score.c_str();

	string s_lines = std::to_string(lines);
	const char* ch_lines = s_lines.c_str();

	string s_linesleft = std::to_string(linesleft);
	const char* ch_linesleft = s_linesleft.c_str();

	string s_credits = to_string(lvl_credits);
	const char* ch_credits = s_credits.c_str();

	

	// Player 1 section
	if (App->tetronimo->lvl_instaLose == false)	{ App->render->TextDraw("next", 16, 25, 255, 0, 0, 255, 16); } // TO CHANGE CONDITION

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
		LOG("Loading background music: Loginska");
		App->audio->PlayMusic("Assets/loginska.ogg", 0.5f); 
		App->tetronimo->SpawnTetronimo();
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


	App->tetronimo->Update();

	// Bottom tower section
	App->render->TextDraw("high score", 255, 370, 0, 0, 150, 255, 16);
	App->render->TextDraw("10000", 288, 385, 0, 0, 150, 255, 16);
	App->render->TextDraw("round", 255, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_lines, 353, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw("credits", 255, 450, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_credits, 385, 450, 0, 0, 150, 255, 16);


	// Player 2 section
	App->render->TextDraw("stats", 480, 110, 255, 255, 255, 255, 16);

	if (v_insertCoin >= 0 && v_insertCoin < 130)
	{
		App->render->TextDraw("Join in ", 475, 433, 148, 151, 255, 255, 15);
		App->render->TextDraw("any time", 475, 451, 148, 151, 255, 255, 15);
	}
	if(v_insertCoin >= 130)
	{
		App->render->TextDraw("Insert", 480, 433, 148, 151, 255, 255, 15);
		App->render->TextDraw("coin", 496, 450, 148, 151, 255, 255, 15);
		if (v_insertCoin == 260) { v_insertCoin = 0; }
	}
	//LOG("insert coin counter %d", v_insertCoin);
	v_insertCoin++;
	

	// Points
	//Tetromino placed = p_drop*p_stack(p_stack+h) 
	//p_drop->1 normal gravity 2 if soft drop (if (App->input->keys[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_DOWN) cuando colisiona con linia)
	//p_stack->number of bars on the rainbow + 1, rainbow llena cada cuatro linias completadas
	//h->row on which tetomino placed minus 1 (bottom = 0)
	h = App->tetronimo->blockRB();
	if (h != (-1))
	{
		if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT) { p_drop = 2; }
		score = p_drop * (p_stack + 1) * (p_stack + 1 + h);
	}

	//Score por linia completada
	//1->50
	//2->150
	//3->400
	//4->900
	

	// Rainbow bar
	if ((lines % 4) == 0)
	{
		if (p_stack != 10)
		{
			p_stack++;
		}

		// Color bars
		if (p_stack >= 1)		// blue
		{
			App->render->DrawQuad({ 16, 461, 17, 4 }, 0, 0, 255, 255, 0);
		}
		if (p_stack >= 2)		// cyan
		{
			App->render->DrawQuad({ 16, 457, 17, 4 }, 0, 255, 255, 255, 0);
		}

		if (p_stack >= 3)		// green
		{
			App->render->DrawQuad({ 16, 453, 17, 4 }, 0, 255, 0, 255, 0);

		}
		if (p_stack >= 4)		// "lighter" green
		{
			App->render->DrawQuad({ 16, 449, 17, 4 }, 75, 255, 0, 255, 0);

		}
		if (p_stack >= 5)		// yellow
		{
			App->render->DrawQuad({ 16, 445, 17, 4 }, 255, 255, 0, 255, 0);

		}
		if (p_stack >= 6)		// dark yellow
		{
			App->render->DrawQuad({ 16, 441, 17, 4 }, 255, 221, 0, 255, 0);
		}
		if (p_stack >= 7)		// orange
		{
			App->render->DrawQuad({ 16, 437, 17, 4 }, 255, 147, 0, 255, 0);
		}
		if (p_stack >= 8)		// dark orange
		{
			App->render->DrawQuad({ 16, 433, 17, 4 }, 255, 75, 0, 255, 0);
		}
		if (p_stack >= 9)		// red
		{
			App->render->DrawQuad({ 16, 429, 17, 4 }, 255, 0, 0, 255, 0);
		}	
		
	}

	//Bonus
	//5*e*(e+1), e=numero filas vacias por encima de la ultima pieza colocada (en teoria maximo 2100)


	// Win conditions
	if (linesleft == 0)
	{
		v_message = 0;
		App->audio->PauseMusic();
		lvl_win();
	}

	return update_status::UPDATE_CONTINUE;
}

void ScreenLvl_1::lvl_win()
{
	App->tetronimo->Disable();

	if (App->tetronimo->v_WinLose >= 0 && App->tetronimo->v_WinLose < 250)
	{
		if (App->tetronimo->v_WinLose == 0) App->audio->PlayFx(fxYou_DidIt, 0);
		else { App->audio->PauseMusic(); }
		//You did it
		App->render->TextDraw("you", 305, 250, 255, 255, 255, 255, 16);
		App->render->TextDraw("did it", 290, 280, 255, 255, 255, 255, 15);
	}

	if (App->tetronimo->v_WinLose >= 250 && App->tetronimo->v_WinLose < 450)		// depende de las lineas vacias al final
	{
		//Bonus
		App->render->TextDraw("bonus for", 272, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw("low", 304, 227, 255, 255, 255, 255, 16);
		App->render->TextDraw("puzzle", 288, 244, 255, 255, 255, 255, 16);		
	}


	if (App->tetronimo->v_WinLose >= 450)
	{
		if (openCurtain.GetLoopCount() == 1) { App->render->Blit(curtain_texture, 258, 194, &(openCurtain.GetCurrentFrame()), 0.85f); }
	}
	if (App->tetronimo->v_WinLose == 600)		// cambiar (depende del bonus)
	{ 
		App->tetronimo->lvl_instaWin = false;
		
		App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
	}

	LOG("win counter %d", App->tetronimo->v_WinLose);
	App->tetronimo->v_WinLose++;
}

void ScreenLvl_1::lvl_lose(const char* ch_loseContinue)
{
	// Game Over
	App->tetronimo->Disable();

	if (App->tetronimo->v_WinLose >= 0 && App->tetronimo->v_WinLose < 200)
	{
		if (App->tetronimo->v_WinLose == 5) App->audio->PlayFx(fxGameOver, 0);
		else { App->audio->PauseMusic(); }

		App->render->DrawQuad({ 63, 0, 131, 66 }, 255, 0, 0, 255);
		App->render->DrawQuad({ 70, 5, 118, 56 }, 0, 0, 255, 255);
		App->render->DrawQuad({ 80, 15, 98, 34 }, 37, 37, 85, 255);
		App->render->TextDraw("Game", 95, 16, 255, 255, 255, 255, 16);
		App->render->TextDraw("Over", 95, 32, 255, 255, 255, 255, 16);
	}
	else if (App->tetronimo->v_WinLose > 200)
	{
		if (App->input->keys[SDL_SCANCODE_Z] == KEY_STATE::KEY_DOWN)
		{
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		App->render->TextDraw("Press", 96, 146, 255, 255, 255, 255, 16);
		App->render->TextDraw("Start", 96, 177, 255, 255, 255, 255, 16);
		App->render->TextDraw("To", 128, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw("Continue", 79, 242, 255, 255, 255, 255, 16);

		App->render->TextDraw(ch_loseContinue, 141, 369, 255, 255, 255, 255, 16);
		if (App->tetronimo->v_WinLose % 50 == 0)
		{
			App->tetronimo->v_loseContinue--;
		}
		LOG("%d win lose", App->tetronimo->v_WinLose);
	}

	if (App->tetronimo->v_loseContinue == 0)
	{
		App->tetronimo->lvl_instaLose = false;
		App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
	}
	
	App->tetronimo->v_WinLose++;
}
