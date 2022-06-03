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
bool const r = (srand(time(NULL)), true);
int rotar = 0;

//The portions of the sprite map to be blitted
uint runTime = 0;
uint deltaTime = 0;
uint deltaTimeJoysticks = 0;
uint lastTickTime = 0;

ModuleTetronimo::ModuleTetronimo(bool startEnabled) : Module(startEnabled)
{}


ModuleTetronimo::~ModuleTetronimo()
{}

bool ModuleTetronimo::Start()
{
	//Tile Set
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			tileSet[j][i].id = -1;
			tileSet[j][i].tetronimo = 0;
		}
	}

	// lvls
	switch (App->points->lvl)
	{
	case 4:
		tileSet[0][14].id = 0;
		tileSet[0][15].id = 30;
		tileSet[0][16].id = 61;
		tileSet[0][17].id = 80;
		tileSet[0][18].id = 10;
		tileSet[0][19].id = 40;
		tileSet[0][20].id = 70;
		tileSet[0][21].id = 10;

		tileSet[9][14].id = 10;
		tileSet[9][15].id = 40;
		tileSet[9][16].id = 70;
		tileSet[9][17].id = 0;
		tileSet[9][18].id = 30;
		tileSet[9][19].id = 61;
		tileSet[9][20].id = 80;
		tileSet[9][21].id = 10;
		break;
	case 5:
		tileSet[0][17].id = 0;
		tileSet[4][17].id = 10;
		tileSet[2][18].id = 30;
		tileSet[6][18].id = 40;
		tileSet[7][18].id = 61;
		tileSet[5][19].id = 70;
		tileSet[3][20].id = 80;
		tileSet[8][20].id = 0;
		tileSet[1][21].id = 10;
		tileSet[7][21].id = 30;
		break;
	case 6:
		tileSet[0][21].id = 30;
		tileSet[1][21].id = 40;
		tileSet[1][20].id = 40;
		tileSet[2][20].id = 61;
		tileSet[2][19].id = 80;
		tileSet[3][19].id = 0;
		tileSet[3][21].id = 61;
		tileSet[3][18].id = 30;
		tileSet[4][18].id = 40;
		tileSet[4][17].id = 0;
		tileSet[4][20].id = 70;
		tileSet[4][21].id = 70;

		tileSet[5][17].id = 10;
		tileSet[5][18].id = 61;
		tileSet[5][20].id = 80;
		tileSet[5][21].id = 80;
		tileSet[6][18].id = 70;
		tileSet[6][19].id = 10;
		tileSet[6][21].id = 0;
		tileSet[7][19].id = 30;
		tileSet[7][20].id = 0;
		tileSet[8][20].id = 10;
		tileSet[8][21].id = 10;
		tileSet[9][21].id = 30;
	default:
		break;
	}

	nextT = rand() % 7;
	rotation = 0;
	v_lines = 0;
	v_4L = 0;
	t = 0;
	pause = false;
	App->player->godMode = false;

	LOG("Loading grid_texture");
	grid_texture = App->textures->Load("Assets/ss_grid.png");
	blockTexture = App->textures->Load("Assets/Sprites/ss_tetronimos.png");

	return true;
}

float reduce_val(float v1, float min, float clamp_to) {
	float sign = v1 / fabs(v1);
	float reduced = v1 - ((fabs(v1) > min) ? sign * min : v1);
	float to_1 = reduced / (float)(SDL_MAX_SINT16);
	float reclamped = to_1 * clamp_to;
	return reclamped;
}

update_status ModuleTetronimo::Update()
{
	runTime = SDL_GetTicks();
	deltaTime += runTime - lastTickTime;
	deltaTimeJoysticks += runTime - lastTickTime;
	lastTickTime = runTime;

	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		if (App->input->pads[0].buttons[i] == KEY_DOWN)
		{
			button_press = true; break;
		}
	fx += reduce_val(App->input->pads[0].left_x, 3000, 2);
	fy += reduce_val(App->input->pads[0].left_y, 3000, 2);
	fx += reduce_val(App->input->pads[0].right_x, 3000, 2);
	fy += reduce_val(App->input->pads[0].right_y, 3000, 2);

	if (pause == false)
	{
		if (deltaTime > 700)
		{
			blockFall(1);
			deltaTime = 0;
		}
		if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN || fx < 0 && App->input->num_controllers > 0)
		{
			if (deltaTimeJoysticks > 60)
			{
				blockMoveX(-1);
				deltaTimeJoysticks = 0;
			}
		}
		if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN || fx > 0 && App->input->num_controllers > 0)
		{
			if (deltaTimeJoysticks > 60)
			{
				blockMoveX(1);
				deltaTimeJoysticks = 0;
			}
		}
		if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT || fy > 0 && App->input->num_controllers > 0)
		{
			if (deltaTimeJoysticks > 25)
			{
				blockFall(1);
				deltaTimeJoysticks = 0;
			}
		}
		if (App->input->keys[SDL_SCANCODE_R] == KEY_STATE::KEY_DOWN || button_press)
		{
			Rotation(currentT);
		}
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

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			showcase[j][i].id = -1;
		}
	}
	switch (nextT)
	{
	case BLOCK_TYPE::RED:
		showcase[0][0].id = 1;
		showcase[1][0].id = 2;
		showcase[2][0].id = 2;
		showcase[3][0].id = 3;
		break;
	case BLOCK_TYPE::GREEN:
		showcase[0][0].id = 11;
		showcase[1][0].id = 12;
		showcase[2][0].id = 13;
		showcase[1][1].id = 14;
		break;
	case BLOCK_TYPE::BLUE:
		showcase[0][0].id = 31;
		showcase[1][0].id = 32;
		showcase[0][1].id = 33;
		showcase[1][1].id = 34;
		break;
	case BLOCK_TYPE::YELLOW:
		showcase[0][0].id = 41;
		showcase[1][0].id = 42;
		showcase[2][0].id = 43;
		showcase[2][1].id = 44;
		break;
	case BLOCK_TYPE::PINK:
		showcase[0][0].id = 51;
		showcase[1][0].id = 52;
		showcase[2][0].id = 53;
		showcase[0][1].id = 54;
		break;
	case BLOCK_TYPE::CYAN:
		showcase[1][0].id = 73;
		showcase[2][0].id = 74;
		showcase[0][1].id = 71;
		showcase[1][1].id = 72;
		break;
	case BLOCK_TYPE::ORANGE:
		showcase[0][0].id = 81;
		showcase[1][0].id = 82;
		showcase[1][1].id = 83;
		showcase[2][1].id = 84;
		break;
	default:
		break;
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			blockUpdate(&showcase[j][i]);
			showcase[j][i].pSection = &showcase[j][i].section;
		}
	}

	for (int i = 0; i < 22; i++)
	{
		if (lineCheck(i) == true)
		{
			deleteLine(i);
			App->audio->PlayFx(App->sLvl_1->fxLine);
			App->points->lines++;
			App->sLvl_1->linesleft--;
			v_4L++;
			v_lines++;
			App->sLvl_1->v_points = 0;
		}
	}

	if (v_4L == 4)
	{
		if (App->points->p_stack < 10)
		{
			App->points->p_stack++;
		}
		v_4L = 0;
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

	//Showcase
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (showcase[j][i].id != -1)
			{
				App->render->Blit(blockTexture, (16 + (j * (B_WIDTH + 1))), (47 + (i * (B_HEIGHT + 1))), showcase[j][i].pSection);
			}
		}
	}

	// score + text render
	if (App->sLvl_1->v_points < 30 && v_lines != 0)
	{
		switch (v_lines)
		{
		case 1:
			App->render->TextDraw("Single  50", 64, 405, 0, 0, 255, 255, 16);
			if (App->sLvl_1->v_points == 0)
			{
				App->points->score += 50;
			}
			break;
		case 2:
			App->render->TextDraw("Double 150", 64, 405, 0, 0, 255, 255, 16);
			if (App->sLvl_1->v_points == 0)
			{
				App->points->score += 150;
			}
			break;
		case 3:
			App->render->TextDraw("Triple 400", 64, 405, 0, 0, 255, 255, 16);
			if (App->sLvl_1->v_points == 0)
			{
				App->points->score += 400;
			}
			break;
		case 4:
			App->render->TextDraw("Tetris 900", 64, 405, 0, 0, 0, 0, 16);
			if (App->sLvl_1->v_points == 0)
			{
				App->points->score += 900;
			}
			break;
		default:
			v_lines = 0;
			break;
		}
		App->sLvl_1->v_points++;
	}
	else
	{
		v_lines = 0;
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleTetronimo::CleanUp()
{
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			tileSet[j][i].id = -1;
			tileSet[j][i].tileX = j;
			tileSet[j][i].tileY = i;
			tileSet[j][i].tetronimo = 0;
		}
	}

	nextT = rand() % 7;
	rotation = 0;
	v_lines = 0;
	v_4L = 0;
	t = 0;
	pause = false;

	bool pause;

	SDL_Texture* grid_texture = nullptr;
	SDL_Texture* blockTexture = nullptr;
	return true;
}

