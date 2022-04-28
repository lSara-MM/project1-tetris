#include "ModuleTetronimo.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ScreenLvl_1.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

#include <stdlib.h>
#include <time.h>

#include "SDL/include/SDL_scancode.h"
#include <sstream>


//20 (22 spawn) y
//10 x
//pieza de spawn en x = 5
//pieza de spawn en y = 0

bool const r = (srand(time(NULL)), true);
int  num = rand() % 7;
int rotar = 0;

SDL_Surface* dots = NULL;
SDL_Surface* screen = NULL;

//The event structure
SDL_Event event;

//The portions of the sprite map to be blitted
uint runTime = 0;
uint deltaTime = 0;
uint lastTickTime = 0;

ModuleTetronimo::ModuleTetronimo(bool startEnabled) : Module(startEnabled) {

	//currentAnimation = &idleAnim;
}


ModuleTetronimo::~ModuleTetronimo() {



}

bool ModuleTetronimo::Start() {

	//SDL_Init(IMG_INIT_PNG);
	//SDL_Surface* image = IMG_Load("Assets/Sprites/Tetramino/Spritesheet/ss_tetramino.png");
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, image);

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 10; j++) {
			arr[j][i] = 0;
			type[j][i] = 'N';
		}
	}

	combo = 0;

	lvl_instaLose = false;
	lvl_instaWin = false;

	App->player->godMode = false;
	v_loseContinue = 9;
	v_WinLose = 0;

	LOG("Loading grid_texture");
	//texture = App->textures->Load("Assets/Sprites/Tetramino/Spritesheet/Block_Spritesheet.png");
	grid_texture = App->textures->Load("Assets/ss_grid.png");

	return true;
}

