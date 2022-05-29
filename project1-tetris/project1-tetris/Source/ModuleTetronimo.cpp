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

			//tileSet[j][i].tetronimo = 0;
			//tileSetInt[j][i] = 0;
		}
	}


	rotation = 0;
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

	if (deltaTime > 700)
	{
		blockFall();
		deltaTime = 0;
	}

	if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
	{
		if (deltaTime > 25)
		{
			blockMovement(-1);
			deltaTime = 0;
		}
	}

	if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
	{
		if (deltaTime > 25)
		{
			blockMovement(1);
			deltaTime = 0;
		}
	}
	if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	{
		if (deltaTime > 25)
		{
			blockFall();
			deltaTime = 0;
		}

		/*v = b1;
		blockFall(v);*/
		/*b1 = v;*/
		//b1->id = blockFall(b1);
		///*blockFall(b2);
		//blockFall(b3);
		//blockFall(b4);*/
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

	for (int i = 0; i < 22; i++)
	{
		if (lineCheck(i) == true)
		{
			deleteLine(i);
			// put a function to delete and do the animations of line
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
	//a

	int  tetronimo = rand() % 7;

	if (tetronimo == 0) {
		next = BLOCK_TYPE::RED;
	}

	else if (tetronimo == 1) {
		next = BLOCK_TYPE::GREEN;
	}

	else if (tetronimo == 2) {
		next = BLOCK_TYPE::BLUE;
	}

	else if (tetronimo == 3) {
		next = BLOCK_TYPE::YELLOW;
	}

	else if (tetronimo == 4) {
		next = BLOCK_TYPE::PINK;
	}

	else if (tetronimo == 5) {
		next = BLOCK_TYPE::CYAN;
	}

	else if (tetronimo == 6) {
		next = BLOCK_TYPE::ORANGE;
	}

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

	case BLOCK_TYPE::GREEN:
		tileSet[3][0].id = 11;
		tileSet[4][0].id = 12;
		tileSet[5][0].id = 13;
		tileSet[4][1].id = 14;

		b1 = &tileSet[3][0];
		b2 = &tileSet[4][0];
		b3 = &tileSet[5][0];
		b4 = &tileSet[4][1];

		break;

	case BLOCK_TYPE::BLUE:
		tileSet[3][0].id = 31;
		tileSet[4][0].id = 32;
		tileSet[3][1].id = 33;
		tileSet[4][1].id = 34;

		b1 = &tileSet[3][0];
		b2 = &tileSet[4][0];
		b3 = &tileSet[3][1];
		b4 = &tileSet[4][1];

		break;

	case BLOCK_TYPE::YELLOW:
		tileSet[3][0].id = 41;
		tileSet[4][0].id = 42;
		tileSet[5][0].id = 43;
		tileSet[5][1].id = 44;

		b1 = &tileSet[3][0];
		b2 = &tileSet[4][0];
		b3 = &tileSet[5][0];
		b4 = &tileSet[5][1];

		break;

	case BLOCK_TYPE::PINK:
		tileSet[3][0].id = 51;
		tileSet[4][0].id = 52;
		tileSet[5][0].id = 53;
		tileSet[3][1].id = 54;

		b1 = &tileSet[3][0];
		b2 = &tileSet[4][0];
		b3 = &tileSet[5][0];
		b4 = &tileSet[3][1];

		break;

	case BLOCK_TYPE::CYAN:
		tileSet[3][1].id = 71;
		tileSet[4][1].id = 72;
		tileSet[4][0].id = 73;
		tileSet[5][0].id = 74;

		b1 = &tileSet[3][1];
		b2 = &tileSet[4][1];
		b3 = &tileSet[4][0];
		b4 = &tileSet[5][0];

		break;

	case BLOCK_TYPE::ORANGE:
		tileSet[3][0].id = 81;
		tileSet[4][0].id = 82;
		tileSet[4][1].id = 83;
		tileSet[5][1].id = 84;

		b1 = &tileSet[3][0];
		b2 = &tileSet[4][0];
		b3 = &tileSet[4][1];
		b4 = &tileSet[5][1];

		break;
	default:
		break;
	}

	b1->tetronimo++;
	b2->tetronimo++;
	b3->tetronimo++;
	b4->tetronimo++;

	//return rand() % 7 + 1;


	//if (tetronimo == 0) {
	return BLOCK_TYPE::RED;
	//}

	//else if (tetronimo == 1) {
	return BLOCK_TYPE::GREEN;
	//}

	//else if (tetronimo == 2) {
	return BLOCK_TYPE::BLUE;
	//}

	//else if (tetronimo == 3) {
	return BLOCK_TYPE::YELLOW;
	//}

	//else if (tetronimo == 4) {
	return BLOCK_TYPE::PINK;
	//}

	//else if (tetronimo == 5) {
	return BLOCK_TYPE::CYAN;
	//}

	//else if (tetronimo == 6) {
	return BLOCK_TYPE::ORANGE;
	//}
}

void ModuleTetronimo::blockUpdate(Block* block)
{
	switch (block->id)
	{
		//red

	case 0:
		block->section = { 238, 0, 16, 16 };
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
		block->section = { 51, 0, 16, 16 };
		break;

	case 5:
		block->section = { 68, 0, 16, 16 };
		break;

	case 6:
		block->section = { 85, 0, 16, 16 };
		break;

		//green

	case 10:
		block->section = { 238, 17, 16, 16 };
		break;

	case 11:
		block->section = { 0, 17, 16, 16 };
		break;

	case 12:
		block->section = { 136, 17, 16, 16 };
		break;

	case 13:
		block->section = { 34, 17, 16, 16 };
		break;

	case 14:
		block->section = { 85, 17, 16, 16 };
		break;

	case 15:
		block->section = { 51, 17, 16, 16 };
		break;

	case 16:
		block->section = { 119, 17, 16, 16 };
		break;

	case 17:
		block->section = { 102, 17, 16, 16 };
		break;

	case 18:
		block->section = { 153, 17, 16, 16 };
		break;

	case 19:
		block->section = { 17, 17, 16, 16 };
		break;

	case 20:
		block->section = { 204, 17, 16, 16 };
		break;

	case 21:
		block->section = { 170, 17, 16, 16 };
		break;

	case 22:
		block->section = { 187, 17, 16, 16 };
		break;

	case 23:
		block->section = { 221, 17, 16, 16 };
		break;

		//blue

	case 31:
		block->section = { 170, 34, 16, 16 };
		break;

	case 32:
		block->section = { 221, 34, 16, 16 };
		break;

	case 33:
		block->section = { 204, 34, 16, 16 };
		break;

	case 34:
		block->section = { 187, 34, 16, 16 };
		break;

	case 35:
		block->section = { 0, 34, 16, 16 };
		break;

	case 36:
		block->section = { 34, 34, 16, 16 };
		break;

		//yellow

	case 40:
		block->section = { 238, 51, 16, 16 };
		break;

	case 41:
		block->section = { 0, 51, 16, 16 };
		break;

	case 42:
		block->section = { 17, 51, 16, 16 };
		break;

	case 43:
		block->section = { 221, 51, 16, 16 };
		break;

	case 44:
		block->section = { 85, 51, 16, 16 };
		break;

	case 45:
		block->section = { 170, 51, 16, 16 };
		break;

	case 46:
		block->section = { 68, 51, 16, 16 };
		break;

	case 47:
		block->section = { 34, 51, 16, 16 };
		break;

	case 48:
		block->section = { 51, 51, 16, 16 };
		break;

	case 49:
		block->section = { 204, 51, 16, 16 };
		break;

	case 50:
		block->section = { 187, 51, 16, 16 };
		break;

		//pink

	case 51:
		block->section = { 170, 68, 16, 16 };
		break;

	case 52:
		block->section = { 17, 68, 16, 16 };
		break;

	case 53:
		block->section = { 34, 68, 16, 16 };
		break;

	case 54:
		block->section = { 85, 68, 16, 16 };
		break;

	case 55:
		block->section = { 51, 68, 16, 16 };
		break;

	case 56:
		block->section = { 68, 68, 16, 16 };
		break;

	case 57:
		block->section = { 204, 68, 16, 16 };
		break;

	case 58:
		block->section = { 0, 68, 16, 16 };
		break;

	case 59:
		block->section = { 187, 68, 16, 16 };
		break;

	case 60:
		block->section = { 221, 68, 16, 16 };
		break;

	case 61:
		block->section = { 238, 68, 16, 16 };
		break;

		//sky

	case 70:
		block->section = { 238, 85, 16, 16 };
		break;

	case 71:
		block->section = { 0, 85, 16, 16 };
		break;

	case 72:
		block->section = { 187, 85, 16, 16 };
		break;

	case 73:
		block->section = { 170, 85, 16, 16 };
		break;

	case 74:
		block->section = { 34, 85, 16, 16 };
		break;

	case 75:
		block->section = { 51, 85, 16, 16 };
		break;

	case 76:
		block->section = { 204, 85, 16, 16 };
		break;

	case 77:
		block->section = { 221, 85, 16, 16 };
		break;

	case 78:
		block->section = { 85, 85, 16, 16 };
		break;

		//orange

	case 80:
		block->section = { 138, 102, 16, 16 };
		break;

	case 81:
		block->section = { 0, 102, 16, 16 };
		break;

	case 82:
		block->section = { 221, 102, 16, 16 };
		break;

	case 83:
		block->section = { 204, 102, 16, 16 };
		break;

	case 84:
		block->section = { 34, 102, 16, 16 };
		break;

	case 85:
		block->section = { 51, 102, 16, 16 };
		break;

	case 86:
		block->section = { 187, 102, 16, 16 };
		break;

	case 87:
		block->section = { 170, 102, 16, 16 };
		break;

	case 88:
		block->section = { 85, 102, 16, 16 };
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
			tileSet[b4->tileX][var4.tileY] = var4;

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

bool ModuleTetronimo::blockFall(Block* block)	// idk perque no me dixa ferho per a un block individual
{
	if (block != nullptr)
	{
		if ((block->tileY + 1 < 22) &&
			(tileSet[block->tileX][block->tileY + 1].id == -1 ||
				tileSet[block->tileX][block->tileY + 1].tetronimo == block->tetronimo))
		{
			var1 = *block;
			var1.tileY++;
			tileSet[block->tileX][block->tileY].id = -1;
			tileSet[block->tileX][var1.tileY] = var1;
			block = &tileSet[block->tileX][var1.tileY];
		}
		return true;
	}
	else
	{
		nextT = spawnTetronimo(nextT);
		return false;
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

void ModuleTetronimo::Rotation(int num)
{
	rotation;
	int stop = 0;

	switch (num)
	{
	case BLOCK_TYPE::RED:
		break;
	default:
		break;
	}
}

bool ModuleTetronimo::lineCheck(int i)
{
	for (int j = 0; j < 10; j++)
	{
		if (tileSet[j][i].id == -1)
		{
			return false;
		}
	}
	return true;
}

bool ModuleTetronimo::deleteLine(int i)		// probably doesnt work yet
{
	for (int j = 0; j < 10; j++)
	{
		tileSet[j][i].id = -1;
	}

	// do a function to make blocks fall once
	v = &tileSet[0][21];
	for (int i = 22; i > 2; i--)
	{
		for (int j = 0; j < 10; j++)
		{

			if ((v->tileY + 1 < 22 && b2->tileY + 1 < 22) &&
				(tileSet[v->tileX][v->tileY + 1].id == -1 ||
					tileSet[v->tileX][v->tileY + 1].tetronimo == v->tetronimo))
			{
				var1 = *v;
				var1.tileY++;
				tileSet[v->tileX][v->tileY].id = -1;
				tileSet[v->tileX][var1.tileY] = var1;
				v = &tileSet[v->tileX][var1.tileY];
			}
		}
	}

	return true;
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

		App->player->godMode = false;
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