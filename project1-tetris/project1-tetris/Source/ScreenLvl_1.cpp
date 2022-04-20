#include "ScreenLvl_1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "ModuleInput.h"

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
	curtain.PushBack({ 166, 3, 157, 126 });
	curtain.PushBack({ 328, 3, 157, 126 });
	curtain.PushBack({ 491, 3, 157, 126 });
	curtain.PushBack({ 653, 3, 157, 126 });
	curtain.PushBack({ 816, 3, 157, 126 });

	curtain.speed = 0.2f;


}

ScreenLvl_1::~ScreenLvl_1()
{

}

// Load assets
bool ScreenLvl_1::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bg_texture = App->textures->Load("Assets/ss_easyBg.png");

	curtain_texture = App->textures->Load("Assets/curtain1.png");

	score = 0;
	lines = 0;
	linesleft = 12;


	return ret;
}

update_status ScreenLvl_1::Update()
{
	start_screen.Update();

	curtain.Update();


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
	}

	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenLvl_1::PostUpdate()
{
	App->render->Blit(bg_texture, 0, 10, NULL);

	//Curtain animation
	App->render->Blit(curtain_texture, 258, 194, &(curtain.GetCurrentFrame()), 0.85f);


	string s_score = std::to_string(score);
	const char* ch_score = s_score.c_str();


	string s_lines = std::to_string(lines);
	const char* ch_lines = s_lines.c_str();

	string s_linesleft = std::to_string(linesleft);
	const char* ch_linesleft = s_linesleft.c_str();

	App->render->TextDraw("Score", 49, 433, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_score, 137, 433, 255, 0, 0, 255, 15);

	App->render->TextDraw("lines", 49, 451, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_lines, 137, 451, 255, 0, 0, 255, 15);


	//Lines left
	/*App->render->TextDraw("0", 270, 225, 255, 0, 0, 255, 15);
	App->render->TextDraw(ch_linesleft, 285, 225, 255, 0, 0, 255, 15);
	App->render->TextDraw("lines", 305, 255, 255, 255, 255, 255, 16);
	App->render->TextDraw("left", 305, 285, 255, 255, 255, 255, 16);*/


	//You did it
	/*App->render->TextDraw("you", 305, 250, 255, 255, 255, 255, 16);
	App->render->TextDraw("did it", 290, 280, 255, 255, 255, 255, 15);*/


	//Bonus
	//App->render->TextDraw("bonus for", 272, 210, 255, 255, 255, 255, 16);
	//App->render->TextDraw("low", 304, 227, 255, 255, 255, 255, 16);
	//App->render->TextDraw("puzzle", 288, 244, 255, 255, 255, 255, 16);


	//Complete x lines to win
	App->render->TextDraw("complete", 272, 210, 255, 255, 255, 255, 16);
	App->render->TextDraw(ch_linesleft, 272, 242, 255, 255, 255, 255, 16);
	App->render->TextDraw("lines", 320, 240, 252, 255, 255, 255, 16);
	App->render->TextDraw("to go to", 272, 272, 255, 255, 255, 255, 16);
	App->render->TextDraw("next round", 257, 305, 255, 255, 255, 255, 16);



	App->render->TextDraw("high score", 255, 370, 0, 0, 150, 255, 16);
	App->render->TextDraw("68572", 288, 385, 0, 0, 150, 255, 16);
	App->render->TextDraw("round", 255, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_lines, 353, 418, 0, 0, 150, 255, 16);
	App->render->TextDraw("credits", 255, 450, 0, 0, 150, 255, 16);
	App->render->TextDraw(ch_linesleft, 385, 450, 0, 0, 150, 255, 16);

	App->render->TextDraw("next", 16, 25, 255, 0, 0, 255, 16);


	return update_status::UPDATE_CONTINUE;
}