update_status ModuleTetronimo::Update() {

	runTime = SDL_GetTicks();
	deltaTime += runTime - lastTickTime;
	lastTickTime = runTime;

	//App->tetronimo->position.y += speed;

	for (int s = 21; s >= 0; s--) {
		int n = 0;
		for (int f = 0; f <= 9; f++) {
			if (type[f][s] == 'B') {
				n++;
			}

			if (lineCompleted(n, f, s) == true) 
			{
				App->sLvl_1->linesleft--; 
				App->sLvl_1->lines++;
				App->audio->PlayFx(App->sLvl_1->fxLine, 0);

				//Score por linia completada
				//1->50
				//2->150
				//3->400
				//4->900
				switch (combo)
				{
				case 1:
					App->render->TextDraw("Single  50", 64, 405, 0, 0, 255, 255, 16);
					break;
				case 2:
					App->render->TextDraw("Double 150", 64, 405, 0, 0, 255, 255, 16);
					break;
				case 3:
					App->render->TextDraw("Triple 400", 64, 405, 0, 0, 255, 255, 16);
					break;
				case 4:
					App->render->TextDraw("Tetris 900", 64, 405, 0, 0, 0, 0, 16);
					break;
				default:
					break;
				}
			}
		}
	}

	combo = 0;

	// Lost
	for (int f = 0; f <= 9; f++) {
		if (type[f][1] == 'B') {

			string s_loseContinue = to_string(v_loseContinue);
			const char* ch_loseContinue = s_loseContinue.c_str();

			//v_WinLose = 0;
			App->sLvl_1->lvl_lose(ch_loseContinue);
			LOG("HAS PERDIDO!");
			//ACABAR CON GAMEPLAY!
		}
	}

	// Block falling
	{//if (deltaTime > 500) {
	//	
	//	for (int i = 20; i >= 0; i--) {
	//		for (int j = 0; j <= 9; j++) {
	//			if (type[j][i] == 'P') {

	//				int aux = arr[j][i];
	//				type[j][i] = 'N';
	//				type[j][i + 1] = 'P';

	//				arr[j][i] = 0;
	//				arr[j][i + 1] = aux;
	//			}
	//		}
	//	}
	//	deltaTime = 0;
	//}
	}

	if (deltaTime > 1000) {
		int m = 0;

		for (int i = 20; i >= 0; i--) {
			for (int j = 0; j <= 9; j++) {
				if (type[j][i] == 'P') {

					int aux = arr[j][i];
					type[j][i] = 'N';
					type[j][i + 1] = 'P';

					arr[j][i] = 0;
					arr[j][i + 1] = aux;
					m = 1;
				}
			}
		}

		if (m == 1) {
			int stop = 0;

			for (int i = 0; i < MAX_TETRONIMOS; i++)
			{
				if ((tetronimos[i + 1] == nullptr) && (stop == 0))
				{
					tetronimos[i]->y += 10;
					tetronimos[i - 1]->y += 10;
					tetronimos[i - 2]->y += 10;
					tetronimos[i - 3]->y += 10;
					stop = 1;
				}
			}

			m = 0;
		}

		deltaTime = 0;
	}

	// Rotations
	if (App->input->keys[SDL_SCANCODE_R] == KEY_DOWN)
	{
		rotar++;
		int bloque = 0;

		if (num == 0) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						if (type[9][s] != 'P')
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f][s + 3] = 'N';

							type[f][s] = 'P';
							type[f + 1][s] = 'P';
							type[f + 2][s] = 'P';
							type[f + 3][s] = 'P';

							arr[f][s] = 0;
							arr[f][s + 1] = 0;
							arr[f][s + 2] = 0;
							arr[f][s + 3] = 0;

							arr[f][s] = 1;
							arr[f + 1][s] = 1;
							arr[f + 2][s] = 1;
							arr[f + 3][s] = 1;

							int stop = 0;

							for (int i = 0; i < MAX_TETRONIMOS; i++)
							{
								if ((tetronimos[i + 1] == nullptr) && (stop == 0))
								{
									tetronimos[i - 2]->x += 10;
									tetronimos[i - 2]->y -= 10;

									tetronimos[i - 1]->x += 20;
									tetronimos[i - 1]->y -= 20;

									tetronimos[i]->x += 30;
									tetronimos[i]->y -= 30;

									stop = 1;
								}
							}

							/*rectIdle1.x += 10;
							rectIdle2.x += 20;
							rectIdle3.x += 30;

							rectIdle1.y -= 10;
							rectIdle2.y -= 20;
							rectIdle3.y -= 30;*/
						}
						else
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f][s + 3] = 'N';

							type[f][s] = 'P';
							type[f - 1][s] = 'P';
							type[f - 2][s] = 'P';
							type[f - 3][s] = 'P';

							arr[f][s] = 0;
							arr[f][s + 1] = 0;
							arr[f][s + 2] = 0;
							arr[f][s + 3] = 0;

							arr[f][s] = 1;
							arr[f + 1][s] = 1;
							arr[f + 2][s] = 1;
							arr[f + 3][s] = 1;

							int stop = 0;

							for (int i = 0; i < MAX_TETRONIMOS; i++)
							{
								if ((tetronimos[i + 1] == nullptr) && (stop == 0))
								{
									tetronimos[i - 3]->x -= 30;

									tetronimos[i - 2]->x -= 20;
									tetronimos[i - 2]->y -= 10;

									tetronimos[i - 1]->x -= 10;
									tetronimos[i - 1]->y -= 20;

									tetronimos[i]->y -= 30;

									stop = 1;
								}
							}

							//rectIdle.x -= 30;
							//rectIdle1.x -= 20;
							//rectIdle2.x -= 10;

							//rectIdle1.y -= 10;
							//rectIdle2.y -= 20;
							//rectIdle3.y -= 30;
						}

						bloque++;
					}

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0))
					{
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f + 2][s] = 'N';
						type[f + 3][s] = 'N';

						type[f][s] = 'P';
						type[f][s + 1] = 'P';
						type[f][s + 2] = 'P';
						type[f][s + 3] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f + 2][s] = 0;
						arr[f + 3][s] = 0;

						arr[f][s] = 1;
						arr[f][s + 1] = 1;
						arr[f][s + 2] = 1;
						arr[f][s + 3] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i]->x -= 30;
								tetronimos[i]->y += 30;

								tetronimos[i - 1]->x -= 20;
								tetronimos[i - 1]->y += 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y += 10;

								stop = 1;
							}
						}

						//rectIdle1.x -= 10;
						//rectIdle2.x -= 20;
						//rectIdle3.x -= 30;

						//rectIdle1.y += 10;
						//rectIdle2.y += 20;
						//rectIdle3.y += 30;

						bloque++;
					}
				}
			}

			if (rotar == 2) {
				rotar = 0;
			}
		}

		if (num == 1) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						/*if (type[0][s] != 'P')
						{*/
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f - 1][s + 1] = 'N';
						type[f - 1][s + 2] = 'N';

						type[f - 2][s] = 'P';
						type[f - 1][s] = 'P';
						type[f - 1][s + 1] = 'P';
						type[f][s + 1] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f - 1][s + 1] = 0;
						arr[f - 1][s + 2] = 0;

						arr[f - 2][s] = 1;
						arr[f - 1][s] = 1;
						arr[f - 1][s + 1] = 1;
						arr[f][s + 1] = 1;


						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x -= 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y -= 10;

								tetronimos[i]->x += 10;
								tetronimos[i]->y -= 10;

								stop = 1;
							}
						}
						//}

						//if(type[0][s] == 'P')
						//{

						//	type[f][s] = 'N';
						//	type[f][s + 1] = 'N';
						//	type[f - 1][s + 1] = 'N';
						//	type[f - 1][s + 2] = 'N';

						//	type[f - 1][s] = 'P';
						//	type[f][s] = 'P';
						//	type[f][s + 1] = 'P';
						//	type[f + 1][s + 1] = 'P';

						//	arr[f][s] = 0;
						//	arr[f][s + 1] = 0;
						//	arr[f - 1][s + 1] = 0;
						//	arr[f - 1][s + 2] = 0;

						//	arr[f - 1][s] = 1;
						//	arr[f][s] = 1;
						//	arr[f][s + 1] = 1;
						//	arr[f + 1][s + 1] = 1;


						//	int stop = 0;

						//	for (int i = 0; i < MAX_TETRONIMOS; i++)
						//	{
						//		if ((tetronimos[i + 1] == nullptr) && (stop == 0))
						//		{
						//			tetronimos[i - 3]->x -= 10;

						//			tetronimos[i - 2]->y -= 10;

						//			tetronimos[i - 1]->x += 10;

						//			tetronimos[i]->x += 20;
						//			tetronimos[i]->y -= 10;

						//			stop = 1;
						//		}
						//	}
						//}

						///*rectIdle.x -= 20;
						//rectIdle1.x -= 10;
						//rectIdle3.x += 10;

						//rectIdle1.y -= 10;
						//rectIdle3.y -= 10;*/

						bloque++;
					}

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f + 1][s + 1] = 'N';
						type[f + 2][s + 1] = 'N';

						type[f + 2][s] = 'P';
						type[f + 2][s + 1] = 'P';
						type[f + 1][s + 1] = 'P';
						type[f + 1][s + 2] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f + 1][s + 1] = 0;
						arr[f + 2][s + 1] = 0;

						arr[f + 2][s] = 1;
						arr[f + 2][s + 1] = 1;
						arr[f + 1][s + 1] = 1;
						arr[f + 1][s + 2] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x += 20;

								tetronimos[i - 2]->x += 10;
								tetronimos[i - 2]->y += 10;

								tetronimos[i]->x -= 10;
								tetronimos[i]->y += 10;

								stop = 1;
							}
						}

						//rectIdle.x += 20;
						//rectIdle1.x += 10;
						//rectIdle3.x -= 10;

						//rectIdle1.y += 10;
						//rectIdle3.y += 10;

						bloque++;
					}
				}
			}

			if (rotar == 2) {
				rotar = 0;
			}
		}

		if (num == 2) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 4) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f + 1][s + 1] = 'N';
						type[f + 1][s + 2] = 'N';

						type[f][s] = 'P';
						type[f - 1][s] = 'P';
						type[f + 1][s] = 'P';
						type[f - 1][s + 1] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f + 1][s + 1] = 0;
						arr[f + 1][s + 2] = 0;

						arr[f][s] = 1;
						arr[f - 1][s] = 1;
						arr[f + 1][s] = 1;
						arr[f - 1][s + 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x -= 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y -= 10;

								tetronimos[i - 1]->y -= 20;

								tetronimos[i]->x -= 10;
								tetronimos[i]->y += 10;

								stop = 1;
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
					if ((type[f][s] == 'P') && (rotar == 3) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f - 1][s + 1] = 'N';
						type[f - 2][s + 1] = 'N';

						type[f][s] = 'P';
						type[f][s + 1] = 'P';
						type[f][s - 1] = 'P';
						type[f - 1][s - 1] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f - 1][s + 1] = 0;
						arr[f - 2][s + 1] = 0;

						arr[f][s] = 1;
						arr[f][s + 1] = 1;
						arr[f][s - 1] = 1;
						arr[f - 1][s - 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->y -= 20;

								tetronimos[i - 2]->x += 10;
								tetronimos[i - 2]->y -= 10;

								tetronimos[i - 1]->x += 20;

								tetronimos[i]->x -= 10;
								tetronimos[i]->y -= 10;

								stop = 1;
							}
						}

						/*rectIdle1.x += 10;
						rectIdle2.x += 20;
						rectIdle3.x -= 10;

						rectIdle.y -= 20;
						rectIdle1.y -= 10;
						rectIdle3.y -= 10;*/

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f][s + 2] = 'N';
						type[f + 1][s + 2] = 'N';

						type[f][s + 2] = 'P';
						type[f + 1][s + 2] = 'P';
						type[f + 2][s + 2] = 'P';
						type[f + 2][s + 1] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f][s + 2] = 0;
						arr[f + 1][s + 2] = 0;

						arr[f][s + 2] = 1;
						arr[f + 1][s + 2] = 1;
						arr[f + 2][s + 2] = 1;
						arr[f + 2][s + 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x += 20;

								tetronimos[i - 2]->x += 10;
								tetronimos[i - 2]->y += 10;

								tetronimos[i - 1]->y += 20;

								tetronimos[i]->x += 10;
								tetronimos[i]->y -= 10;

								stop = 1;
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

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f + 2][s] = 'N';
						type[f][s + 1] = 'N';

						type[f][s] = 'P';
						type[f][s + 1] = 'P';
						type[f][s + 2] = 'P';
						type[f + 1][s + 2] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f + 2][s] = 0;
						arr[f][s + 1] = 0;

						arr[f][s] = 1;
						arr[f][s + 1] = 1;
						arr[f][s + 2] = 1;
						arr[f + 1][s + 2] = 1;

						//rectIdle1.x -= 10;
						//rectIdle2.x -= 20;
						//rectIdle3.x += 10;

						//rectIdle.y += 20;
						//rectIdle1.y += 10;
						//rectIdle3.y += 10;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->y += 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y += 10;

								tetronimos[i - 1]->x -= 20;

								tetronimos[i]->x += 10;
								tetronimos[i]->y += 10;

								stop = 1;
							}
						}

						bloque++;
					}
				}
			}

			if (rotar == 4) {
				rotar = 0;
			}
		}

		if (num == 3) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 4) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f][s + 2] = 'N';
						type[f - 1][s + 2] = 'N';

						type[f][s] = 'P';
						type[f - 1][s] = 'P';
						type[f - 2][s] = 'P';
						type[f][s + 1] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f][s + 2] = 0;
						arr[f - 1][s + 2] = 0;

						arr[f][s] = 1;
						arr[f - 1][s] = 1;
						arr[f - 2][s] = 1;
						arr[f][s + 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x -= 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y -= 10;

								tetronimos[i - 1]->y -= 20;

								tetronimos[i]->x += 10;
								tetronimos[i]->y -= 10;

								stop = 1;
							}
						}

						/*rectIdle.x -= 20;
						rectIdle1.x -= 10;
						rectIdle3.x += 10;

						rectIdle1.y -= 10;
						rectIdle2.y -= 20;
						rectIdle3.y -= 10;*/

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 3) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f + 1][s + 1] = 'N';
						type[f + 2][s + 1] = 'N';

						type[f + 2][s + 1] = 'P';
						type[f + 2][s] = 'P';
						type[f + 2][s - 1] = 'P';
						type[f + 1][s + 1] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f + 1][s + 1] = 0;
						arr[f + 2][s + 1] = 0;

						arr[f + 2][s + 1] = 1;
						arr[f + 2][s] = 1;
						arr[f + 2][s - 1] = 1;
						arr[f + 1][s + 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->y -= 20;

								tetronimos[i - 2]->x += 10;
								tetronimos[i - 2]->y -= 10;

								tetronimos[i - 1]->x += 20;

								tetronimos[i]->x += 10;
								tetronimos[i]->y += 10;

								stop = 1;
							}
						}

						//rectIdle1.x += 10;
						//rectIdle2.x += 20;
						//rectIdle3.x += 10;

						//rectIdle.y -= 20;
						//rectIdle1.y -= 10;
						//rectIdle3.y += 10;

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f][s + 1] = 'N';
						type[f][s + 2] = 'N';

						type[f][s + 2] = 'P';
						type[f][s + 1] = 'P';
						type[f + 1][s + 2] = 'P';
						type[f + 2][s + 2] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f][s + 1] = 0;
						arr[f][s + 2] = 0;

						arr[f][s + 2] = 1;
						arr[f][s + 1] = 1;
						arr[f + 1][s + 2] = 1;
						arr[f + 2][s + 2] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x += 20;

								tetronimos[i - 2]->x += 10;
								tetronimos[i - 2]->y += 10;

								tetronimos[i - 1]->y += 20;

								tetronimos[i]->x -= 10;
								tetronimos[i]->y += 10;

								stop = 1;
							}
						}

						/*rectIdle.x += 20;
						rectIdle1.x += 10;
						rectIdle3.x -= 10;


						rectIdle1.y += 10;
						rectIdle2.y += 20;
						rectIdle3.y += 10;*/

						bloque++;
					}

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f + 2][s] = 'N';
						type[f + 2][s + 1] = 'N';

						type[f][s] = 'P';
						type[f + 1][s] = 'P';
						type[f][s + 1] = 'P';
						type[f][s + 2] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f + 2][s] = 0;
						arr[f + 2][s + 1] = 0;

						arr[f][s] = 1;
						arr[f + 1][s] = 1;
						arr[f][s + 1] = 1;
						arr[f][s + 2] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->y += 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y += 10;

								tetronimos[i - 1]->x -= 20;

								tetronimos[i]->x -= 10;
								tetronimos[i]->y -= 10;

								stop = 1;
							}
						}

						/*rectIdle1.x -= 10;
						rectIdle2.x -= 20;
						rectIdle3.x -= 10;

						rectIdle.y += 20;
						rectIdle1.y += 10;
						rectIdle3.y -= 10;*/

						bloque++;
					}
				}
			}

			if (rotar == 4) {
				rotar = 0;
			}
		}

		if (num == 4) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 4) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f][s + 2] = 'N';
						type[f - 1][s + 1] = 'N';

						type[f][s] = 'P';
						type[f - 1][s] = 'P';
						type[f - 2][s] = 'P';
						type[f - 1][s + 1] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f][s + 2] = 0;
						arr[f - 1][s + 1] = 0;

						arr[f][s] = 1;
						arr[f - 1][s] = 1;
						arr[f - 2][s] = 1;
						arr[f - 1][s + 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x -= 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y -= 10;

								tetronimos[i - 1]->y -= 20;

								stop = 1;
							}
						}

						//rectIdle.x -= 20;
						//rectIdle1.x -= 10;

						//rectIdle1.y -= 10;
						//rectIdle2.y -= 20;

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 3) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f - 1][s + 1] = 'N';
						type[f + 1][s + 1] = 'N';
						type[f][s - 1] = 'N';

						type[f + 1][s] = 'P';
						type[f][s + 1] = 'P';
						type[f + 1][s + 1] = 'P';
						type[f + 1][s + 2] = 'P';

						arr[f][s] = 0;
						arr[f - 1][s + 1] = 0;
						arr[f + 1][s + 1] = 0;
						arr[f][s - 1] = 0;

						arr[f + 1][s] = 1;
						arr[f][s + 1] = 1;
						arr[f + 1][s + 1] = 1;
						arr[f + 1][s + 2] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->y -= 20;

								tetronimos[i - 2]->x += 10;
								tetronimos[i - 2]->y -= 10;

								tetronimos[i - 1]->x += 20;

								stop = 1;
							}
						}

						/*rectIdle1.x += 10;
						rectIdle2.x += 20;

						rectIdle.y -= 20;
						rectIdle1.y -= 10;*/

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f][s + 2] = 'N';
						type[f + 1][s + 1] = 'N';

						type[f][s + 1] = 'P';
						type[f + 1][s + 1] = 'P';
						type[f + 2][s + 1] = 'P';
						type[f + 1][s] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f][s + 2] = 0;
						arr[f + 1][s + 1] = 0;

						arr[f][s + 1] = 1;
						arr[f + 1][s + 1] = 1;
						arr[f + 2][s + 1] = 1;
						arr[f + 1][s] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x += 20;

								tetronimos[i - 2]->x += 10;
								tetronimos[i - 2]->y += 10;

								tetronimos[i - 1]->y += 20;

								stop = 1;
							}
						}

						/*rectIdle.x += 20;
						rectIdle1.x += 10;

						rectIdle1.y += 10;
						rectIdle2.y += 20;*/

						bloque++;
					}

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f + 2][s] = 'N';
						type[f + 1][s + 1] = 'N';

						type[f][s] = 'P';
						type[f][s + 1] = 'P';
						type[f][s + 2] = 'P';
						type[f + 1][s + 1] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f + 2][s] = 0;
						arr[f + 1][s + 1] = 0;

						arr[f][s] = 1;
						arr[f][s + 1] = 1;
						arr[f][s + 2] = 1;
						arr[f + 1][s + 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->y += 20;

								tetronimos[i - 2]->x -= 10;
								tetronimos[i - 2]->y += 10;

								tetronimos[i - 1]->x -= 20;

								stop = 1;
							}
						}

						/*rectIdle1.x -= 10;
						rectIdle2.x -= 20;

						rectIdle.y += 20;
						rectIdle1.y += 10;*/

						bloque++;
					}
				}
			}

			if (rotar == 4) {
				rotar = 0;
			}
		}

		//if(num == 5) -> No hace nada 

		if (num == 6) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f][s + 1] = 'N';
						type[f + 1][s + 1] = 'N';
						type[f + 1][s + 2] = 'N';

						type[f][s] = 'P';
						type[f + 1][s] = 'P';
						type[f + 1][s - 1] = 'P';
						type[f + 2][s - 1] = 'P';

						arr[f][s] = 0;
						arr[f][s + 1] = 0;
						arr[f + 1][s + 1] = 0;
						arr[f + 1][s + 2] = 0;

						arr[f][s] = 1;
						arr[f + 1][s] = 1;
						arr[f + 1][s - 1] = 1;
						arr[f + 2][s - 1] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i - 3]->x -= 10;
								tetronimos[i - 3]->y += 20;

								tetronimos[i - 2]->y += 10;

								tetronimos[i - 1]->x -= 10;

								tetronimos[i]->y -= 10;

								stop = 1;
							}
						}

						//rectIdle.x -= 10;
						//rectIdle2.x -= 10;

						//rectIdle.y += 20;
						//rectIdle1.y += 10;
						//rectIdle3.y -= 10;

						bloque++;
					}

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f][s + 1] = 'N';
						type[f - 1][s + 1] = 'N';

						type[f - 1][s] = 'P';
						type[f - 1][s + 1] = 'P';
						type[f][s + 1] = 'P';
						type[f][s + 2] = 'P';

						arr[f][s] = 0;
						arr[f + 1][s] = 0;
						arr[f][s + 1] = 0;
						arr[f - 1][s + 1] = 0;

						arr[f - 1][s] = 1;
						arr[f - 1][s + 1] = 1;
						arr[f][s + 1] = 1;
						arr[f][s + 2] = 1;

						int stop = 0;

						for (int i = 0; i < MAX_TETRONIMOS; i++)
						{
							if ((tetronimos[i + 1] == nullptr) && (stop == 0))
							{
								tetronimos[i]->y += 10;

								tetronimos[i - 1]->x += 10;

								tetronimos[i - 2]->y -= 10;

								tetronimos[i - 3]->x += 10;
								tetronimos[i - 3]->y -= 20;

								stop = 1;
							}
						}
						/*rectIdle.x += 10;
						rectIdle2.x += 10;

						rectIdle.y -= 20;
						rectIdle1.y -= 10;
						rectIdle3.y += 10;*/

						bloque++;
					}
				}
			}

			if (rotar == 2) {
				rotar = 0;
			}
		}
	}

	// Left movement
	if (App->input->keys[SDL_SCANCODE_A] == KEY_DOWN)
	{

		for (int s = 0; s <= 21; s++)
		{
			for (int f = 0; f <= 9; f++)
			{
				if (type[0][s] == 'P')
				{
					move = 1;
				}

				if (f != 0)
				{
					if ((type[f - 1][s] == 'B') && (type[f][s] == 'P')) {
						move = 1;
					}
				}
			}
		}

		for (int s = 0; s <= 21; s++) {
			for (int f = 0; f <= 9; f++) {
				if ((type[f][s] == 'P') && (move != 1)) {
					type[f - 1][s] = 'P';
					type[f][s] = 'N';

					arr[f - 1][s] = 1;
					arr[f][s] = 0;
				}
			}
		}

		if (move != 1) {
			int stop = 0;

			for (int i = 0; i < MAX_TETRONIMOS; i++)
			{
				if ((tetronimos[i + 1] == nullptr) && (stop == 0))
				{
					tetronimos[i]->x -= 10;
					tetronimos[i - 1]->x -= 10;
					tetronimos[i - 2]->x -= 10;
					tetronimos[i - 3]->x -= 10;
					stop = 1;
				}
			}
		}

		move = 0;
	}

	// Right movement
	if (App->input->keys[SDL_SCANCODE_D] == KEY_DOWN)
	{

		for (int s = 0; s <= 21; s++)
		{
			for (int f = 9; f >= 0; f--)
			{
				if (type[9][s] == 'P')
				{
					move = 1;
				}

				if (f != 9)
				{
					if ((type[f + 1][s] == 'B') && (type[f][s] == 'P')) {
						move = 1;
					}
				}
			}
		}

		for (int s = 0; s <= 21; s++) {
			for (int f = 9; f >= 0; f--) {
				if ((type[f][s] == 'P') && (move != 1)) {
					type[f + 1][s] = 'P';
					type[f][s] = 'N';

					arr[f + 1][s] = 1;
					arr[f][s] = 0;
				}
			}
		}

		if (move != 1) {
			int stop = 0;

			for (int i = 0; i < MAX_TETRONIMOS; i++)
			{
				if ((tetronimos[i + 1] == nullptr) && (stop == 0))
				{
					tetronimos[i]->x += 10;
					tetronimos[i - 1]->x += 10;
					tetronimos[i - 2]->x += 10;
					tetronimos[i - 3]->x += 10;
					stop = 1;
				}
			}
		}

		move = 0;
	}


	// Fall fast (to do)
	if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT)
	{
		if (deltaTime > 1000) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P')) {
						type[f][s + 1] = 'P';
						type[f][s] = 'N';

						arr[f][s + 1] = 1;
						arr[f][s] = 0;
						SDL_Delay(200);
					}
				}
			}
			deltaTime = 0;
		}
	}

	Debugging();

	return update_status::UPDATE_CONTINUE;
}

