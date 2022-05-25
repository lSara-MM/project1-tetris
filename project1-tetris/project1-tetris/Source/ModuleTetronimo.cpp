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
			tileSet[j][i].tetronimo = 0;
			//tileSetInt[j][i] = 0;
		}
	}



	combo = 0;

	App->player->godMode = false;

	LOG("Loading grid_texture");
	grid_texture = App->textures->Load("Assets/ss_grid.png");
	blockTexture = App->textures->Load("Assets/Sprites/ss_tetronimos.png");
	return true;
}


update_status ModuleTetronimo::Update() 
{
	runTime = SDL_GetTicks();
	deltaTime += runTime - lastTickTime;
	lastTickTime = runTime;

	//blockFall();

	if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN)
	{
		blockMovement(-1);
	}

	if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN)
	{
		blockMovement(1);
	}
	if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_DOWN)
	{
		blockFall();
	}

	Debugging();
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			blockUpdate(&tileSet[j][i]);
			tileSet[j][i].pSection = &tileSet[j][i].section;
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
				App->render->Blit(blockTexture, (65 + (j * (B_WIDTH + 1))), (51 + (i * (B_HEIGHT + 1))), tileSet[j][i].pSection);
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
	next = BLOCK_TYPE::RED;
	switch (next)
	{
	case BLOCK_TYPE::RED:
		tileSet[3][0].id = 1;
		tileSet[4][0].id = 2;
		tileSet[4][1].id = 2;
		tileSet[6][0].id = 3;

		b1 = &tileSet[3][0];
		b2 = &tileSet[4][0];
		b3 = &tileSet[4][1];
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

	b1->tetronimo++;
	b2->tetronimo++;
	b3->tetronimo++;
	b4->tetronimo++;

	//return rand() % 7 + 1;

	return BLOCK_TYPE::RED;
}

void ModuleTetronimo::blockUpdate(Block* block)
{

	switch (block->id)
	{
	case 0:
			//block->section = { 0, 0, 16, 16 };;
			break;
	case 1:
		block->section = { 0, 0, 16, 16 };
		break;

	case 2:
		block->section = { 17, 0, 16, 16 };
		break;

	case 3:
		block->section = { 34, 0, 16, 16 };
		break;

	case 4:
		//block->section = { 34, 0, 16, 16 };
		break;

	default:
		break;
	}

	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			tileSet[j][i].tileX = j;
			tileSet[j][i].tileY = i;
		}
	}

	block->bX = 65 + (block->tileX * (B_WIDTH + 1));
	block->bY = 51 + (block->tileY * (B_HEIGHT + 1));
}

bool ModuleTetronimo::blockFall()
{
	if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
	{
		// check border's - check if tile below is null OR if it's not, if the block is from the same tetronim, move.
		if ((b1->tileY + 1 < 22 && b2->tileY + 1 < 22 &&
			b3->tileY + 1 < 22 && b4->tileY + 1 < 22) &&
			(tileSet[b1->tileX][b1->tileY + 1].id == -1 || tileSet[b1->tileX][b1->tileY + 1].tetronimo == b1->tetronimo) &&
			(tileSet[b2->tileX][b2->tileY + 1].id == -1 || tileSet[b2->tileX][b2->tileY + 1].tetronimo == b2->tetronimo) &&
			(tileSet[b3->tileX][b3->tileY + 1].id == -1 || tileSet[b3->tileX][b3->tileY + 1].tetronimo == b3->tetronimo) &&
			(tileSet[b4->tileX][b4->tileY + 1].id == -1 || tileSet[b4->tileX][b4->tileY + 1].tetronimo == b4->tetronimo))
		{
			// save current block info
			var1 = *b1;
			var2 = *b2;
			var3 = *b3;
			var4 = *b4;

			// increment the Y position
			var1.tileY++;
			var2.tileY++;
			var3.tileY++;
			var4.tileY++;

			// make the block non printable 
			tileSet[b1->tileX][b1->tileY].id = -1;
			tileSet[b2->tileX][b2->tileY].id = -1;
			tileSet[b3->tileX][b3->tileY].id = -1;
			tileSet[b4->tileX][b4->tileY].id = -1;

			// fill the next block with the current block info
			tileSet[b1->tileX][var1.tileY] = var1;
			tileSet[b2->tileX][var2.tileY] = var2;
			tileSet[b3->tileX][var3.tileY] = var3;
			tileSet[b4->tileX][var4.tileY]= var4;

			// change the pointer's position
			b1 = &tileSet[b1->tileX][var1.tileY];
			b2 = &tileSet[b2->tileX][var2.tileY];
			b3 = &tileSet[b3->tileX][var3.tileY];
			b4 = &tileSet[b4->tileX][var4.tileY];

			//SDL_Delay(200);		// to change because it causes problems with movement X
			return true;
		}
		else
		{
			nextT = spawnTetronimo(nextT);
			return false;
		}
	}
}


void ModuleTetronimo::blockMovement(int p)
{
	if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
	{
		// check border's - check if next tile is null OR if it's not, if the block is from the same tetronim, move.
		if (b1->tileX + p > -1 && b1->tileX + p < 10 &&
			b2->tileX + p > -1 && b2->tileX + p < 10 &&
			b3->tileX + p > -1 && b3->tileX + p < 10 &&
			b4->tileX + p > -1 && b4->tileX + p < 10 &&	
			(tileSet[b1->tileX + p][b1->tileY].id == -1 || tileSet[b1->tileX + p][b1->tileY].tetronimo == b1->tetronimo) &&
			(tileSet[b2->tileX + p][b2->tileY].id == -1 || tileSet[b2->tileX + p][b2->tileY].tetronimo == b2->tetronimo) &&
			(tileSet[b3->tileX + p][b3->tileY].id == -1 || tileSet[b3->tileX + p][b3->tileY].tetronimo == b3->tetronimo) &&
			(tileSet[b4->tileX + p][b4->tileY].id == -1 || tileSet[b4->tileX + p][b4->tileY].tetronimo == b4->tetronimo))
		{
			// save current block info
			var1 = *b1;
			var2 = *b2;
			var3 = *b3;
			var4 = *b4;

			// increment / decrement X position
			var1.tileX += p;
			var2.tileX += p;
			var3.tileX += p;
			var4.tileX += p;
			
			// make the block non printable 
			tileSet[b1->tileX][b1->tileY].id = -1;
			tileSet[b2->tileX][b2->tileY].id = -1;
			tileSet[b3->tileX][b3->tileY].id = -1;
			tileSet[b4->tileX][b4->tileY].id = -1;
			
			// fill the next block with the current block info
			tileSet[var1.tileX][b1->tileY] = var1;
			tileSet[var2.tileX][b2->tileY] = var2;
			tileSet[var3.tileX][b3->tileY] = var3;
			tileSet[var4.tileX][b4->tileY] = var4;

			// change the pointer's position
			b1 = &tileSet[var1.tileX][b1->tileY];
			b2 = &tileSet[var2.tileX][b2->tileY];
			b3 = &tileSet[var3.tileX][b3->tileY];
			b4 = &tileSet[var4.tileX][b4->tileY];

			//SDL_Delay(100);		// to change 
		}
	}
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

		App->player->godMode = false; //Muy cutre, no he encontrado la manera de desactivar1lo con el mismo boton
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

	if (App->player->godMode == true)
	{
		App->render->Blit(grid_texture, 62, 50, NULL);
	}
}

