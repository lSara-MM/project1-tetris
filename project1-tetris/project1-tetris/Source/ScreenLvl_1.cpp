#include "ScreenLvl_1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"

#include <iostream>
using namespace std;
#include <sstream>


//#include "ModuleFadeToBlack.h"

ScreenLvl_1::ScreenLvl_1(bool startEnabled) : Module(startEnabled)
{
	curtain.PushBack({ 816, 3, 157, 126 });
	curtain.PushBack({ 653, 3, 157, 126 });
	curtain.PushBack({ 491, 3, 157, 126 });
	curtain.PushBack({ 328, 3, 157, 126 });
	curtain.PushBack({ 166, 3, 157, 126 });
	curtain.PushBack({ 3, 3, 157, 126 });

	/*curtain.PushBack({ 166, 3, 157, 126 });
	curtain.PushBack({ 328, 3, 157, 126 });
	curtain.PushBack({ 491, 3, 157, 126 });
	curtain.PushBack({ 653, 3, 157, 126 });
	curtain.PushBack({ 816, 3, 157, 126 });*/

	curtain.speed = 0.2f;
	curtain.loop = true;
}

ScreenLvl_1::~ScreenLvl_1()
{

}

// Load assets
bool ScreenLvl_1::Start()
{
	
	App->collisions->Enable();
	//App->player->Enable();		// dona error nose perque, "Exception thrown: read access violation. **this** was nullptr."


	LOG("Loading lvl 1 background assets");
	LOG("Loading curtain assets\n");

	bool ret = true;

	bg_texture = App->textures->Load("Assets/ss_easyBg.png");
	curtain_texture = App->textures->Load("Assets/curtain.png");

	LOG("Loading background music: Loginska");
	App->audio->PlayMusic("Assets/loginska.ogg", 1.0f);

	LOG("Loading sound effects");
	fxBlock_Fall = App->audio->LoadFx("Assets/Audio/FX/block_fall.wav");
	fxLine = App->audio->LoadFx("Assets/Audio/FX/line.wav");

	// fxLvl = load different depending on what is needed (save space?)
	fxBlock_Fall = App->audio->LoadFx("Assets/Audio/FX/block_fall.wav");
	fxYou_DidIt = App->audio->LoadFx("Assets/Audio/FX/you_did_it.wav");
	fxClearing_Bars = App->audio->LoadFx("Assets/Audio/FX/clearing_bars.wav");
	fxGameOver = App->audio->LoadFx("Assets/Audio/FX/gameover.wav");
	fxLine = App->audio->LoadFx("Assets/Audio/FX/line.wav"); 

	score = 0;
	lines = 0;
	linesObjective = 12;
	linesleft = linesObjective;

	lvl_credits = App->sStart->credits - 1;

	v_message = 0;
	v_insertCoin = 0;
	return ret;
}