void ModuleTetronimo::Debugging()
{
	string s_loseContinue = to_string(v_loseContinue);
	const char* ch_loseContinue = s_loseContinue.c_str();

	if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN)//(KEY_REPEAT)		// to fix somehow
	{
		/*(App->player->godMode == false) ? App->player->godMode = true : App->player->godMode = false;*/
		/*App->player->godMode != App->player->godMode;*/
		
		App->player->godMode = true;	
	}

	if (App->input->keys[SDL_SCANCODE_F6] == KEY_DOWN)//(KEY_REPEAT)		
	{

		App->player->godMode = false; //Muy cutre, no he encontrado la manera de desactivarlo con el mismo boton
	}
	
	// Manually spawn a block
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN)
	{
		rotar = 0;
		SpawnTetronimo();
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

	// LOG info
	if (App->input->keys[SDL_SCANCODE_F5] == KEY_DOWN)
	{
		Print();
	}

	// GodMode
	if (App->player->godMode == true)
	{
		LOG("GodMode on");
		App->render->Blit(grid_texture, 62, 50, NULL);
	}

	if (lvl_instaLose == true)
	{
		//LOG("Instant Lost!");
		App->audio->PauseMusic();
		App->sLvl_1->lvl_lose(ch_loseContinue);
	}
	if (lvl_instaWin == true)
	{
		App->sLvl_1->v_message = 0;
		//LOG("Level complete (Instant Win)!");
		App->audio->PauseMusic();
		App->sLvl_1->lvl_win();
	}
}

