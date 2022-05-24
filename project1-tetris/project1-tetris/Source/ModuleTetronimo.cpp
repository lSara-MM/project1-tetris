#include "ModuleTetronimo.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ScreenLvl_1.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"

#include "Points.h"

#include <stdlib.h>
#include <time.h>

#include "SDL/include/SDL_scancode.h"
#include <sstream>

using namespace std;


//20 (22 spawn) y
//10 x
//pieza de spawn en x = 5
//pieza de spawn en y = 0

bool const r = (srand(time(NULL)), true);
int  num = rand() % 7;
int rotar = 0;

//The portions of the sprite map to be blitted
uint runTime = 0;
uint deltaTime = 0;
uint lastTickTime = 0;

ModuleTetronimo::ModuleTetronimo(bool startEnabled) : Module(startEnabled) 
{}


ModuleTetronimo::~ModuleTetronimo()
{
}


bool ModuleTetronimo::Start() 
{
	//Tile Set
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			tileSet[j][i].id = -1;
			tileSet[j][i].tileX = j;
			tileSet[j][i].tileY = i;
			//tileSetInt[j][i] = 0;
		}
	}

	
	combo = 0;

	App->player->godMode = false;

	LOG("Loading grid_texture");
	grid_texture = App->textures->Load("Assets/ss_grid.png");

	return true;
}


update_status ModuleTetronimo::Update() 
{

	runTime = SDL_GetTicks();
	deltaTime += runTime - lastTickTime;
	lastTickTime = runTime;

	nextT = spawnTetronimo(nextT);
	Debugging();
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			blockUpdate(&tileSet[j][i]);
		}
	}
	

	return update_status::UPDATE_CONTINUE;
}


update_status ModuleTetronimo::PostUpdate()
{
	//Tile Set
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (tileSet[j][i].id != -1)
			{
				App->render->Blit(blockTexture, (65 + (j * (B_WIDTH + 1))), (51 + (i * (B_HEIGHT + 1))), tileSet[j][i].section);
			}
		}
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleTetronimo::CleanUp()
{
	return true;
}

int ModuleTetronimo::spawnTetronimo(int next)
{
	switch (next)
	{
	case BLOCK_TYPE::RED:
		tileSet[3][0].id = 1;
		tileSet[4][0].id = 2;
		tileSet[5][0].id = 2;
		tileSet[6][0].id = 3;

		b1 = &tileSet[3][0];
		b2 = &tileSet[4][0];
		b3 = &tileSet[5][0];
		b4 = &tileSet[6][0];

		break;

		/*case BLOCK_TYPE::GREEN:
			tileSet[3][0].id = 1;
			tileSet[4][0].id = 2;
			tileSet[5][0].id = 2;
			tileSet[6][0].id = 3;
			break;

		case BLOCK_TYPE::BLUE:
			tileSet[3][0].id = 1;
			tileSet[4][0].id = 2;
			tileSet[5][0].id = 2;
			tileSet[6][0].id = 3;
			break;

		case BLOCK_TYPE::YELLOW:
			tileSet[3][0].id = 1;
			tileSet[4][0].id = 2;
			tileSet[5][0].id = 2;
			tileSet[6][0].id = 3;
			break;

		case BLOCK_TYPE::PINK:
			tileSet[3][0].id = 1;
			tileSet[4][0].id = 2;
			tileSet[5][0].id = 2;
			tileSet[6][0].id = 3;
			break;

		case BLOCK_TYPE::CYAN:
			tileSet[3][0].id = 1;
			tileSet[4][0].id = 2;
			tileSet[5][0].id = 2;
			tileSet[6][0].id = 3;
			break;

		case BLOCK_TYPE::ORANGE:
			tileSet[3][0].id = 1;
			tileSet[4][0].id = 2;
			tileSet[5][0].id = 2;
			tileSet[6][0].id = 3;
			break;*/
	default:
		break;
	}

	//return rand() % 7 + 1;

	return BLOCK_TYPE::RED;
}

void ModuleTetronimo::blockUpdate(Block* block)
{

	switch (block->id)
	{
	case 0:
		//block->section = ;
	default:
		break;
	}


	block->bX = 65 + (block->tileX * (B_WIDTH + 1));
	block->bY = 51 + (block->tileY * (B_HEIGHT + 1));
}

bool ModuleTetronimo::blockFall()
{
	if (b1->tileY + 1 < 21 && b2->tileY + 1 < 21 &&
		b3->tileY + 1 < 21 && b4->tileY + 1 < 21)
	{
		if (tileSet[b1->tileX][b1->tileY + 1].id == -1 &&
			tileSet[b2->tileX][b2->tileY + 1].id == -1 &&
			tileSet[b3->tileX][b3->tileY + 1].id == -1 &&
			tileSet[b4->tileX][b4->tileY + 1].id == -1)
		{
			tileSet[b1->tileX][b1->tileY].id = -1;
			tileSet[b2->tileX][b2->tileY].id = -1;
			tileSet[b3->tileX][b3->tileY].id = -1;
			tileSet[b4->tileX][b4->tileY].id = -1;

			b1->tileY++;
			b2->tileY++;
			b3->tileY++;
			b4->tileY++;

			SDL_Delay(100);
			return true;
		}
	}
	else
		return false;
}

void ModuleTetronimo::Debugging()
{

	if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN) //(KEY_REPEAT)		// to fix somehow
	{
		(App->player->godMode == false) ? App->player->godMode = true : App->player->godMode = false;
		/*App->player->godMode != App->player->godMode;*/

		//App->player->godMode = true;	
	}

	if (App->input->keys[SDL_SCANCODE_F6] == KEY_DOWN)	//	just in case		
	{

		App->player->godMode = false; //Muy cutre, no he encontrado la manera de desactivarlo con el mismo boton
	}

	// Manually spawn a block
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN)
	{

	}

	//// Instant win
	//if (App->input->keys[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN) {
	//	lvl_instaWin = true;
	//	App->sLvl_1->v_WinLose = 0;
	//}

	//// Instant lose
	//if (App->input->keys[SDL_SCANCODE_F4] == KEY_STATE::KEY_DOWN) {
	//	lvl_instaLose = true;
	//	App->sLvl_1->v_WinLose = 0;
	//}

}