update_status ScreenLvl_1::Update()
{

	if (curtain.GetLoopCount() == 1) { curtain.speed = 0; }
	else { curtain.Update(); }
	
	if (App->input->keys[SDL_SCANCODE_P] == KEY_STATE::KEY_REPEAT)
	{
		score++;
	}


	if (App->input->keys[SDL_SCANCODE_L] == KEY_STATE::KEY_REPEAT)
	{
		lines++;
	}

	if (App->input->keys[SDL_SCANCODE_L] == KEY_STATE::KEY_DOWN)
	{
		linesleft--;
		LOG("Lines left: %d", linesleft);
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		if (lvl_credits < 9) { lvl_credits++; }
		LOG("Curtain loop count: %d", curtain.GetLoopCount());
	}



	// Debugging
	if (App->input->keys[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN) {
		lvl_instaWin = true;
		v_message = 0;
	}

	if (App->input->keys[SDL_SCANCODE_F4] == KEY_STATE::KEY_DOWN) {
		lvl_instaLose = true;
		v_message = 0;
	}
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenLvl_1::PostUpdate()
{
	App->render->Blit(bg_texture, 0, 10, NULL);
	
	//Curtain animation
	if (curtain.GetLoopCount() == 0)
	{
		App->render->Blit(curtain_texture, 258, 194, &(curtain.GetCurrentFrame()), 0.85f);
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
	App->render->TextDraw("next", 16, 25, 255, 0, 0, 255, 16);

	App->render->TextDraw("Score", 49, 433, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_score, 137, 433, 255, 0, 0, 255, 15);

	App->render->TextDraw("lines", 49, 451, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_lines, 137, 451, 255, 0, 0, 255, 15);


	////Complete x lines to win		
	//App->render->TextDraw("complete", 272, 210, 255, 255, 255, 255, 16);
	//App->render->TextDraw(ch_linesleft, 272, 242, 255, 255, 255, 255, 16);
	//App->render->TextDraw("lines", 320, 240, 252, 255, 255, 255, 16);
	//App->render->TextDraw("to go to", 272, 272, 255, 255, 255, 255, 16);
	//App->render->TextDraw("next round", 257, 305, 255, 255, 255, 255, 16);


	// Mid tower section
	//Complete x lines to win
	if (curtain.GetLoopCount() > 0 && v_message == 0)
	{
		App->render->TextDraw("complete", 272, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw(ch_linesleft, 272, 242, 255, 255, 255, 255, 16);
		App->render->TextDraw("lines", 320, 240, 252, 255, 255, 255, 16);
		App->render->TextDraw("to go to", 272, 272, 255, 255, 255, 255, 16);
		App->render->TextDraw("next round", 257, 305, 255, 255, 255, 255, 16);
	}
	LOG("curtain loop %d", curtain.GetLoopCount());

	//Lines left
	if (linesleft > 0 && linesleft <= linesObjective) // Last condition is for the "Complete x lines to win"
	// Appears when spawns first block (SONIA)
	{
		App->render->TextDraw("0", 270, 225, 255, 0, 0, 255, 15);
		App->render->TextDraw(ch_linesleft, 285, 225, 255, 0, 0, 255, 15);
		App->render->TextDraw("lines", 305, 255, 255, 255, 255, 255, 16);
		App->render->TextDraw("left", 305, 285, 255, 255, 255, 255, 16);
	}

	//if (linesleft == 0)
	//{
	//	//You did it
	//	App->render->TextDraw("you", 305, 250, 255, 255, 255, 255, 16);
	//	App->render->TextDraw("did it", 290, 280, 255, 255, 255, 255, 15);
	//	////Bonus
	//	//App->render->TextDraw("bonus for", 272, 210, 255, 255, 255, 255, 16);
	//	//App->render->TextDraw("low", 304, 227, 255, 255, 255, 255, 16);
	//	//App->render->TextDraw("puzzle", 288, 244, 255, 255, 255, 255, 16);
	//}

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
	

	// Win conditions

	if (linesleft == 0)
	{
		lvl_win();
	}

	// Debugging
	/*if (App->player->instaLose == true)
	{
		LOG("Instant Lost!");
		lvl_lose();
	}*/
	if (lvl_instaLose == true)
	{
		LOG("Instant Lost!");
		lvl_lose();
	}
	
	/*if (App->player->instaWin == true)
	{
		LOG("Level complete (Instant Win)!");
		lvl_win();
	}*/
	if (lvl_instaWin == true)
	{
		LOG("Level complete (Instant Win)!");

		lvl_win();
	}

	return update_status::UPDATE_CONTINUE;
}

void ScreenLvl_1::lvl_win()
{
	if (v_message >= 0 && v_message < 100)
	{
		//You did it
		App->render->TextDraw("you", 305, 250, 255, 255, 255, 255, 16);
		App->render->TextDraw("did it", 290, 280, 255, 255, 255, 255, 15);
		App->audio->PlayFx(fxYou_DidIt, 0);
	}
	if (v_message >= 200)
	{
		//Bonus
		App->render->TextDraw("bonus for", 272, 210, 255, 255, 255, 255, 16);
		App->render->TextDraw("low", 304, 227, 255, 255, 255, 255, 16);
		App->render->TextDraw("puzzle", 288, 244, 255, 255, 255, 255, 16);
	}

	//LOG("v_message %d", v_message);
	v_message++;

	if (v_message == 300) 
	{ 
		lvl_instaWin = false; 
		App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
	}
}

void ScreenLvl_1::lvl_lose()
{
	// Game Over
	App->audio->PlayFx(fxGameOver, 0);
	if (v_message >= 0 && v_message < 100)
	{
		App->render->DrawQuad({ 63, 0, 131, 66 }, 255, 0, 0, 255);
		App->render->DrawQuad({ 70, 5, 118, 62 }, 0, 0, 255, 255);
		App->render->DrawQuad({ 80, 15, 98, 34 }, 37, 37, 85, 255);
		App->render->TextDraw("Game", 95, 16, 255, 255, 255, 255, 16);
		App->render->TextDraw("Over", 95, 32, 255, 255, 255, 255, 16);

		v_message++;
	}
	else { lvl_instaLose = false; }
	
	
	
}