bool ModuleTetronimo::lineCompleted(int n, int f, int s)
{
	if (n == 10) {
		type[f][s] = 'N';
		type[f - 1][s] = 'N';
		type[f - 2][s] = 'N';
		type[f - 3][s] = 'N';
		type[f - 4][s] = 'N';
		type[f - 5][s] = 'N';
		type[f - 6][s] = 'N';
		type[f - 7][s] = 'N';
		type[f - 8][s] = 'N';
		type[f - 9][s] = 'N';

		for (int m = s; m >= 0; m--) {
			for (int j = 0; j <= 9; j++) {
				if (type[j][s - 1] == 'B') {
					type[j][s - 1] = 'N';
					type[j][s] = 'B';
				}
			}
		}
		combo++;
		return true;
	}
	return false;
}

int ModuleTetronimo::blockRB()
{
	// Block reached bottom
	for (int f = 0; f <= 9; f++) {
		if (type[f][21] == 'P') {

			App->audio->PlayFx(App->sLvl_1->fxBlock_Fall, 0);
			Change();
			SpawnTetronimo();
			return 0;
		}
	}

	// Block stop
	for (int s = 0; s <= 21; s++) {
		for (int f = 0; f <= 9; f++) {
			if ((type[f][s] == 'P')) {
				if (type[f][s + 1] == 'B') {
					App->audio->PlayFx(App->sLvl_1->fxLine, 0);
					Change();
					SpawnTetronimo();
					return (21 - s);
				}
			}
		}
	}

	return -1;
}