void ModuleTetronimo::Rotation(int next) {
	int bloque = 0;

	switch (next)
	{
	case BLOCK_TYPE::RED:
		if ((rotar == 1) && (bloque == 0))
		{
			if ((b1->tileX == 9) &&
				(tileSet[b1->tileX - 3][b1->tileY].id == -1 || tileSet[b1->tileX - 3][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX - 2][b2->tileY - 1].id == -1 || tileSet[b2->tileX - 2][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX - 1][b3->tileY - 2].id == -1 || tileSet[b3->tileX - 1][b3->tileY - 2].tetronimo == b3->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var3.tileY -= 2;
				var4.tileY -= 3;

				// Change X position
				var1.tileX -= 3;
				var2.tileX -= 2;
				var3.tileX--;

				var1.id = 1;
				var2.id = 2;
				var3.id = 2;
				var4.id = 3;

				rotar++;
			}
			else if ((b1->tileX == 8) &&
				(tileSet[b1->tileX - 2][b1->tileY].id == -1 || tileSet[b1->tileX - 2][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX - 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX - 1][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX][b3->tileY - 2].id == -1 || tileSet[b3->tileX][b3->tileY - 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX + 1][b4->tileY - 3].id == -1 || tileSet[b4->tileX + 1][b4->tileY - 3].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var3.tileY -= 2;
				var4.tileY -= 3;

				// Change X position
				var1.tileX -= 2;
				var2.tileX--;
				var4.tileX++;

				var1.id = 1;
				var2.id = 2;
				var3.id = 2;
				var4.id = 3;

				rotar++;
			}
			else if ((b1->tileX == 7) &&
				(tileSet[b1->tileX - 1][b1->tileY].id == -1 || tileSet[b1->tileX - 1][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX][b2->tileY - 1].id == -1 || tileSet[b2->tileX][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX + 1][b3->tileY - 2].id == -1 || tileSet[b3->tileX + 1][b3->tileY - 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX + 2][b4->tileY - 3].id == -1 || tileSet[b4->tileX + 2][b4->tileY - 3].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var3.tileY -= 2;
				var4.tileY -= 3;

				// Change X position
				var1.tileX--;
				var3.tileX++;
				var4.tileX += 2;

				var1.id = 1;
				var2.id = 2;
				var3.id = 2;
				var4.id = 3;

				rotar++;
			}
			else if ((b1->tileX <= 6) &&
				(tileSet[b2->tileX + 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX + 1][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX + 2][b3->tileY - 2].id == -1 || tileSet[b3->tileX + 2][b3->tileY - 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX + 3][b4->tileY - 3].id == -1 || tileSet[b4->tileX + 3][b4->tileY - 3].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var3.tileY -= 2;
				var4.tileY -= 3;

				// Change X position
				var2.tileX++;
				var3.tileX += 2;
				var4.tileX += 3;

				var1.id = 1;
				var2.id = 2;
				var3.id = 2;
				var4.id = 3;

				rotar++;
			}
			bloque++;
		}

		if ((rotar == 0) && (bloque == 0))
		{
			if ((b1->tileY + 3 < 22) &&
				(tileSet[b2->tileX - 1][b2->tileY + 1].id == -1 || tileSet[b2->tileX - 1][b2->tileY + 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX - 2][b3->tileY + 2].id == -1 || tileSet[b3->tileX - 2][b3->tileY - 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX - 3][b4->tileY + 3].id == -1 || tileSet[b4->tileX - 3][b4->tileY + 3].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY++;
				var3.tileY += 2;
				var4.tileY += 3;

				// Change X position
				var2.tileX--;
				var3.tileX -= 2;
				var4.tileX -= 3;

				//Change id's
				var1.id = 4;
				var2.id = 5;
				var3.id = 5;
				var4.id = 6;

				rotar++;
			}
			bloque++;
		}
		if (rotar == 2) {
			rotar = 0;
		}
		break;

	case BLOCK_TYPE::GREEN:
		if ((rotar == 3) && (bloque == 0))
		{
			if ((b1->tileX == 1) &&
				(tileSet[b1->tileX - 1][b1->tileY].id == -1 || tileSet[b1->tileX - 1][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX][b2->tileY - 1].id == -1 || tileSet[b2->tileX][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX + 1][b3->tileY - 2].id == -1 || tileSet[b3->tileX + 1][b3->tileY - 2].tetronimo == b3->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var3.tileY -= 2;

				// Change X position
				var1.tileX--;
				var3.tileX++;
				var4.tileX++;

				//Change id's
				var1.id = 11;
				var2.id = 12;
				var3.id = 13;
				var4.id = 14;

				rotar++;
			}
			else
			{
				if ((tileSet[b1->tileX - 2][b1->tileY].id == -1 || tileSet[b1->tileX - 2][b1->tileY].tetronimo == b1->tetronimo) &&
					(tileSet[b2->tileX - 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX - 1][b2->tileY - 1].tetronimo == b2->tetronimo) &&
					(tileSet[b3->tileX][b3->tileY - 2].id == -1 || tileSet[b3->tileX][b3->tileY - 2].tetronimo == b3->tetronimo))
				{
					// save current block info
					var1 = *b1;
					var2 = *b2;
					var3 = *b3;
					var4 = *b4;

					// Change Y position
					var2.tileY--;
					var3.tileY -= 2;

					// Change X position
					var1.tileX -= 2;
					var2.tileX--;

					//Change id's
					var1.id = 11;
					var2.id = 12;
					var3.id = 13;
					var4.id = 14;

					rotar++;
				}
			}
			bloque++;
		}
		if ((rotar == 2) && (bloque == 0))
		{
			if ((tileSet[b1->tileX][b1->tileY - 2].id == -1 || tileSet[b1->tileX][b1->tileY - 2].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX + 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX + 1][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX + 2][b3->tileY].id == -1 || tileSet[b3->tileX + 2][b3->tileY].tetronimo == b3->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY -= 2;
				var2.tileY--;

				// Change X position
				var2.tileX++;
				var3.tileX += 2;

				//Change id's
				var1.id = 15;
				var2.id = 18;
				var3.id = 14;
				var4.id = 11;

				rotar++;
			}
			bloque++;
		}

		if ((rotar == 1) && (bloque == 0))
		{
			if ((b1->tileX == 8) &&
				(tileSet[b1->tileX + 1][b1->tileY].id == -1 || tileSet[b1->tileX + 1][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX][b2->tileY + 1].id == -1 || tileSet[b2->tileX][b2->tileY + 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX - 1][b3->tileY + 2].id == -1 || tileSet[b3->tileX - 1][b3->tileY + 2].tetronimo == b3->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY++;
				var3.tileY += 2;

				// Change X position
				var1.tileX++;
				var3.tileX--;
				var4.tileX--;

				//Change id's
				var1.id = 13;
				var2.id = 17;
				var3.id = 11;
				var4.id = 15;

				rotar++;
			}
			else
			{
				if ((tileSet[b1->tileX + 2][b1->tileY].id == -1 || tileSet[b1->tileX + 2][b1->tileY].tetronimo == b1->tetronimo) &&
					(tileSet[b2->tileX + 1][b2->tileY + 1].id == -1 || tileSet[b2->tileX + 1][b2->tileY + 1].tetronimo == b2->tetronimo) &&
					(tileSet[b3->tileX][b3->tileY + 2].id == -1 || tileSet[b3->tileX][b3->tileY + 2].tetronimo == b3->tetronimo))
				{
					// save current block info
					var1 = *b1;
					var2 = *b2;
					var3 = *b3;
					var4 = *b4;

					// Change Y position
					var2.tileY++;
					var3.tileY += 2;

					// Change X position
					var1.tileX += 2;
					var2.tileX++;

					//Change id's
					var1.id = 13;
					var2.id = 17;
					var3.id = 11;
					var4.id = 15;

					rotar++;
				}
			}
			bloque++;
		}

		if ((rotar == 0) && (bloque == 0))
		{
			if ((b1->tileY + 2 < 22) &&
				(tileSet[b1->tileX][b1->tileY + 2].id == -1 || tileSet[b1->tileX][b1->tileY + 2].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX - 1][b2->tileY + 1].id == -1 || tileSet[b2->tileX - 1][b2->tileY + 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX - 2][b3->tileY].id == -1 || tileSet[b3->tileX - 2][b3->tileY].tetronimo == b3->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY += 2;
				var2.tileY++;

				// Change X position
				var2.tileX--;
				var3.tileX -= 2;

				//Change id's
				var1.id = 14;
				var2.id = 16;
				var3.id = 15;
				var4.id = 13;

				rotar++;
			}
			bloque++;
		}
		if (rotar == 4) {
			rotar = 0;
		}
		break;

	case BLOCK_TYPE::YELLOW:
		if ((rotar == 3) && (bloque == 0))
		{
			if ((b1->tileX == 1) &&
				(tileSet[b1->tileX - 1][b1->tileY].id == -1 || tileSet[b1->tileX - 1][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX][b2->tileY - 1].id == -1 || tileSet[b2->tileX][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX + 1][b3->tileY - 2].id == -1 || tileSet[b3->tileX + 1][b3->tileY - 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX + 2][b4->tileY - 1].id == -1 || tileSet[b4->tileX + 2][b4->tileY - 1].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var3.tileY -= 2;
				var4.tileY--;

				// Change X position
				var1.tileX--;
				var3.tileX++;
				var4.tileX += 2;

				//Change id's
				var1.id = 41;
				var2.id = 42;
				var3.id = 43;
				var4.id = 44;

				rotar++;
			}
			else
			{
				if ((tileSet[b1->tileX - 2][b1->tileY].id == -1 || tileSet[b1->tileX - 2][b1->tileY].tetronimo == b1->tetronimo) &&
					(tileSet[b2->tileX - 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX - 1][b2->tileY - 1].tetronimo == b2->tetronimo) &&
					(tileSet[b3->tileX][b3->tileY - 2].id == -1 || tileSet[b3->tileX][b3->tileY - 2].tetronimo == b3->tetronimo) &&
					(tileSet[b4->tileX + 1][b4->tileY - 1].id == -1 || tileSet[b4->tileX + 1][b4->tileY - 1].tetronimo == b4->tetronimo))
				{
					// save current block info
					var1 = *b1;
					var2 = *b2;
					var3 = *b3;
					var4 = *b4;

					// Change Y position
					var2.tileY--;
					var3.tileY -= 2;
					var4.tileY--;

					// Change X position
					var1.tileX -= 2;
					var2.tileX--;
					var4.tileX++;

					//Change id's
					var1.id = 41;
					var2.id = 42;
					var3.id = 43;
					var4.id = 44;

					rotar++;
				}
			}
			bloque++;
		}
		if ((rotar == 2) && (bloque == 0))
		{
			if ((tileSet[b1->tileX][b1->tileY - 2].id == -1 || tileSet[b1->tileX][b1->tileY - 2].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX + 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX + 1][b2->tileY - 1].tetronimo == b2->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY -= 2;
				var2.tileY--;
				var4.tileY++;

				// Change X position
				var2.tileX++;
				var3.tileX += 2;
				var4.tileX++;

				//Change id's
				var1.id = 48;
				var2.id = 46;
				var3.id = 50;
				var4.id = 41;

				rotar++;
			}
			bloque++;
		}
		if ((rotar == 1) && (bloque == 0))
		{
			if ((b1->tileX == 8) &&
				(tileSet[b1->tileX + 1][b1->tileY].id == -1 || tileSet[b1->tileX + 1][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX][b2->tileY + 1].id == -1 || tileSet[b2->tileX][b2->tileY + 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX - 1][b3->tileY + 2].id == -1 || tileSet[b3->tileX - 1][b3->tileY + 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX - 2][b4->tileY + 1].id == -1 || tileSet[b4->tileX - 2][b4->tileY + 1].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY++;
				var3.tileY += 2;
				var4.tileY++;

				// Change X position
				var1.tileX++;
				var3.tileX--;
				var4.tileX -= 2;

				//Change id's
				var1.id = 47;
				var2.id = 42;
				var3.id = 49;
				var4.id = 48;

				rotar++;
			}
			else
			{
				if ((b1->tileX + 2 < 10 && b2->tileX + 1 < 10) &&
					(tileSet[b1->tileX + 2][b1->tileY].id == -1 || tileSet[b1->tileX + 2][b1->tileY].tetronimo == b1->tetronimo) &&
					(tileSet[b2->tileX + 1][b2->tileY + 1].id == -1 || tileSet[b2->tileX + 1][b2->tileY + 1].tetronimo == b2->tetronimo))
				{
					// save current block info
					var1 = *b1;
					var2 = *b2;
					var3 = *b3;
					var4 = *b4;

					// Change Y position
					var2.tileY++;
					var3.tileY += 2;
					var4.tileY++;

					// Change X position
					var1.tileX += 2;
					var2.tileX++;
					var4.tileX--;

					//Change id's
					var1.id = 47;
					var2.id = 42;
					var3.id = 49;
					var4.id = 48;

					rotar++;
				}
			}

			bloque++;
		}

		if ((rotar == 0) && (bloque == 0))
		{
			if ((b1->tileY + 2 < 22) &&
				(tileSet[b1->tileX][b1->tileY + 2].id == -1 || tileSet[b1->tileX][b1->tileY + 2].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX - 1][b2->tileY + 1].id == -1 || tileSet[b2->tileX - 1][b2->tileY + 1].tetronimo == b2->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY += 2;
				var2.tileY++;
				var4.tileY--;

				// Change X position
				var2.tileX--;
				var3.tileX -= 2;
				var4.tileX--;

				//Change id's
				var1.id = 44;
				var2.id = 46;
				var3.id = 45;
				var4.id = 47;

				rotar++;
			}
			bloque++;
		}

		if (rotar == 4) {
			rotar = 0;
		}
		break;

	case BLOCK_TYPE::PINK:
		if ((rotar == 3) && (bloque == 0))
		{
			if ((b1->tileX == 1) &&
				(tileSet[b1->tileX - 1][b2->tileY].id == -1 || tileSet[b1->tileX - 1][b2->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX][b2->tileY - 1].id == -1 || tileSet[b2->tileX][b3->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX + 1][b3->tileY - 2].id == -1 || tileSet[b3->tileX + 1][b3->tileY - 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX][b1->tileY + 1].id == -1 || tileSet[b4->tileX][b1->tileY + 1].tetronimo == b4->tetronimo))
			{

				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var3.tileY -= 2;
				var4.tileY++;

				// Change X position
				var1.tileX--;
				var3.tileX++;

				//Change id's
				var1.id = 51;
				var2.id = 52;
				var3.id = 53;
				var4.id = 54;

				rotar++;
			}
			else
			{
				if ((tileSet[b1->tileX - 2][b2->tileY].id == -1 || tileSet[b1->tileX - 2][b2->tileY].tetronimo == b1->tetronimo) &&
					(tileSet[b2->tileX - 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX - 1][b3->tileY - 1].tetronimo == b2->tetronimo) &&
					(tileSet[b3->tileX][b3->tileY - 2].id == -1 || tileSet[b3->tileX][b3->tileY - 2].tetronimo == b3->tetronimo) &&
					(tileSet[b4->tileX + 1][b1->tileY + 1].id == -1 || tileSet[b4->tileX + 1][b1->tileY + 1].tetronimo == b4->tetronimo))
				{

					// save current block info
					var1 = *b1;
					var2 = *b2;
					var3 = *b3;
					var4 = *b4;

					// Change Y position
					var2.tileY--;
					var3.tileY -= 2;
					var4.tileY++;

					// Change X position
					var1.tileX -= 2;
					var2.tileX--;
					var4.tileX--;

					//Change id's
					var1.id = 51;
					var2.id = 52;
					var3.id = 53;
					var4.id = 54;

					rotar++;
				}
			}
			/*rectIdle.x -= 20;
			rectIdle1.x -= 10;
			rectIdle3.x -= 10;

			rectIdle1.y -= 10;
			rectIdle2.y -= 20;
			rectIdle3.y += 10;*/

			bloque++;
		}

		if ((rotar == 2) && (bloque == 0))
		{
			if ((tileSet[b1->tileX][b2->tileY - 2].id == -1 || tileSet[b1->tileX][b2->tileY - 2].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX + 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX + 1][b3->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX + 2][b3->tileY].id == -1 || tileSet[b3->tileX + 2][b3->tileY].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX - 1][b1->tileY - 1].id == -1 || tileSet[b4->tileX - 1][b1->tileY - 1].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY -= 2;
				var2.tileY--;
				var4.tileY--;

				// Change X position
				var2.tileX++;
				var3.tileX += 2;
				var4.tileX--;

				//Change id's
				var1.id = 60;
				var2.id = 56;
				var3.id = 54;
				var4.id = 58;

				rotar++;
			}


			/*rectIdle1.x += 10;
			rectIdle2.x += 20;
			rectIdle3.x -= 10;

			rectIdle.y -= 20;
			rectIdle1.y -= 10;
			rectIdle3.y -= 10;*/

			bloque++;
		}

		if ((rotar == 1) && (bloque == 0))
		{
			if ((b1->tileX == 8) &&
				(tileSet[b1->tileX + 1][b2->tileY].id == -1 || tileSet[b1->tileX + 1][b2->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX][b2->tileY + 1].id == -1 || tileSet[b2->tileX][b3->tileY + 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX - 1][b3->tileY + 2].id == -1 || tileSet[b3->tileX - 1][b3->tileY + 2].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX][b1->tileY - 1].id == -1 || tileSet[b4->tileX][b1->tileY - 1].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY++;
				var3.tileY += 2;
				var4.tileY--;

				// Change X position
				var1.tileX++;
				var3.tileX--;

				//Change id's
				var1.id = 59;
				var2.id = 52;
				var3.id = 58;
				var4.id = 55;

				rotar++;
			}
			else
			{
				if ((tileSet[b1->tileX + 2][b2->tileY].id == -1 || tileSet[b1->tileX + 2][b2->tileY].tetronimo == b1->tetronimo) &&
					(tileSet[b2->tileX + 1][b2->tileY + 1].id == -1 || tileSet[b2->tileX + 1][b3->tileY + 1].tetronimo == b2->tetronimo) &&
					(tileSet[b3->tileX][b3->tileY + 2].id == -1 || tileSet[b3->tileX][b3->tileY + 2].tetronimo == b3->tetronimo) &&
					(tileSet[b4->tileX + 1][b1->tileY - 1].id == -1 || tileSet[b4->tileX + 1][b1->tileY - 1].tetronimo == b4->tetronimo))
				{
					// save current block info
					var1 = *b1;
					var2 = *b2;
					var3 = *b3;
					var4 = *b4;

					// Change Y position
					var2.tileY++;
					var3.tileY += 2;
					var4.tileY--;

					// Change X position
					var1.tileX += 2;
					var2.tileX++;
					var4.tileX++;

					//Change id's
					var1.id = 59;
					var2.id = 52;
					var3.id = 58;
					var4.id = 55;

					rotar++;
				}
			}

			/*rectIdle.x += 20;
			rectIdle1.x += 10;
			rectIdle3.x += 10;

			rectIdle1.y += 10;
			rectIdle2.y += 20;
			rectIdle3.y -= 10;*/

			bloque++;
		}

		if ((rotar == 0) && (bloque == 0))
		{
			if ((b1->tileY + 2 < 22) &&
				(tileSet[b1->tileX][b2->tileY + 2].id == -1 || tileSet[b1->tileX][b2->tileY + 2].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX - 1][b2->tileY + 1].id == -1 || tileSet[b2->tileX - 1][b3->tileY + 1].tetronimo == b2->tetronimo) &&
				(tileSet[b3->tileX - 2][b3->tileY].id == -1 || tileSet[b3->tileX - 2][b3->tileY].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX + 1][b1->tileY + 1].id == -1 || tileSet[b4->tileX + 1][b1->tileY + 1].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY += 2;
				var2.tileY++;
				var4.tileY++;

				// Change X position
				var2.tileX--;
				var3.tileX -= 2;
				var4.tileX++;

				//Change id's
				var1.id = 57;
				var2.id = 56;
				var3.id = 55;
				var4.id = 53;

				rotar++;
			}

			//rectIdle1.x -= 10;
			//rectIdle2.x -= 20;
			//rectIdle3.x += 10;

			//rectIdle.y += 20;
			//rectIdle1.y += 10;
			//rectIdle3.y += 10;

			bloque++;
		}

		if (rotar == 4) {
			rotar = 0;
		}

		break;

	case BLOCK_TYPE::CYAN:
		if ((rotar == 1) && (bloque == 0))
		{
			if ((b1->tileX == 8) &&
				(tileSet[b1->tileX - 1][b1->tileY + 1].id == -1 || tileSet[b1->tileX - 1][b1->tileY + 1].tetronimo == b1->tetronimo) &&
				(tileSet[b4->tileX][b4->tileY - 2].id == -1 || tileSet[b4->tileX][b4->tileY - 2].tetronimo == b4->tetronimo))
			{
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY++;
				var3.tileY--;
				var4.tileY -= 2;

				// Change X position
				var1.tileX--;
				var3.tileX--;

				//Change id's
				var1.id = 71;
				var2.id = 72;
				var3.id = 73;
				var4.id = 74;

				rotar++;
			}
			else if ((tileSet[b3->tileX][b3->tileY - 1].id == -1 || tileSet[b3->tileX][b3->tileY - 1].tetronimo == b3->tetronimo) &&
				(tileSet[b4->tileX + 1][b4->tileY - 2].id == -1 || tileSet[b4->tileX + 1][b4->tileY - 2].tetronimo == b4->tetronimo))
			{

				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY++;
				var3.tileY--;
				var4.tileY -= 2;

				// Change X position
				var2.tileX++;
				var4.tileX++;

				//Change id's
				var1.id = 71;
				var2.id = 72;
				var3.id = 73;
				var4.id = 74;

				rotar++;
			}
			bloque++;
		}

		if ((rotar == 0) && (bloque == 0))
		{
			if ((b1->tileY + 1 < 22) &&
				(tileSet[b1->tileX][b1->tileY - 1].id == -1 || tileSet[b1->tileX][b1->tileY - 1].tetronimo == b1->tetronimo) &&
				(tileSet[b4->tileX - 1][b4->tileY + 2].id == -1 || tileSet[b4->tileX - 1][b4->tileY + 2].tetronimo == b4->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY--;
				var3.tileY++;
				var4.tileY += 2;

				// Change X position
				var2.tileX--;
				var4.tileX--;

				//Change id's
				var1.id = 75;
				var2.id = 76;
				var3.id = 77;
				var4.id = 78;

				rotar++;
			}

			bloque++;
		}

	case BLOCK_TYPE::ORANGE:
		if ((rotar == 1) && (bloque == 0))
		{
			if ((b3->tileX == 0) &&
				(tileSet[b1->tileX - 1][b1->tileY].id == -1 || tileSet[b1->tileX - 1][b1->tileY].tetronimo == b1->tetronimo) &&
				(tileSet[b4->tileX + 2][b4->tileY - 1].id == -1 || tileSet[b4->tileX + 2][b4->tileY - 1].tetronimo == b4->tetronimo))
			{

				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var4.tileY--;

				// Change X position
				var1.tileX--;
				var3.tileX++;
				var4.tileX += 2;
				//var4.tileX--;

				//Change id's
				var1.id = 81;
				var2.id = 82;
				var3.id = 83;
				var4.id = 84;

				rotar++;
			}
			else if ((tileSet[b2->tileX - 1][b2->tileY - 1].id == -1 || tileSet[b2->tileX - 1][b2->tileY - 1].tetronimo == b2->tetronimo) &&
				(tileSet[b1->tileX - 2][b1->tileY].id == -1 || tileSet[b1->tileX - 2][b1->tileY].tetronimo == b1->tetronimo))
			{

				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY--;
				var4.tileY--;

				// Change X position
				var1.tileX -= 2;
				var2.tileX--;
				var4.tileX++;

				//Change id's
				var1.id = 81;
				var2.id = 82;
				var3.id = 83;
				var4.id = 84;

				rotar++;
			}

			bloque++;
		}

		if ((rotar == 0) && (bloque == 0))
		{
			if (((b3->tileY < 21) &&
				(tileSet[b4->tileX - 1][b4->tileY + 1].id == -1 || tileSet[b4->tileX - 1][b4->tileY + 1].tetronimo == b4->tetronimo) &&
				(tileSet[b1->tileX + 2][b1->tileY - 1].id == -1 || tileSet[b1->tileX + 2][b1->tileY - 1].tetronimo == b1->tetronimo)))
			{

				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var2.tileY++;
				var4.tileY++;

				// Change X position
				var1.tileX += 2;
				var2.tileX++;
				var4.tileX--;

				//Change id's
				var1.id = 85;
				var2.id = 86;
				var3.id = 87;
				var4.id = 88;

				rotar++;
			}
			else if ((tileSet[b1->tileX + 2][b1->tileY - 1].id == -1 || tileSet[b1->tileX + 2][b1->tileY - 1].tetronimo == b1->tetronimo) &&
				(tileSet[b2->tileX + 1][b2->tileY].id == -1 || tileSet[b2->tileX + 1][b2->tileY].tetronimo == b2->tetronimo))
			{
				// save current block info
				var1 = *b1;
				var2 = *b2;
				var3 = *b3;
				var4 = *b4;

				// Change Y position
				var1.tileY--;
				var3.tileY--;

				// Change X position
				var1.tileX += 2;
				var2.tileX++;
				var4.tileX--;

				//Change id's
				var1.id = 85;
				var2.id = 86;
				var3.id = 87;
				var4.id = 88;

				rotar++;
			}

			bloque++;
		}
		if (rotar == 2)
		{
			rotar = 0;
		}

		break;

	default:
		break;
	}

	// make the block non printable 
	tileSet[b1->tileX][b1->tileY].id = -1;
	tileSet[b2->tileX][b2->tileY].id = -1;
	tileSet[b3->tileX][b3->tileY].id = -1;
	tileSet[b4->tileX][b4->tileY].id = -1;

	// fill the next block with the current block info
	tileSet[var1.tileX][var1.tileY] = var1;
	tileSet[var2.tileX][var2.tileY] = var2;
	tileSet[var3.tileX][var3.tileY] = var3;
	tileSet[var4.tileX][var4.tileY] = var4;

	// change the pointer's position
	b1 = &tileSet[var1.tileX][var1.tileY];
	b2 = &tileSet[var2.tileX][var2.tileY];
	b3 = &tileSet[var3.tileX][var3.tileY];
	b4 = &tileSet[var4.tileX][var4.tileY];
}

int ModuleTetronimo::spawnTetronimo(int next)
{
	t++;
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

	if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
	{
		b1->tetronimo = t;
		b2->tetronimo = t;
		b3->tetronimo = t;
		b4->tetronimo = t;
	}
	currentT = next;
	return rand() % 7 + 1;
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
	case 30:
		block->section = { 238, 34, 16, 16 };
		break;

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

		//cyan
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
		block->section = { 238, 102, 16, 16 };
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

bool ModuleTetronimo::blockFall(int p)
{
	if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
	{
		// check border's - check if tile below is null OR if it's not, if the block is from the same tetronim, move.
		if (((p == 1 && (b1->tileY + p < 22 && b2->tileY + p < 22 && b3->tileY + p < 22 && b4->tileY + p < 22)) ||
			(p == -1 && (b1->tileY + p > 1 && b2->tileY + p > 1 && b3->tileY + p > 1 && b4->tileY + p > 1))) &&
			(tileSet[b1->tileX][b1->tileY + p].id == -1 || tileSet[b1->tileX][b1->tileY + p].tetronimo == b1->tetronimo) &&
			(tileSet[b2->tileX][b2->tileY + p].id == -1 || tileSet[b2->tileX][b2->tileY + p].tetronimo == b2->tetronimo) &&
			(tileSet[b3->tileX][b3->tileY + p].id == -1 || tileSet[b3->tileX][b3->tileY + p].tetronimo == b3->tetronimo) &&
			(tileSet[b4->tileX][b4->tileY + p].id == -1 || tileSet[b4->tileX][b4->tileY + p].tetronimo == b4->tetronimo))
		{
			// save current block info
			var1 = *b1;
			var2 = *b2;
			var3 = *b3;
			var4 = *b4;

			// change the Y position
			var1.tileY += p;
			var2.tileY += p;
			var3.tileY += p;
			var4.tileY += p;

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

			App->sLvl_1->v_fall = true;
			return true;
		}
		else if (p == 1)
		{
			if ((b1->tileY == 1 || b2->tileY == 1 || b3->tileY == 1 || b4->tileY == 1) && App->player->godMode == false)
			{
				App->sLvl_1->lvl_instaLose = true;
			}
			else
			{
				int arr[] = { b1->tileY , b2->tileY , b3->tileY , b4->tileY };
				for (int i = 0; i < 4 - 1; i++)
				{
					for (int j = 0; j < 4 - i - 1; j++)
					{
						if (arr[j] < arr[j + 1])
						{
							int temp = arr[j];
							arr[j] = arr[j + 1];
							arr[j + 1] = temp;
						}
					}
				}
				App->points->h = 21 - arr[0];

				App->audio->PlayFx(App->sLvl_1->fxBlock_Fall);
				rotar = 0;
				nextT = spawnTetronimo(nextT);
			}
			App->sLvl_1->v_fall = false;
			return false;
		}
	}
}
void ModuleTetronimo::blockMoveX(int p)
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

bool ModuleTetronimo::lineCheck(int i)
{
	for (int j = 0; j < 10; j++)
	{
		if (tileSet[j][i].id == -1)
		{
			return false;
		}
	}

	if (b1->tileY != i && b2->tileY != i && b3->tileY != i && b4->tileY != i)
	{
		App->audio->PlayFx(App->sLvl_1->fxLine);
		return true;
	}
}
bool ModuleTetronimo::deleteLine(int i)
{
	pause = true;

	for (int j = 0; j < 10; j++)
	{
		switch (tileSet[j][i].id)
		{
			// red
		case 4:
			if (tileSet[j][i + 1].id == 6)
			{
				tileSet[j][i + 1].id = 0;
			}
			if (tileSet[j][i + 1].id == 5)
			{
				tileSet[j][i + 1].id = 4;
			}
			break;
		case 5:
			if (tileSet[j][i - 1].id == 4)
			{
				tileSet[j][i - 1].id = 4;
			}
			break;
		case 6:
			if (tileSet[j][i - 1].id == 4)
			{
				tileSet[j][i - 1].id = 0;
			}
			if (tileSet[j][i - 1].id == 5)
			{
				tileSet[j][i - 1].id = 6;
			}
			break;

			// green
		case 12:
			tileSet[j][i + 1].id = 10;
			break;
		case 14:
			if (tileSet[j][i - 1].id == 12)
			{
				tileSet[j][i - 1].id = 19;
			}
			if (tileSet[j][i - 1].id == 15)
			{
				tileSet[j][i - 1].id = 10;
			}
			if (tileSet[j][i - 1].id == 16)
			{
				tileSet[j][i - 1].id = 20;
			}
			if (tileSet[j][i - 1].id == 18)
			{
				tileSet[j][i - 1].id = 22;
			}
			if (tileSet[j][i - 1].id == 21 || tileSet[j][i - 1].id == 23)
			{
				tileSet[j][i - 1].id = 11;
			}
			break;
		case 15:
			if (tileSet[j][i + 1].id == 14)
			{
				tileSet[j][i + 1].id = 10;
			}
			if (tileSet[j][i + 1].id == 16)
			{
				tileSet[j][i + 1].id = 21;
			}
			if (tileSet[j][i + 1].id == 17)
			{
				tileSet[j][i + 1].id = 19;
			}
			if (tileSet[j][i + 1].id == 18)
			{
				tileSet[j][i + 1].id = 23;
			}
			if (tileSet[j][i + 1].id == 20 || tileSet[j][i + 1].id == 22)
			{
				tileSet[j][i + 1].id = 11;
			}
			break;
		case 16:
			tileSet[j][i - 1].id = 15;
			break;
		case 17:
			tileSet[j][i - 1].id = 10;
			break;
		case 18:
			tileSet[j][i - 1].id = 15;
			break;
		case 20:
			tileSet[j][i - 1].id = 10;
			break;
		case 21:
			tileSet[j][i + 1].id = 10;
			break;
		case 22:
			tileSet[j][i + 1].id = 10;
			break;
		case 23:
			tileSet[j][i + 1].id = 10;
			break;

			// blue
		case 31:
			tileSet[j][i + 1].id = 35;
			break;
		case 32:
			tileSet[j][i + 1].id = 36;
			break;
		case 33:
			tileSet[j][i - 1].id = 35;
			break;
		case 34:
			tileSet[j][i - 1].id = 36;
			break;

			// yellow
		case 43:
			tileSet[j][i + 1].id = 40;
			break;
		case 44:
			if (tileSet[j][i - 1].id == 43)
			{
				tileSet[j][i - 1].id = 47;
			}
			if (tileSet[j][i - 1].id == 45)
			{
				tileSet[j][i - 1].id = 41;
			}
			if (tileSet[j][i - 1].id == 46)
			{
				tileSet[j][i - 1].id = 44;
			}
			if (tileSet[j][i - 1].id == 48)
			{
				tileSet[j][i - 1].id = 40;
			}
			break;
		case 45:
			if (tileSet[j][i + 1].id == 44)
			{
				tileSet[j][i + 1].id = 40;
			}
			if (tileSet[j][i + 1].id == 46)
			{
				tileSet[j][i + 1].id = 48;
			}
			break;
		case 46:
			if (tileSet[j][i - 1].id == 45)
			{
				tileSet[j][i - 1].id = 45;
			}
			if (tileSet[j][i - 1].id == 48)
			{
				tileSet[j][i - 1].id = 48;
			}
			break;
		case 48:
			if (tileSet[j][i + 1].id == 44)
			{
				tileSet[j][i + 1].id = 40;
			}
			if (tileSet[j][i + 1].id == 46)
			{
				tileSet[j][i + 1].id = 48;
			}
			if (tileSet[j][i + 1].id == 49)
			{
				tileSet[j][i + 1].id = 41;
			}
			if (tileSet[j][i + 1].id == 50)
			{
				tileSet[j][i + 1].id = 47;
			}
			break;
		case 49:
			tileSet[j][i - 1].id = 40;
			break;
		case 50:
			if (tileSet[j][i - 1].id == 46)
			{
				tileSet[j][i - 1].id = 44;
			}
			if (tileSet[j][i - 1].id == 48)
			{
				tileSet[j][i - 1].id = 40;
			}
			break;

			// pink
		case 51:
			tileSet[j][i + 1].id = 61;
			break;
		case 54:
			if (tileSet[j][i - 1].id == 46)
			{
				tileSet[j][i - 1].id = 58;
			}
			if (tileSet[j][i - 1].id == 51)
			{
				tileSet[j][i - 1].id = 58;
			}
			if (tileSet[j][i - 1].id == 55)
			{
				tileSet[j][i - 1].id = 61;
			}
			if (tileSet[j][i - 1].id == 56)
			{
				tileSet[j][i - 1].id = 60;
			}
			break;
		case 55:
			if (tileSet[j][i + 1].id == 54)
			{
				tileSet[j][i + 1].id = 61;
			}
			if (tileSet[j][i + 1].id == 56)
			{
				tileSet[j][i + 1].id = 55;
			}
			if (tileSet[j][i + 1].id == 57)
			{
				tileSet[j][i + 1].id = 58;
			}
			if (tileSet[j][i + 1].id == 59)
			{
				tileSet[j][i + 1].id = 53;
			}
			break;
		case 56:
			if (tileSet[j][i - 1].id == 55)
			{
				tileSet[j][i - 1].id = 55;
			}
			if (tileSet[j][i - 1].id == 60)
			{
				tileSet[j][i - 1].id = 60;
			}
			break;
		case 57:
			if (tileSet[j][i - 1].id == 55)
			{
				tileSet[j][i - 1].id = 61;
			}
			if (tileSet[j][i - 1].id == 56)
			{
				tileSet[j][i - 1].id = 54;
			}
			break;
		case 59:
			tileSet[j][i - 1].id = 61;
			break;
		case 60:
			if (tileSet[j][i + 1].id == 54)
			{
				tileSet[j][i + 1].id = 61;
			}
			if (tileSet[j][i + 1].id == 56)
			{
				tileSet[j][i + 1].id = 55;
			}
			break;

			// cyan
		case 72:
			tileSet[j][i - 1].id = 71;
			break;
		case 73:
			tileSet[j][i + 1].id = 74;
			break;
		case 75:
			tileSet[j][i + 1].id = 71;
			break;
		case 76:
			tileSet[j][i - 1].id = 70;
			break;
		case 77:
			tileSet[j][i + 1].id = 70;
			break;
		case 78:
			tileSet[j][i - 1].id = 74;
			break;

			// orange
		case 82:
			tileSet[j][i + 1].id = 81;
			break;
		case 83:
			tileSet[j][i - 1].id = 84;
			break;
		case 85:
			tileSet[j][i + 1].id = 84;
			break;
		case 86:
			tileSet[j][i - 1].id = 80;
			break;
		case 87:
			tileSet[j][i + 1].id = 80;
			break;
		case 88:
			tileSet[j][i - 1].id = 81;
			break;
		default:
			break;
		}
		tileSet[j][i].id = -1;
	}

	for (int m = i - 1; m >= 0; m--)
	{
		for (int n = 0; n < 10; n++)
		{
			if (m != 0 && m != 1)
			{
				if (tileSet[n][m].id != -1)
				{
					v = tileSet[n][m];
					tileSet[n][m].id = -1;
					tileSet[n][m + 1].id = v.id;
				}
			}
		}
	}

	b1 = nullptr;
	b2 = nullptr;
	b3 = nullptr;
	b4 = nullptr;
	pause = false;

	spawnTetronimo(currentT);
	return true;
}

void ModuleTetronimo::Debugging()
{
	if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN)
	{
		if (App->player->godMode == false)
		{
			App->player->godMode = true;
			pause = true;
		}
		else
		{
			App->player->godMode = false;
			pause = false;
		}
	}

	// Manually spawn a block
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_REPEAT)
	{
		rotar = 0;

		if (App->input->keys[SDL_SCANCODE_0] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(rand() % 7 + 1);
		}
		else if (App->input->keys[SDL_SCANCODE_1] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(BLOCK_TYPE::RED);
		}
		else if (App->input->keys[SDL_SCANCODE_2] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(BLOCK_TYPE::GREEN);
		}
		else if (App->input->keys[SDL_SCANCODE_3] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(BLOCK_TYPE::BLUE);
		}
		else if (App->input->keys[SDL_SCANCODE_4] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(BLOCK_TYPE::YELLOW);
		}
		else if (App->input->keys[SDL_SCANCODE_5] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(BLOCK_TYPE::PINK);
		}
		else if (App->input->keys[SDL_SCANCODE_6] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(BLOCK_TYPE::CYAN);
		}
		else if (App->input->keys[SDL_SCANCODE_7] == KEY_DOWN)
		{
			if (b1 != nullptr && b2 != nullptr && b3 != nullptr && b4 != nullptr)
			{
				b1->id = -1;
				b2->id = -1;
				b3->id = -1;
				b4->id = -1;
			}
			spawnTetronimo(BLOCK_TYPE::ORANGE);
		}
	}

	// +- lines
	if (App->input->keys[SDL_SCANCODE_F6] == KEY_DOWN)
	{
		//App->player->godMode = false;
		if (App->sLvl_1->linesleft > 0)
		{
			v_4L++;
			App->sLvl_1->linesleft--;
			App->points->lines++;
		}
	}

	// Pause movement
	if (App->input->keys[SDL_SCANCODE_P] == KEY_STATE::KEY_DOWN)
	{
		if (pause == false)
		{
			pause = true;
			App->audio->PauseMusic();
		}
		else
		{
			pause = false;
			App->audio->ResumeMusic();
		}
	}

	if (App->player->godMode == true)
	{
		App->render->Blit(grid_texture, 62, 50, NULL);

		if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN || fx < 0 && App->input->num_controllers > 0)
		{
			blockMoveX(-1);
		}
		if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN || fx > 0 && App->input->num_controllers > 0)
		{
			blockMoveX(1);
		}
		if (App->input->keys[SDL_SCANCODE_W] == KEY_STATE::KEY_DOWN || fx > 0 && App->input->num_controllers > 0)
		{
			blockFall(-1);
		}
		if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT || fy > 0 && App->input->num_controllers > 0)
		{
			blockFall(1);
		}
		if (App->input->keys[SDL_SCANCODE_R] == KEY_STATE::KEY_DOWN || button_press)
		{
			Rotation(currentT);
		}
	}
}