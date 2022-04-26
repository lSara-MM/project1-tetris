#include "ModuleTetronimo.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ScreenLvl_1.h"
#include "ModuleAudio.h"

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

uint runTime = 0;
uint deltaTime = 0;
uint lastTickTime = 0;

ModuleTetronimo::ModuleTetronimo(bool startEnabled) : Module(startEnabled) {

	currentAnimation = &idleAnim;
}


ModuleTetronimo::~ModuleTetronimo() {



}

bool ModuleTetronimo::Start() {

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 10; j++) {
			arr[j][i] = 0;
			type[j][i] = 'N';
		}
	}

	combo = 0;


	lvl_instaLose = false;
	lvl_instaWin = false;

	v_loseContinue = 9;
	v_WinLose = 0;
	texture = App->textures->Load("Assets/Sprites/Tetramino/Spritesheet/Block_Spritesheet.png");
	return true;
}

update_status ModuleTetronimo::Update() {

	runTime = SDL_GetTicks();
	deltaTime += runTime - lastTickTime;
	lastTickTime = runTime;

	currentAnimation = &idleAnim; 

	App->tetronimo->position.y += speed;

	

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

	// Block fell down
	//blockRB();


	// Block falling
	if (deltaTime > 500) {
		for (int i = 20; i >= 0; i--) {
			for (int j = 0; j <= 9; j++) {
				if (type[j][i] == 'P') {

					int aux = arr[j][i];
					type[j][i] = 'N';
					type[j][i + 1] = 'P';

					arr[j][i] = 0;
					arr[j][i + 1] = aux;
				}
			}
		}
		deltaTime = 0;
	}

	// Rotations
	if (App->input->keys[SDL_SCANCODE_RETURN] == KEY_DOWN)
	{
		rotar++;
		int bloque = 0;

		if (num == 0) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {

						if (type[9][s] != 'P') {
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
						}
						
						bloque++;
					}

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0)) {
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
		int move = 0;		// move == 1 No se puede mover // move == 0 Si se puede mover

		for (int s = 0; s <= 21; s++) {
			for (int f = 9; f >= 0; f--) {
				if (type[0][s] == 'P') {
					move = 1;
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
	}

	// Right movement
	if (App->input->keys[SDL_SCANCODE_D] == KEY_DOWN)
	{
		int move = 0;

		for (int s = 0; s <= 21; s++) {
			for (int f = 9; f >= 0; f--) {
				if (type[9][s] == 'P') {
					move = 1;
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

	currentAnimation->Update();

	Debugging();

	return update_status::UPDATE_CONTINUE;
}

void ModuleTetronimo::Debugging()
{
	string s_loseContinue = to_string(v_loseContinue);
	const char* ch_loseContinue = s_loseContinue.c_str();

	// Manually spawn a block
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN)
	{
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
			App->audio->PlayFx(App->sLvl_1->fxLine, 0);
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

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y - rect.h, &rect);

	return update_status::UPDATE_CONTINUE;
}

void ModuleTetronimo::Spawn() {

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
	}

	SDL_Delay(600);
}

void ModuleTetronimo::SpawnTetronimo() {

	bool const r = (srand(time(NULL)), true);
	int next = rand() % 7;

	LOG("NUM: %d", num);
	LOG("NEXT: %d", next);

	Spawn();
	num = next;
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
}


bool ModuleTetronimo::CleanUp()
{
	App->textures->Unload(texture);

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