update_status ModuleTetronimo::PostUpdate() {
	for (int i = 0; i < MAX_TETRONIMOS; ++i)
	{
		if (tetronimos[i] != nullptr) {

			App->render->DrawQuad(*tetronimos[i], *red[i], *green[i], *blue[i], 255);

		}
	}

	//rectIdle = currentAnimation->GetCurrentSprite();
	//SDL_Rect rect1 = currentAnimation1->GetCurrentSprite();
	//SDL_Rect rect2 = currentAnimation2->GetCurrentSprite();
	//SDL_Rect rect3 = currentAnimation3->GetCurrentSprite();

	/*apply_surface(0, 0, dots, screen, &rectIdle);
	apply_surface(200, 0, dots, screen, &rectIdle1);
	apply_surface(0, 200, dots, screen, &rectIdle2);
	apply_surface(200, 200, dots, screen, &rectIdle3);*/

	//App->render->Blit(texture, rectIdle.position.x, rectIdle.position.y, &rect);
	//App->render->Blit(texture, rectIdle1.position.x, rectIdle1.position.y, &rect1);
	//App->render->Blit(texture, rectIdle2.position.x, rectIdle2.position.y, &rect2);
	//App->render->Blit(texture, rectIdle3.position.x, rectIdle3.position.y, &rect3);

	return update_status::UPDATE_CONTINUE;
}

