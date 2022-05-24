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
	lines = 0;
	linesObjective = 5;
	linesleft = linesObjective;
	App->points->credits -= 1;


	// Counter
	v_message = 0;
	v_insertCoin = 0;
	v_points = 0;

	// debug
	lvl_instaWin = false;
	lvl_instaLose = false;

	v_loseContinue = 9;
	v_WinLose = 0;
	// Game
	App->tetronimo->Start();

	return ret;
}

update_status ScreenLvl_1::Update()
{
	if (openCurtain.GetLoopCount() == 1) { openCurtain.speed = 0; }
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
		App->render->Blit(curtain_texture, 258, 194, &(openCurtain.GetCurrentFrame()), 0.85f);
	}


	// strings to const char*
	string s_score = std::to_string(App->points->score);
	const char* ch_score = s_score.c_str();

	string s_lines = std::to_string(lines);
	const char* ch_lines = s_lines.c_str();

	string s_linesleft = std::to_string(linesleft);
	const char* ch_linesleft = s_linesleft.c_str();

	string s_credits = to_string(App->points->credits);
	const char* ch_credits = s_credits.c_str();


	//App->tetronimo->Update();

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
		LOG("Loading background music: Loginska");
		App->audio->PlayMusic("Assets/Audio/Music/01_Loginska.ogg", 0.5f);
		//App->tetronimo->SpawnTetronimo();
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
	App->render->TextDraw("1", 353, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw("credits", 255, 450, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_credits, 385, 450, 0, 0, 150, 255, 16);


	// Player 2 section
	App->render->TextDraw("next", 605, 25, 137, 137, 235, 255, 16);
	App->render->TextDraw("stats", 480, 110, 255, 255, 255, 255, 16);

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
	//LOG("insert coin counter %d", v_insertCoin);
	v_insertCoin++;


	// Points
	//Tetromino placed = p_drop*p_stack(p_stack+h) 
	//p_drop->1 normal gravity 2 if soft drop (if (App->input->keys[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_DOWN) cuando colisiona con linia)
	//p_stack->number of bars on the rainbow + 1, rainbow llena cada cuatro linias completadas
	//h->row on which tetomino placed minus 1 (bottom = 0)
	if (lvl_instaLose == false)
	{
		//App->points->h = App->tetronimo->blockRB();
		if (App->points->h != (-1))
		{
			v_points = 0;
			App->points->calcScore();
			
			value = (App->points->p_drop * (App->points->p_stack + 1) * (App->points->p_stack + 1 + App->points->h));

		}

		if (v_points < 30 && value != 0)
		{
			string s_points = to_string(value);
			const char* ch_points = s_points.c_str();

			App->render->TextDraw(ch_points, 195, 405, 21, 11, 134, 255, 16);
			v_points++;
		}
	}


	//// Rainbow bar			
	//if ((lines % 4) == 0 && lines != 0)
	//{
	//	if (v_stack == true)
	//	{

	//		v_stack = false;
	//	}
	//}


	//Bonus
	//5*e*(e+1), e=numero filas vacias por encima de la ultima pieza colocada (en teoria maximo 2100)


	// Win conditions
	if (linesleft == 0)
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
	App->tetronimo->Disable();

	if (v_WinLose >= 0 && v_WinLose < 250)
	{
		if (v_WinLose == 0) App->audio->PlayFx(fxYou_DidIt, 0);
		else { App->audio->PauseMusic(); }
		//You did it
		App->render->TextDraw("you", 305, 250, 255, 255, 255, 255, 16);
		App->render->TextDraw("did it", 290, 280, 255, 255, 255, 255, 15);
	}

	if (v_WinLose >= 250 && v_WinLose < 450)		// depende de las lineas vacias al final
	{
		//Bonus
		App->render->TextDraw("bonus for", 272, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw("low", 304, 227, 255, 255, 255, 255, 16);
		App->render->TextDraw("puzzle", 288, 244, 255, 255, 255, 255, 16);		
	}


	if (v_WinLose >= 450)
	{
		if (openCurtain.GetLoopCount() == 1) { App->render->Blit(curtain_texture, 258, 194, &(openCurtain.GetCurrentFrame()), 0.85f); }
	}
	if (v_WinLose == 600)		// cambiar (depende del bonus)
	{ 
		lvl_instaWin = false;

		App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
	}

	LOG("win counter %d", v_WinLose);
	v_WinLose++;
}

void ScreenLvl_1::lvl_lose(const char* ch_loseContinue)
{
	LOG("v_WinLose: %d", v_WinLose);

	// Game Over
	App->tetronimo->Disable();
	
	if (v_WinLose >= 0 && v_WinLose < 200)
	{
		if (v_WinLose == 5) App->audio->PlayFx(fxGameOver, 0);
		else { App->audio->PauseMusic(); }

		App->render->DrawQuad({ 63, 0, 131, 66 }, 255, 0, 0, 255);
		App->render->DrawQuad({ 70, 5, 118, 56 }, 0, 0, 255, 255);
		App->render->DrawQuad({ 80, 15, 98, 34 }, 37, 37, 85, 255);
		App->render->TextDraw("Game", 95, 16, 255, 255, 255, 255, 16);
		App->render->TextDraw("Over", 95, 32, 255, 255, 255, 255, 16);
	}
	else if (v_WinLose > 200)
	{
		if (App->input->keys[SDL_SCANCODE_Z] == KEY_STATE::KEY_DOWN)
		{
			App->points->Reset();
			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
		}

		App->render->TextDraw("Press", 96, 146, 255, 255, 255, 255, 16);
		App->render->TextDraw("Start", 96, 177, 255, 255, 255, 255, 16);
		App->render->TextDraw("To", 128, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw("Continue", 79, 242, 255, 255, 255, 255, 16);

		App->render->TextDraw(ch_loseContinue, 141, 369, 255, 255, 255, 255, 16);

		if (v_WinLose % 50 == 0)
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

bool ScreenLvl_1::CleanUp()
{

	App->tetronimo->Disable();
	//App->render->ttfQuit();	// excepcio xd
	//App->audio->CleanUp();	// excepcio xd
	App->textures->Unload(bg_texture);
	App->textures->Unload(curtain_texture);
	return true;
}