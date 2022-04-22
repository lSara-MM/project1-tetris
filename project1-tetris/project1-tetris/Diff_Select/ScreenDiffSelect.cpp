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
#include <sstream>


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

	bg_texture = App->textures->Load("Assets/ss_DifficultyBg.png");
	arrowleft_texture= App->textures->Load("Assets/arrow_left.png");
	arrowright_texture = App->textures->Load("Assets/arrow_right.png");


	green1_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_green_1.png");
	pink1_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_pink_1.png");
	sky1_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_sky_1.png");

	sky_block_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_sky_Block.png");
	pink_block_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_pink_Block.png");
	orange_block_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_orange_Block.png");
	green_block_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_green_Block.png");
	yellow_block_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_yellow_Block.png");


	yellow4_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_yellow_4.png");
	orange2_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_orange_2.png");
	gray_block_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_gray_Block.png");
	pink3_texture = App->textures->Load("Assets/Sprites/Tetramino/SelectDiff/tetra_pink_3.png");

	fxAdd_Press_L_R = App->audio->LoadFx("Assets/Audio/FX/diff_selection_arrow.wav");
	fxAdd_PressEnter= App->audio->LoadFx("Assets/Audio/FX/diff_selection_enter.wav");
	


	return ret;
}


update_status ScreenDiffSelect::Update()
{
	start_screen.Update();

	if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN)
	{
	/*	App->render->Blit(arrowleft_texture, 64, 64, NULL);
		App->render->Blit(arrowright_texture, 144, 64, NULL);*/
		App->audio->PlayFx(fxAdd_Press_L_R);

	}

	if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN)
	{

		App->audio->PlayFx(fxAdd_Press_L_R);

	}
	

	if (App->input->keys[SDL_SCANCODE_RETURN] == KEY_STATE::KEY_DOWN)
	{
		App->audio->PlayFx(fxAdd_PressEnter);
		App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
	}

	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenDiffSelect::PostUpdate()
{


	App->render->Blit(bg_texture, 0, 10, NULL);
	
	App->render->Blit(arrowleft_texture, 64, 64, NULL);
	App->render->Blit(arrowleft_texture, 274, 64, NULL);
	App->render->Blit(arrowleft_texture, 510, 64, NULL);


	App->render->Blit(arrowright_texture, 144, 64, NULL);
	App->render->Blit(arrowright_texture, 388, 64, NULL);
	App->render->Blit(arrowright_texture, 589, 64, NULL);


	App->render->Blit(green1_texture, 99, 142, NULL);

	App->render->Blit(pink1_texture, 322, 122, NULL);

	App->render->Blit(sky1_texture, 530, 142, NULL);

	
	// middle left

	App->render->Blit(orange_block_texture, 258, 270, NULL);
	
	App->render->Blit(green_block_texture, 258, 287, NULL);
	
	App->render->Blit(yellow_block_texture, 258, 304, NULL);

	App->render->Blit(sky_block_texture, 258, 321, NULL);

	App->render->Blit(pink_block_texture, 258, 338, NULL);

	App->render->Blit(orange_block_texture, 258, 355, NULL);
	
	App->render->Blit(green_block_texture, 258, 372, NULL);

	App->render->Blit(yellow_block_texture, 258, 389, NULL);


	// middle right

	App->render->Blit(orange_block_texture, 401, 270, NULL);

	App->render->Blit(green_block_texture, 401, 287, NULL);

	App->render->Blit(yellow_block_texture, 401, 304, NULL);

	App->render->Blit(sky_block_texture, 401, 321, NULL);

	App->render->Blit(pink_block_texture, 401, 338, NULL);

	App->render->Blit(orange_block_texture, 401, 355, NULL);

	App->render->Blit(green_block_texture, 401, 372, NULL);

	App->render->Blit(yellow_block_texture, 401, 389, NULL);

	// left 

	App->render->Blit(green1_texture, 482, 373, NULL);

	App->render->Blit(yellow4_texture, 551, 357, NULL);

	App->render->Blit(orange2_texture, 581, 357, NULL);

	App->render->Blit(pink3_texture, 596, 375, NULL);

	App->render->Blit(gray_block_texture, 483, 373, NULL);

	App->render->Blit(gray_block_texture, 550, 374, NULL); 

	App->render->Blit(gray_block_texture, 627, 358, NULL);


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
	App->render->TextDraw("40000  BONUS", 464, 448, 255, 255, 255, 255, 15);


	return update_status::UPDATE_CONTINUE;
}