void ModuleTetronimo::Spawn() {

	SDL_Rect Block[4];

	if (num == 0)
	{
		arr[5][0] = 1;
		arr[6][0] = 1;
		arr[7][0] = 1;
		arr[8][0] = 1;

		type[5][0] = 'P';
		type[6][0] = 'P';
		type[7][0] = 'P';
		type[8][0] = 'P';


		Block[0] = { 150, 30, 9, 9 };
		Block[1] = { 160, 30, 9, 9 };
		Block[2] = { 170, 30, 9, 9 };
		Block[3] = { 180, 30, 9, 9 };

		rectIdle = Block[0];
		rectIdle1 = Block[1];
		rectIdle2 = Block[2];
		rectIdle3 = Block[3];

		r = 255;
		g = 0;
		b = 0;
	}

	if (num == 1)
	{
		arr[5][0] = 1;
		arr[6][0] = 1;
		arr[6][1] = 1;
		arr[7][1] = 1;

		type[5][0] = 'P';
		type[6][0] = 'P';
		type[6][1] = 'P';
		type[7][1] = 'P';

		Block[0] = { 150, 30, 9, 9 };
		Block[1] = { 160, 30, 9, 9 };
		Block[2] = { 160, 40, 9, 9 };
		Block[3] = { 170, 40, 9, 9 };

		rectIdle = Block[0];
		rectIdle1 = Block[1];
		rectIdle2 = Block[2];
		rectIdle3 = Block[3];

		r = 255;
		g = 152;
		b = 2;
	}

	if (num == 2)
	{
		arr[5][0] = 1;
		arr[6][0] = 1;
		arr[7][0] = 1;
		arr[5][1] = 1;

		type[5][0] = 'P';
		type[6][0] = 'P';
		type[7][0] = 'P';
		type[5][1] = 'P';

		Block[0] = { 150, 30, 9, 9 };
		Block[1] = { 160, 30, 9, 9 };
		Block[2] = { 170, 30, 9, 9 };
		Block[3] = { 150, 40, 9, 9 };

		rectIdle = Block[0];
		rectIdle1 = Block[1];
		rectIdle2 = Block[2];
		rectIdle3 = Block[3];

		r = 255;
		g = 0;
		b = 255;
	}

	if (num == 3)
	{
		arr[5][0] = 1;
		arr[6][0] = 1;
		arr[7][0] = 1;
		arr[7][1] = 1;

		type[5][0] = 'P';
		type[6][0] = 'P';
		type[7][0] = 'P';
		type[7][1] = 'P';

		Block[0] = { 150, 30, 9, 9 };
		Block[1] = { 160, 30, 9, 9 };
		Block[2] = { 170, 30, 9, 9 };
		Block[3] = { 170, 40, 9, 9 };

		rectIdle = Block[0];
		rectIdle1 = Block[1];
		rectIdle2 = Block[2];
		rectIdle3 = Block[3];

		r = 249;
		g = 177;
		b = 7;
	}

	if (num == 4)
	{
		arr[5][0] = 1;
		arr[6][0] = 1;
		arr[7][0] = 1;
		arr[6][1] = 1;

		type[5][0] = 'P';
		type[6][0] = 'P';
		type[7][0] = 'P';
		type[6][1] = 'P';

		Block[0] = { 150, 30, 9, 9 };
		Block[1] = { 160, 30, 9, 9 };
		Block[2] = { 170, 30, 9, 9 };
		Block[3] = { 160, 40, 9, 9 };

		rectIdle = Block[0];
		rectIdle1 = Block[1];
		rectIdle2 = Block[2];
		rectIdle3 = Block[3];

		r = 0;
		g = 161;
		b = 0;
	}

	if (num == 5)
	{
		arr[5][0] = 1;
		arr[6][0] = 1;
		arr[5][1] = 1;
		arr[6][1] = 1;

		type[5][0] = 'P';
		type[6][0] = 'P';
		type[5][1] = 'P';
		type[6][1] = 'P';

		Block[0] = { 150, 30, 9, 9 };
		Block[1] = { 160, 30, 9, 9 };
		Block[2] = { 150, 40, 9, 9 };
		Block[3] = { 160, 40, 9, 9 };

		rectIdle = Block[0];
		rectIdle1 = Block[1];
		rectIdle2 = Block[2];
		rectIdle3 = Block[3];

		r = 16;
		g = 50;
		b = 229;
	}

	if (num == 6)
	{
		arr[5][1] = 1;
		arr[6][1] = 1;
		arr[6][0] = 1;
		arr[7][0] = 1;

		type[5][1] = 'P';
		type[6][1] = 'P';
		type[6][0] = 'P';
		type[7][0] = 'P';

		Block[0] = { 150, 40, 9, 9 };
		Block[1] = { 160, 40, 9, 9 };
		Block[2] = { 160, 30, 9, 9 };
		Block[3] = { 170, 30, 9, 9 };

		rectIdle = Block[0];
		rectIdle1 = Block[1];
		rectIdle2 = Block[2];
		rectIdle3 = Block[3];

		r = 0;
		g = 163;
		b = 173;
	}

	AddTetronimo(rectIdle, r, g, b);
	AddTetronimo(rectIdle1, r, g, b);
	AddTetronimo(rectIdle2, r, g, b);
	AddTetronimo(rectIdle3, r, g, b);

	SDL_Delay(600);

}

