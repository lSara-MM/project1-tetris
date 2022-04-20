#include "ScreenLvl_1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "ModuleInput.h"
#include "ModulePlayer.h"

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

	score = 0;
	lines = 0;
	linesObjective = 12;
	linesleft = linesObjective;

	lvl_credits = App->sStart->credits - 1;

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
	
	string s_score = std::to_string(score);
	const char* ch_score = s_score.c_str();

	string s_lines = std::to_string(lines);
	const char* ch_lines = s_lines.c_str();

	string s_linesleft = std::to_string(linesleft);
	const char* ch_linesleft = s_linesleft.c_str();

	string s_credits = to_string(lvl_credits);
	const char* ch_credits = s_credits.c_str();


	App->render->TextDraw("Score", 49, 433, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_score, 137, 433, 255, 0, 0, 255, 15);

	App->render->TextDraw("lines", 49, 451, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_lines, 137, 451, 255, 0, 0, 255, 15);


	////Complete x lines to win		// Appears when spawns first block (SONIA)
	//App->render->TextDraw("complete", 272, 210, 255, 255, 255, 255, 16);
	//App->render->TextDraw(ch_linesleft, 272, 242, 255, 255, 255, 255, 16);
	//App->render->TextDraw("lines", 320, 240, 252, 255, 255, 255, 16);
	//App->render->TextDraw("to go to", 272, 272, 255, 255, 255, 255, 16);
	//App->render->TextDraw("next round", 257, 305, 255, 255, 255, 255, 16);

	//Lines left
	if (linesleft > 0 && linesleft <= linesObjective && curtain.GetLoopCount() > 0) // Last condition is for the "Complete x lines to win"
	{
		App->render->TextDraw("0", 270, 225, 255, 0, 0, 255, 15);
		App->render->TextDraw(ch_linesleft, 285, 225, 255, 0, 0, 255, 15);
		App->render->TextDraw("lines", 305, 255, 255, 255, 255, 255, 16);
		App->render->TextDraw("left", 305, 285, 255, 255, 255, 255, 16);
	}

	
	if (linesleft == 0)
	{
		//You did it
		App->render->TextDraw("you", 305, 250, 255, 255, 255, 255, 16);
		App->render->TextDraw("did it", 290, 280, 255, 255, 255, 255, 15);

		////Bonus		// 
		//App->render->TextDraw("bonus for", 272, 210, 255, 255, 255, 255, 16);
		//App->render->TextDraw("low", 304, 227, 255, 255, 255, 255, 16);
		//App->render->TextDraw("puzzle", 288, 244, 255, 255, 255, 255, 16);
	}

	App->render->TextDraw("high score", 255, 370, 0, 0, 150, 255, 16);
	App->render->TextDraw("10000", 288, 385, 0, 0, 150, 255, 16);
	App->render->TextDraw("round", 255, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_lines, 353, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw("credits", 255, 450, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_credits, 385, 450, 0, 0, 150, 255, 16);

	App->render->TextDraw("next", 16, 25, 255, 0, 0, 255, 16);


	// Debugging
	if (App->player->instaLose == true)
	{
		LOG("Lost!");
	}

	return update_status::UPDATE_CONTINUE;
}