void ModuleTetronimo::SpawnTetronimo() {

	bool const r = (srand(time(NULL)), true);
	int next = rand() % 7;

	LOG("NUM: %d", num);
	LOG("NEXT: %d", next);

	Spawn();
	//num = next;
}

void ModuleTetronimo::Print() {
	for (int s = 0; s <= 21; s++) {
		for (int f = 0; f <= 9; f++) {
			LOG("%c, %d, %d", type[f][s], f, s);
		}
		LOG("\n");
	}
}

void ModuleTetronimo::Change() {
	for (int m = 0; m <= 21; m++) {
		for (int l = 0; l <= 9; l++) {
			if ((type[l][m] == 'P')) {
				type[l][m] = 'B';
			}
		}
	}
	num = next;
}

void ModuleTetronimo::AddTetronimo(const SDL_Rect& tetronimo, const int& r, const int& g, const int& b)
{
	for (int i = 0; i < MAX_TETRONIMOS; ++i)
	{
		if (tetronimos[i] == nullptr)
		{
			int* red1 = new int(r);
			int* green1 = new int(g);
			int* blue1 = new int(b);

			red[i] = red1;
			green[i] = green1;
			blue[i] = blue1;

			SDL_Rect* t = new SDL_Rect(tetronimo);

			tetronimos[i] = t;
			break;
		}
	}
}

bool ModuleTetronimo::CleanUp()
{
	App->textures->Unload(texture);
	App->textures->Unload(grid_texture);

	//Eliminar bloques creados como hacia con los enemigos creados en la solucion 8

	//LOG("Freeing all enemies");

	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (enemies[i] != nullptr)
	//	{
	//		delete enemies[i];
	//		enemies[i] = nullptr;
	//	}
	//}

	return true;
}