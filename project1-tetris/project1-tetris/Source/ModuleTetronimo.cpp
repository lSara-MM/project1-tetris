#include "ModuleTetronimo.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ScreenLvl_1.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"

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
int auxiliarColor = 0;

SDL_Surface* dots = NULL;
SDL_Surface* screen = NULL;

SDL_Rect Vacio = { 0, 0, 0, 0 };

//The event structure
SDL_Event event;

//The portions of the sprite map to be blitted
uint runTime = 0;
uint deltaTime = 0;
uint lastTickTime = 0;

ModuleTetronimo::ModuleTetronimo(bool startEnabled) : Module(startEnabled) {

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

	LOG("Loading grid_texture");
	//texture = App->textures->Load("Assets/Sprites/Tetramino/Spritesheet/Block_Spritesheet.png");
	grid_texture = App->textures->Load("Assets/ss_grid.png");


	//
	v_loseContinue = 9;
	v_WinLose = 0;

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
	//for (int f = 0; f <= 9; f++) {
	//	if (type[f][1] == 'B') {

	//		string s_loseContinue = to_string(App->sLvl_1->v_loseContinue);
	//		const char* ch_loseContinue = s_loseContinue.c_str();

	//		App->audio->PauseMusic();
	//		App->sLvl_1->v_WinLose = 0;
	//		App->sLvl_1->lvl_lose(ch_loseContinue);
	//		LOG("HAS PERDIDO!");

	//		//ACABAR CON GAMEPLAY!
	//	}
	//}

	for (int f = 0; f <= 9; f++) {
		if (type[f][1] == 'B') {

			/*string s_loseContinue = to_string(v_loseContinue);
			const char* ch_loseContinue = s_loseContinue.c_str();*/

			//App->audio->PauseMusic();
			//v_WinLose = 0;
			//lvl_lose(ch_loseContinue);

			//App->sLvl_1->lvl_instaLose = true;
			LOG("ACTIVE");

			App->sLvl_1->lvl_instaLose = true;

			//ACABAR CON GAMEPLAY!
		}
	}

	// Block falling
	if (deltaTime > 200) {
		int m = 0;

		for (int i = 20; i >= 0; i--) {
			for (int j = 0; j <= 9; j++) {
				if (type[j][i] == 'P') {

					int aux = arr[j][i];
					type[j][i] = 'N';
					type[j][i + 1] = 'P';

					arr[j][i] = 0;
					arr[j][i + 1] = aux;

					tetronimos[j][i + 1] = tetronimos[j][i];
					tetronimos[j][i] = Vacio;
					tetronimos[j][i + 1].y += (T_HEIGHT + 1);

					red[j][i] = 0;
					red[j][i + 1] = r;

					green[j][i] = 0;
					green[j][i + 1] = g;

					blue[j][i] = 0;
					blue[j][i + 1] = b;
				}
			}
		}

		deltaTime = 0;
	}

	// Rotations
	if (App->input->keys[SDL_SCANCODE_R] == KEY_DOWN)
	{
		rotar++;
		int bloque = 0;

		// red
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

							tetronimos[f + 1][s] = tetronimos[f][s + 1];
							tetronimos[f][s + 1] = Vacio;
							tetronimos[f + 1][s].x += (T_WIDTH + 1);
							tetronimos[f + 1][s].y -= (T_HEIGHT + 1);

							tetronimos[f + 2][s] = tetronimos[f][s + 2];
							tetronimos[f][s + 2] = Vacio;
							tetronimos[f + 2][s].x += (T_WIDTH + 1) * 2;
							tetronimos[f + 2][s].y -= (T_HEIGHT + 1) * 2;

							tetronimos[f + 3][s] = tetronimos[f][s + 3];
							tetronimos[f][s + 3] = Vacio;
							tetronimos[f + 3][s].x += (T_WIDTH + 1) * 3;
							tetronimos[f + 3][s].y -= (T_HEIGHT + 1) * 3;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f + 1][s] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 2][s] = auxiliarColor;

							auxiliarColor = red[f][s + 3];
							red[f][s + 3] = 0;

							red[f + 3][s] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f + 1][s] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 2][s] = auxiliarColor;

							auxiliarColor = green[f][s + 3];
							green[f][s + 3] = 0;

							green[f + 3][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f + 1][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 2][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 3];
							blue[f][s + 3] = 0;

							blue[f + 3][s] = auxiliarColor;

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

							tetronimos[f - 3][s] = tetronimos[f][s];
							tetronimos[f][s] = Vacio;
							tetronimos[f - 3][s].x -= (T_WIDTH + 1) * 3;

							tetronimos[f - 1][s] = tetronimos[f][s + 1];
							tetronimos[f][s + 1] = Vacio;
							tetronimos[f - 1][s].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s].y -= (T_HEIGHT + 1);

							tetronimos[f - 2][s] = tetronimos[f][s + 2];
							tetronimos[f][s + 2] = Vacio;
							tetronimos[f - 2][s].x -= (T_WIDTH + 1) * 2;
							tetronimos[f - 2][s].y -= (T_HEIGHT + 1) * 2;

							tetronimos[f][s] = tetronimos[f][s + 3];
							tetronimos[f][s + 3] = Vacio;
							tetronimos[f][s].y -= (T_HEIGHT + 1) * 3;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f - 2][s] = auxiliarColor;

							auxiliarColor = red[f][s + 3];
							red[f][s + 3] = 0;

							red[f - 3][s] = auxiliarColor;


							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f - 2][s] = auxiliarColor;

							auxiliarColor = green[f][s + 3];
							green[f][s + 3] = 0;

							green[f - 3][s] = auxiliarColor;


							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f - 2][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 3];
							blue[f][s + 3] = 0;

							blue[f - 3][s] = auxiliarColor;

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

						tetronimos[f][s + 3] = tetronimos[f + 3][s];
						tetronimos[f][s + 3].x -= (T_HEIGHT + 1) * 3;
						tetronimos[f][s + 3].y += (T_HEIGHT + 1) * 3;
						tetronimos[f + 3][s] = Vacio;

						tetronimos[f][s + 2] = tetronimos[f + 2][s];
						tetronimos[f][s + 2].x -= (T_WIDTH + 1) * 2;
						tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
						tetronimos[f + 2][s] = Vacio;

						tetronimos[f][s + 1] = tetronimos[f + 1][s];
						tetronimos[f][s + 1].x -= (T_WIDTH + 1);
						tetronimos[f][s + 1].y += (T_HEIGHT + 1);
						tetronimos[f + 1][s] = Vacio;

						auxiliarColor = red[f + 1][s];
						red[f + 1][s] = 0;

						red[f][s + 1] = auxiliarColor;

						auxiliarColor = red[f + 2][s];
						red[f + 2][s] = 0;

						red[f][s + 2] = auxiliarColor;

						auxiliarColor = red[f + 3][s];
						red[f + 3][s] = 0;

						red[f][s + 3] = auxiliarColor;


						auxiliarColor = green[f + 1][s];
						green[f + 1][s] = 0;

						green[f][s + 1] = auxiliarColor;

						auxiliarColor = green[f + 2][s];
						green[f + 2][s] = 0;

						green[f][s + 2] = auxiliarColor;

						auxiliarColor = green[f + 3][s];
						green[f + 3][s] = 0;

						green[f][s + 3] = auxiliarColor;


						auxiliarColor = blue[f + 1][s];
						blue[f + 1][s] = 0;

						blue[f][s + 1] = auxiliarColor;

						auxiliarColor = blue[f + 2][s];
						blue[f + 2][s] = 0;

						blue[f][s + 2] = auxiliarColor;

						auxiliarColor = blue[f + 3][s];
						blue[f + 3][s] = 0;

						blue[f][s + 3] = auxiliarColor;

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

		// orange
		if (num == 1) {
			int n = 0;
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0))
					{
						if (type[1][s] == 'P')
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f - 1][s + 1] = 'N';
							type[f - 1][s + 2] = 'N';

							type[f - 1][s] = 'P';
							type[f][s] = 'P';
							type[f][s + 1] = 'P';
							type[f + 1][s + 1] = 'P';

							tetronimos[f - 1][s] = tetronimos[f][s];
							tetronimos[f - 1][s].x -= (T_WIDTH + 1);
							tetronimos[f][s] = Vacio;

							tetronimos[f][s] = tetronimos[f][s + 1];
							tetronimos[f][s].y -= (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f][s + 1] = tetronimos[f - 1][s + 1];
							tetronimos[f][s + 1].x += (T_WIDTH + 1);
							tetronimos[f - 1][s + 1] = Vacio;

							tetronimos[f + 1][s + 1] = tetronimos[f - 1][s + 2];
							tetronimos[f + 1][s + 1].x += (T_WIDTH + 1) * 2;
							tetronimos[f + 1][s + 1].y -= (T_HEIGHT + 1);
							tetronimos[f - 1][s + 2] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f][s] = auxiliarColor;

							auxiliarColor = red[f - 1][s + 1];
							red[f - 1][s + 1] = 0;

							red[f][s + 1] = auxiliarColor;

							auxiliarColor = red[f - 1][s + 2];
							red[f - 1][s + 2] = 0;

							red[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f][s] = auxiliarColor;

							auxiliarColor = green[f - 1][s + 1];
							green[f - 1][s + 1] = 0;

							green[f][s + 1] = auxiliarColor;

							auxiliarColor = green[f - 1][s + 2];
							green[f - 1][s + 2] = 0;

							green[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f][s] = auxiliarColor;

							auxiliarColor = blue[f - 1][s + 1];
							blue[f - 1][s + 1] = 0;

							blue[f][s + 1] = auxiliarColor;

							auxiliarColor = blue[f - 1][s + 2];
							blue[f - 1][s + 2] = 0;

							blue[f + 1][s + 1] = auxiliarColor;
						}
						else
						{
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

							tetronimos[f - 2][s] = tetronimos[f][s];
							tetronimos[f - 2][s].x -= (T_WIDTH + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f - 1][s] = tetronimos[f][s + 1];
							tetronimos[f - 1][s].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s].y -= (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f][s + 1] = tetronimos[f - 1][s + 2];
							tetronimos[f][s + 1].x += (T_WIDTH + 1);
							tetronimos[f][s + 1].y -= (T_HEIGHT + 1);
							tetronimos[f - 1][s + 2] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f - 2][s] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f - 1][s + 2];
							red[f - 1][s + 2] = 0;

							red[f][s + 1] = auxiliarColor;

							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f - 2][s] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f - 1][s + 2];
							green[f - 1][s + 2] = 0;

							green[f][s + 1] = auxiliarColor;

							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f - 2][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f - 1][s + 2];
							blue[f - 1][s + 2] = 0;

							blue[f][s + 1] = auxiliarColor;
						}

						bloque++;
					}

					if ((type[f][s] == 'P') && (rotar == 1) && (bloque == 0)) {
						type[f][s] = 'N';
						type[f + 1][s] = 'N';
						type[f + 2][s + 1] = 'N';

						type[f + 2][s] = 'P';
						type[f + 2][s + 1] = 'P';
						type[f + 1][s + 2] = 'P';

						tetronimos[f + 2][s] = tetronimos[f][s];
						tetronimos[f + 2][s].x += (T_WIDTH + 1) * 2;
						tetronimos[f][s] = Vacio;

						tetronimos[f + 1][s + 2] = tetronimos[f + 2][s + 1];
						tetronimos[f + 1][s + 2].x -= (T_WIDTH + 1);
						tetronimos[f + 1][s + 2].y += (T_HEIGHT + 1);
						tetronimos[f + 2][s + 1] = Vacio;

						tetronimos[f + 2][s + 1] = tetronimos[f + 1][s];
						tetronimos[f + 2][s + 1].x += (T_WIDTH + 1);
						tetronimos[f + 2][s + 1].y += (T_HEIGHT + 1);
						tetronimos[f + 1][s] = Vacio;

						tetronimos[f + 1][s + 1] = tetronimos[f + 1][s + 1];

						auxiliarColor = red[f][s];
						red[f][s] = 0;

						red[f + 2][s] = auxiliarColor;

						auxiliarColor = red[f + 2][s + 1];
						red[f + 2][s + 1] = 0;

						red[f + 1][s + 2] = auxiliarColor;

						auxiliarColor = red[f + 1][s];
						red[f + 1][s] = 0;

						red[f + 2][s + 1] = auxiliarColor;


						auxiliarColor = green[f][s];
						green[f][s] = 0;

						green[f + 2][s] = auxiliarColor;

						auxiliarColor = green[f + 2][s + 1];
						green[f + 2][s + 1] = 0;

						green[f + 1][s + 2] = auxiliarColor;

						auxiliarColor = green[f + 1][s];
						green[f + 1][s] = 0;

						green[f + 2][s + 1] = auxiliarColor;


						auxiliarColor = blue[f][s];
						blue[f][s] = 0;

						blue[f + 2][s] = auxiliarColor;

						auxiliarColor = blue[f + 2][s + 1];
						blue[f + 2][s + 1] = 0;

						blue[f + 1][s + 2] = auxiliarColor;

						auxiliarColor = blue[f + 1][s];
						blue[f + 1][s] = 0;

						blue[f + 2][s + 1] = auxiliarColor;

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

		// pink
		if (num == 2) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 4) && (bloque == 0)) {
						if (type[0][s] != 'P')
						{
							type[f][s] = 'N';
							type[f + 1][s] = 'N';
							type[f + 1][s + 1] = 'N';
							type[f + 1][s + 2] = 'N';

							type[f][s] = 'P';
							type[f - 1][s] = 'P';
							type[f + 1][s] = 'P';
							type[f - 1][s + 1] = 'P';

							tetronimos[f - 1][s] = tetronimos[f + 1][s];
							tetronimos[f - 1][s].x -= (T_WIDTH + 1) * 2;
							tetronimos[f + 1][s] = Vacio;

							tetronimos[f + 1][s] = tetronimos[f + 1][s + 2];
							tetronimos[f + 1][s].y -= (T_HEIGHT + 1) * 2;
							tetronimos[f + 1][s + 2] = Vacio;

							tetronimos[f - 1][s + 1] = tetronimos[f][s];
							tetronimos[f - 1][s + 1].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s + 1].y += (T_HEIGHT + 1);
							tetronimos[f][s] = Vacio;

							tetronimos[f][s] = tetronimos[f + 1][s + 1];
							tetronimos[f][s].x -= (T_WIDTH + 1);
							tetronimos[f][s].y -= (T_HEIGHT + 1);
							tetronimos[f + 1][s + 1] = Vacio;

							auxiliarColor = red[f + 1][s];
							red[f + 1][s] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 1];
							red[f + 1][s + 1] = 0;

							red[f + 1][s] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 2];
							red[f + 1][s + 2] = 0;

							red[f - 1][s + 1] = auxiliarColor;


							auxiliarColor = green[f + 1][s];
							green[f + 1][s] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 1];
							green[f + 1][s + 1] = 0;

							green[f + 1][s] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 2];
							green[f + 1][s + 2] = 0;

							green[f - 1][s + 1] = auxiliarColor;


							auxiliarColor = blue[f + 1][s];
							blue[f + 1][s] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 1];
							blue[f + 1][s + 1] = 0;

							blue[f + 1][s] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 2];
							blue[f + 1][s + 2] = 0;

							blue[f - 1][s + 1] = auxiliarColor;
						}
						else
						{
							type[f][s] = 'N';
							type[f + 1][s] = 'N';
							type[f + 1][s + 1] = 'N';
							type[f + 1][s + 2] = 'N';

							type[f][s] = 'P';
							type[f][s + 1] = 'P';
							type[f + 1][s] = 'P';
							type[f + 2][s] = 'P';

							tetronimos[f][s + 1] = tetronimos[f][s];
							tetronimos[f][s + 1].y += (T_HEIGHT + 1);
							tetronimos[f][s] = Vacio;

							tetronimos[f][s] = tetronimos[f + 1][s];
							tetronimos[f][s].x -= (T_WIDTH + 1);
							tetronimos[f + 1][s] = Vacio;

							tetronimos[f + 2][s] = tetronimos[f + 1][s + 2];
							tetronimos[f + 2][s].y -= (T_HEIGHT + 1) * 2;
							tetronimos[f + 2][s].x += (T_WIDTH + 1);
							tetronimos[f + 1][s + 2] = Vacio;

							tetronimos[f + 1][s] = tetronimos[f + 1][s + 1];
							tetronimos[f + 1][s].y -= (T_HEIGHT + 1);
							tetronimos[f + 1][s + 1] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f][s + 1] = auxiliarColor;

							auxiliarColor = red[f + 1][s];
							red[f + 1][s] = 0;

							red[f][s] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 1];
							red[f + 1][s + 1] = 0;

							red[f][s - 1] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 2];
							red[f + 1][s + 2] = 0;

							red[f + 2][s] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f][s + 1] = auxiliarColor;

							auxiliarColor = green[f + 1][s];
							green[f + 1][s] = 0;

							green[f][s] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 1];
							green[f + 1][s + 1] = 0;

							green[f][s - 1] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 2];
							green[f + 1][s + 2] = 0;

							green[f + 2][s] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f][s + 1] = auxiliarColor;

							auxiliarColor = blue[f + 1][s];
							blue[f + 1][s] = 0;

							blue[f][s] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 1];
							blue[f + 1][s + 1] = 0;

							blue[f][s - 1] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 2];
							blue[f + 1][s + 2] = 0;

							blue[f + 2][s] = auxiliarColor;
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

						tetronimos[f][s - 1] = tetronimos[f][s + 1];
						tetronimos[f][s - 1].y -= (T_HEIGHT + 1) * 2;
						tetronimos[f][s + 1] = Vacio;

						tetronimos[f][s + 1] = tetronimos[f - 2][s + 1];
						tetronimos[f][s + 1].x += (T_WIDTH + 1) * 2;
						tetronimos[f - 2][s + 1] = Vacio;

						tetronimos[f - 1][s - 1] = tetronimos[f][s];
						tetronimos[f - 1][s - 1].x -= (T_WIDTH + 1);
						tetronimos[f - 1][s - 1].y -= (T_HEIGHT + 1);
						tetronimos[f][s] = Vacio;

						tetronimos[f][s] = tetronimos[f - 1][s + 1];
						tetronimos[f][s].x += (T_WIDTH + 1);
						tetronimos[f][s].y -= (T_HEIGHT + 1);
						tetronimos[f - 1][s + 1] = Vacio;

						auxiliarColor = red[f][s + 1];
						red[f][s + 1] = 0;

						red[f][s + 1] = auxiliarColor;

						auxiliarColor = red[f - 1][s + 1];
						red[f - 1][s + 1] = 0;

						red[f][s - 1] = auxiliarColor;

						auxiliarColor = red[f - 2][s + 1];
						red[f - 2][s + 1] = 0;

						red[f - 1][s - 1] = auxiliarColor;


						auxiliarColor = green[f][s + 1];
						green[f][s + 1] = 0;

						green[f][s + 1] = auxiliarColor;

						auxiliarColor = green[f - 1][s + 1];
						green[f - 1][s + 1] = 0;

						green[f][s - 1] = auxiliarColor;

						auxiliarColor = green[f - 2][s + 1];
						green[f - 2][s + 1] = 0;

						green[f - 1][s - 1] = auxiliarColor;


						auxiliarColor = blue[f][s + 1];
						blue[f][s + 1] = 0;

						blue[f][s + 1] = auxiliarColor;

						auxiliarColor = blue[f - 1][s + 1];
						blue[f - 1][s + 1] = 0;

						blue[f][s - 1] = auxiliarColor;

						auxiliarColor = blue[f - 2][s + 1];
						blue[f - 2][s + 1] = 0;

						blue[f - 1][s - 1] = auxiliarColor;

						/*rectIdle1.x += 10;
						rectIdle2.x += 20;
						rectIdle3.x -= 10;

						rectIdle.y -= 20;
						rectIdle1.y -= 10;
						rectIdle3.y -= 10;*/

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						if (type[8][s] != 'P') {
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f + 1][s + 2] = 'N';

							type[f][s + 2] = 'P';
							type[f + 1][s + 2] = 'P';
							type[f + 2][s + 2] = 'P';
							type[f + 2][s + 1] = 'P';

							tetronimos[f + 2][s + 2] = tetronimos[f][s + 2];
							tetronimos[f + 2][s + 2].x += (T_WIDTH + 1) * 2;
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f][s + 2] = tetronimos[f][s];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f + 2][s + 1] = tetronimos[f + 1][s + 2];
							tetronimos[f + 2][s + 1].x += (T_WIDTH + 1);
							tetronimos[f + 2][s + 1].y -= (T_HEIGHT + 1);
							tetronimos[f + 1][s + 2] = Vacio;

							tetronimos[f + 1][s + 2] = tetronimos[f][s + 1];
							tetronimos[f + 1][s + 2].x += (T_WIDTH + 1);
							tetronimos[f + 1][s + 2].y += (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 2][s + 2] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 2];
							red[f + 1][s + 2] = 0;

							red[f + 2][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 2][s + 2] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 2];
							green[f + 1][s + 2] = 0;

							green[f + 2][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 2][s + 2] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 2];
							blue[f + 1][s + 2] = 0;

							blue[f + 2][s + 1] = auxiliarColor;
						}
						else {
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f + 1][s + 2] = 'N';

							type[f - 1][s + 2] = 'P';
							type[f][s + 2] = 'P';
							type[f + 1][s + 2] = 'P';
							type[f + 1][s + 1] = 'P';

							tetronimos[f - 1][s + 2] = tetronimos[f][s];
							tetronimos[f - 1][s + 2].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f + 1][s + 1] = tetronimos[f + 1][s + 2];
							tetronimos[f + 1][s + 1].y -= (T_HEIGHT + 1);
							tetronimos[f + 1][s + 2] = Vacio;

							tetronimos[f + 1][s + 2] = tetronimos[f][s + 2];
							tetronimos[f + 1][s + 2].x += (T_WIDTH + 1);
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f][s + 2] = tetronimos[f][s + 1];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 2];
							red[f + 1][s + 2] = 0;

							red[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 2];
							green[f + 1][s + 2] = 0;

							green[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 2];
							blue[f + 1][s + 2] = 0;

							blue[f + 1][s + 1] = auxiliarColor;
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

						tetronimos[f][s + 2] = tetronimos[f][s];
						tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
						tetronimos[f][s] = Vacio;

						tetronimos[f][s] = tetronimos[f + 2][s];
						tetronimos[f][s].x -= (T_WIDTH + 1) * 2;
						tetronimos[f + 2][s] = Vacio;

						tetronimos[f + 1][s + 2] = tetronimos[f][s + 1];
						tetronimos[f + 1][s + 2].x += (T_WIDTH + 1);
						tetronimos[f + 1][s + 2].y += (T_HEIGHT + 1);
						tetronimos[f][s + 1] = Vacio;

						tetronimos[f][s + 1] = tetronimos[f + 1][s];
						tetronimos[f][s + 1].x -= (T_WIDTH + 1);
						tetronimos[f][s + 1].y += (T_HEIGHT + 1);
						tetronimos[f + 1][s] = Vacio;

						auxiliarColor = red[f + 1][s];
						red[f + 1][s] = 0;

						red[f][s + 1] = auxiliarColor;

						auxiliarColor = red[f + 2][s];
						red[f][s + 2] = 0;

						red[f + 1][s + 2] = auxiliarColor;

						auxiliarColor = red[f][s + 1];
						red[f][s + 1] = 0;

						red[f + 1][s + 2] = auxiliarColor;


						auxiliarColor = green[f + 1][s];
						green[f + 1][s] = 0;

						green[f][s + 1] = auxiliarColor;

						auxiliarColor = green[f + 2][s];
						green[f][s + 2] = 0;

						green[f + 1][s + 2] = auxiliarColor;

						auxiliarColor = green[f][s + 1];
						green[f][s + 1] = 0;

						green[f + 1][s + 2] = auxiliarColor;


						auxiliarColor = blue[f + 1][s];
						blue[f + 1][s] = 0;

						blue[f][s + 1] = auxiliarColor;

						auxiliarColor = blue[f + 2][s];
						blue[f][s + 2] = 0;

						blue[f + 1][s + 2] = auxiliarColor;

						auxiliarColor = blue[f][s + 1];
						blue[f][s + 1] = 0;

						blue[f + 1][s + 2] = auxiliarColor;

						//rectIdle1.x -= 10;
						//rectIdle2.x -= 20;
						//rectIdle3.x += 10;

						//rectIdle.y += 20;
						//rectIdle1.y += 10;
						//rectIdle3.y += 10;

						bloque++;
					}
				}
			}

			if (rotar == 4) {
				rotar = 0;
			}
		}

		// yellow
		if (num == 3) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 4) && (bloque == 0)) {
						if (type[1][s] != 'P')
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f - 1][s + 2] = 'N';

							type[f][s] = 'P';
							type[f - 1][s] = 'P';
							type[f - 2][s] = 'P';
							type[f][s + 1] = 'P';

							tetronimos[f - 2][s] = tetronimos[f][s];
							tetronimos[f - 2][s].x -= (T_WIDTH + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f - 1][s] = tetronimos[f][s + 1];
							tetronimos[f - 1][s].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s].y -= (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f][s] = tetronimos[f][s + 2];
							tetronimos[f][s].y -= (T_HEIGHT + 1) * 2;
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f][s + 1] = tetronimos[f - 1][s + 2];
							tetronimos[f][s + 1].x += (T_WIDTH + 1);
							tetronimos[f][s + 1].y -= (T_HEIGHT + 1);
							tetronimos[f - 1][s + 2] = Vacio;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f - 2][s] = auxiliarColor;

							auxiliarColor = red[f - 1][s + 2];
							red[f - 1][s + 2] = 0;

							red[f][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f - 2][s] = auxiliarColor;

							auxiliarColor = green[f - 1][s + 2];
							green[f - 1][s + 2] = 0;

							green[f][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f - 2][s] = auxiliarColor;

							auxiliarColor = blue[f - 1][s + 2];
							blue[f - 1][s + 2] = 0;

							blue[f][s + 1] = auxiliarColor;
						}
						else
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f - 1][s + 2] = 'N';

							type[f][s] = 'P';
							type[f - 1][s] = 'P';
							type[f + 1][s] = 'P';
							type[f + 1][s + 1] = 'P';

							tetronimos[f - 1][s] = tetronimos[f][s];
							tetronimos[f - 1][s].x -= (T_WIDTH + 1);
							tetronimos[f][s] = Vacio;

							tetronimos[f][s] = tetronimos[f][s + 1];
							tetronimos[f][s].y -= (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f + 1][s] = tetronimos[f][s + 2];
							tetronimos[f + 1][s].x += (T_WIDTH + 1);
							tetronimos[f + 1][s].y -= (T_HEIGHT + 1) * 2;
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f + 1][s + 1] = tetronimos[f - 1][s + 2];
							tetronimos[f + 1][s + 1].x += (T_WIDTH + 1) * 2;
							tetronimos[f + 1][s + 1].y -= (T_HEIGHT + 1);
							tetronimos[f - 1][s + 2] = Vacio;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 1][s] = auxiliarColor;

							auxiliarColor = red[f - 1][s + 2];
							red[f - 1][s + 2] = 0;

							red[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 1][s] = auxiliarColor;

							auxiliarColor = green[f - 1][s + 2];
							green[f - 1][s + 2] = 0;

							green[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 1][s] = auxiliarColor;

							auxiliarColor = blue[f - 1][s + 2];
							blue[f - 1][s + 2] = 0;

							blue[f + 1][s + 1] = auxiliarColor;
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

						tetronimos[f + 2][s - 1] = tetronimos[f + 2][s + 1];
						tetronimos[f + 2][s - 1].y -= (T_HEIGHT + 1) * 2;
						tetronimos[f + 2][s + 1] = Vacio;

						tetronimos[f + 2][s] = tetronimos[f + 1][s + 1];
						tetronimos[f + 2][s].x += (T_WIDTH + 1);
						tetronimos[f + 2][s].y -= (T_HEIGHT + 1);
						tetronimos[f + 1][s + 1] = Vacio;

						tetronimos[f + 2][s + 1] = tetronimos[f][s + 1];
						tetronimos[f + 2][s + 1].x += (T_WIDTH + 1) * 2;
						tetronimos[f][s + 1] = Vacio;

						tetronimos[f + 1][s + 1] = tetronimos[f][s];
						tetronimos[f + 1][s + 1].x += (T_WIDTH + 1);
						tetronimos[f + 1][s + 1].y += (T_HEIGHT + 1);
						tetronimos[f][s] = Vacio;

						auxiliarColor = red[f][s];
						red[f][s] = 0;

						red[f + 2][s + 1] = auxiliarColor;

						auxiliarColor = red[f][s + 1];
						red[f][s + 1] = 0;

						red[f + 2][s] = auxiliarColor;

						auxiliarColor = red[f + 1][s + 1];
						red[f + 1][s + 1] = 0;

						red[f + 2][s - 1] = auxiliarColor;

						auxiliarColor = red[f + 2][s + 1];
						red[f + 2][s + 1] = 0;

						red[f + 1][s + 1] = auxiliarColor;


						auxiliarColor = green[f][s];
						green[f][s] = 0;

						green[f + 2][s + 1] = auxiliarColor;

						auxiliarColor = green[f][s + 1];
						green[f][s + 1] = 0;

						green[f + 2][s] = auxiliarColor;

						auxiliarColor = green[f + 1][s + 1];
						green[f + 1][s + 1] = 0;

						green[f + 2][s - 1] = auxiliarColor;

						auxiliarColor = green[f + 2][s + 1];
						green[f + 2][s + 1] = 0;

						green[f + 1][s + 1] = auxiliarColor;


						auxiliarColor = blue[f][s];
						blue[f][s] = 0;

						blue[f + 2][s + 1] = auxiliarColor;

						auxiliarColor = blue[f][s + 1];
						blue[f][s + 1] = 0;

						blue[f + 2][s] = auxiliarColor;

						auxiliarColor = blue[f + 1][s + 1];
						blue[f + 1][s + 1] = 0;

						blue[f + 2][s - 1] = auxiliarColor;

						auxiliarColor = blue[f + 2][s + 1];
						blue[f + 2][s + 1] = 0;

						blue[f + 1][s + 1] = auxiliarColor;

						//rectIdle1.x += 10;
						//rectIdle2.x += 20;
						//rectIdle3.x += 10;

						//rectIdle.y -= 20;
						//rectIdle1.y -= 10;
						//rectIdle3.y += 10;

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						if (type[9][s] != 'P')
						{
							type[f][s] = 'N';
							type[f + 1][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';

							type[f][s + 2] = 'P';
							type[f][s + 1] = 'P';
							type[f + 1][s + 2] = 'P';
							type[f + 2][s + 2] = 'P';

							tetronimos[f + 2][s + 2] = tetronimos[f][s + 2];
							tetronimos[f + 2][s + 2].x += (T_WIDTH + 1) * 2;
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f + 1][s + 2] = tetronimos[f][s + 1];
							tetronimos[f + 1][s + 2].x += (T_WIDTH + 1);
							tetronimos[f + 1][s + 2].y += (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f][s + 2] = tetronimos[f][s];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f][s + 1] = tetronimos[f + 1][s];
							tetronimos[f][s + 1].x -= (T_WIDTH + 1);
							tetronimos[f][s + 1].y += (T_HEIGHT + 1);
							tetronimos[f + 1][s] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f + 1][s];
							red[f + 1][s] = 0;

							red[f][s + 1] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 2][s + 2] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f + 1][s];
							green[f + 1][s] = 0;

							green[f][s + 1] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 2][s + 2] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f + 1][s];
							blue[f + 1][s] = 0;

							blue[f][s + 1] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 2][s + 2] = auxiliarColor;
						}
						else {
							type[f][s] = 'N';
							type[f + 1][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';

							type[f + 1][s + 2] = 'P';
							type[f][s + 2] = 'P';
							type[f - 1][s + 2] = 'P';
							type[f - 1][s + 1] = 'P';

							tetronimos[f + 1][s + 2] = tetronimos[f][s + 2];
							tetronimos[f + 1][s + 2].x += (T_WIDTH + 1);
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f][s + 2] = tetronimos[f][s + 1];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f - 1][s + 2] = tetronimos[f][s];
							tetronimos[f - 1][s + 2].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f - 1][s + 1] = tetronimos[f + 1][s];
							tetronimos[f - 1][s + 1].x -= (T_WIDTH + 1) * 2;
							tetronimos[f - 1][s + 1].y += (T_HEIGHT + 1);
							tetronimos[f + 1][s] = Vacio;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f + 1][s];
							red[f + 1][s] = 0;

							red[f - 1][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f + 1][s];
							green[f + 1][s] = 0;

							green[f - 1][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f + 1][s];
							blue[f + 1][s] = 0;

							blue[f - 1][s + 1] = auxiliarColor;
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

						tetronimos[f][s + 2] = tetronimos[f][s];
						tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
						tetronimos[f][s] = Vacio;

						tetronimos[f][s + 1] = tetronimos[f + 1][s];
						tetronimos[f][s + 1].x -= (T_WIDTH + 1);
						tetronimos[f][s + 1].y += (T_HEIGHT + 1);
						tetronimos[f + 1][s] = Vacio;

						tetronimos[f][s] = tetronimos[f + 2][s];
						tetronimos[f][s].x -= (T_WIDTH + 1) * 2;
						tetronimos[f + 2][s] = Vacio;

						tetronimos[f + 1][s] = tetronimos[f + 2][s + 1];
						tetronimos[f + 1][s].x -= (T_WIDTH + 1);
						tetronimos[f + 1][s].y -= (T_HEIGHT + 1);
						tetronimos[f + 2][s + 1] = Vacio;

						auxiliarColor = red[f + 2][s];
						red[f + 2][s] = 0;

						red[f][s + 1] = auxiliarColor;

						auxiliarColor = red[f + 2][s + 1];
						red[f + 2][s + 1] = 0;

						red[f][s + 2] = auxiliarColor;


						auxiliarColor = green[f + 2][s];
						green[f + 2][s] = 0;

						green[f][s + 1] = auxiliarColor;

						auxiliarColor = green[f + 2][s + 1];
						green[f + 2][s + 1] = 0;

						green[f][s + 2] = auxiliarColor;


						auxiliarColor = blue[f + 2][s];
						blue[f + 2][s] = 0;

						blue[f][s + 1] = auxiliarColor;

						auxiliarColor = blue[f + 2][s + 1];
						blue[f + 2][s + 1] = 0;

						blue[f][s + 2] = auxiliarColor;

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

		// green
		if (num == 4) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 4) && (bloque == 0)) {
						if (type[1][s] != 'P')
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f - 1][s + 1] = 'N';

							type[f][s] = 'P';
							type[f - 1][s] = 'P';
							type[f - 2][s] = 'P';
							type[f - 1][s + 1] = 'P';

							tetronimos[f - 2][s] = tetronimos[f][s];
							tetronimos[f - 2][s].x -= (T_WIDTH + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f][s] = tetronimos[f][s + 2];
							tetronimos[f][s].y -= (T_HEIGHT + 1) * 2;
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f - 1][s] = tetronimos[f][s + 1];
							tetronimos[f - 1][s].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s].y -= (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f - 2][s] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f][s] = auxiliarColor;


							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f - 2][s] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f][s] = auxiliarColor;


							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f - 2][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f][s] = auxiliarColor;
						}
						else
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f - 1][s + 1] = 'N';

							type[f][s] = 'P';
							type[f - 1][s] = 'P';
							type[f + 1][s] = 'P';
							type[f][s + 1] = 'P';

							tetronimos[f - 1][s] = tetronimos[f][s];
							tetronimos[f - 1][s].x -= (T_WIDTH + 1);
							tetronimos[f][s] = Vacio;

							tetronimos[f + 1][s] = tetronimos[f][s + 2];
							tetronimos[f + 1][s].x += (T_WIDTH + 1);
							tetronimos[f + 1][s].y -= (T_HEIGHT + 1) * 2;
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f][s] = tetronimos[f][s + 1];
							tetronimos[f][s].y -= (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f][s + 1] = tetronimos[f - 1][s + 1];
							tetronimos[f][s + 1].x += (T_WIDTH + 1);
							tetronimos[f - 1][s + 1] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f - 1][s] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f][s] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 1][s] = auxiliarColor;

							auxiliarColor = red[f - 1][s + 1];
							red[f - 1][s + 1] = 0;

							red[f][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f - 1][s] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f][s] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 1][s] = auxiliarColor;

							auxiliarColor = green[f - 1][s + 1];
							green[f - 1][s + 1] = 0;

							green[f][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f - 1][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f][s] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 1][s] = auxiliarColor;

							auxiliarColor = blue[f - 1][s + 1];
							blue[f - 1][s + 1] = 0;

							blue[f][s + 1] = auxiliarColor;
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

						type[f][s] = 'P';
						type[f + 1][s] = 'P';
						type[f + 1][s + 1] = 'P';
						type[f + 1][s - 1] = 'P';

						tetronimos[f + 1][s] = tetronimos[f][s + 1];
						tetronimos[f + 1][s].x += (T_WIDTH + 1);
						tetronimos[f + 1][s].y -= (T_HEIGHT + 1);
						tetronimos[f][s + 1] = Vacio;

						tetronimos[f + 1][s - 1] = tetronimos[f + 1][s + 1];
						tetronimos[f + 1][s - 1].y -= (T_HEIGHT + 1) * 2;
						tetronimos[f + 1][s + 1] = Vacio;

						tetronimos[f + 1][s + 1] = tetronimos[f - 1][s + 1];
						tetronimos[f + 1][s + 1].x += (T_WIDTH + 1) * 2;
						tetronimos[f - 1][s + 1] = Vacio;

						auxiliarColor = red[f + 1][s + 1];
						red[f + 1][s + 1] = 0;

						red[f + 1][s - 1] = auxiliarColor;

						auxiliarColor = red[f - 1][s + 1];
						red[f - 1][s + 1] = 0;

						red[f + 1][s + 1] = auxiliarColor;

						auxiliarColor = red[f][s + 1];
						red[f][s + 1] = 0;

						red[f + 1][s] = auxiliarColor;


						auxiliarColor = green[f + 1][s + 1];
						green[f + 1][s + 1] = 0;

						green[f + 1][s - 1] = auxiliarColor;

						auxiliarColor = green[f - 1][s + 1];
						green[f - 1][s + 1] = 0;

						green[f + 1][s + 1] = auxiliarColor;

						auxiliarColor = green[f][s + 1];
						green[f][s + 1] = 0;

						green[f + 1][s] = auxiliarColor;


						auxiliarColor = blue[f + 1][s + 1];
						blue[f + 1][s + 1] = 0;

						blue[f + 1][s - 1] = auxiliarColor;

						auxiliarColor = blue[f - 1][s + 1];
						blue[f - 1][s + 1] = 0;

						blue[f + 1][s + 1] = auxiliarColor;

						auxiliarColor = blue[f][s + 1];
						blue[f][s + 1] = 0;

						blue[f + 1][s] = auxiliarColor;

						/*rectIdle1.x += 10;
						rectIdle2.x += 20;

						rectIdle.y -= 20;
						rectIdle1.y -= 10;*/

						bloque++;
					}
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						if (type[8][s] != 'P')
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f + 1][s + 1] = 'N';

							type[f][s + 2] = 'P';
							type[f + 1][s + 2] = 'P';
							type[f + 2][s + 2] = 'P';
							type[f + 1][s + 1] = 'P';

							tetronimos[f + 2][s + 2] = tetronimos[f][s + 2];
							tetronimos[f + 2][s + 2].x += (T_WIDTH + 1) * 2;
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f + 1][s + 2] = tetronimos[f][s + 1];
							tetronimos[f + 1][s + 2].x += (T_WIDTH + 1);
							tetronimos[f + 1][s + 2].y += (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f][s + 2] = tetronimos[f][s];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 2][s + 2] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 2][s + 2] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 2][s + 2] = auxiliarColor;
						}
						else
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f][s + 2] = 'N';
							type[f + 1][s + 1] = 'N';

							type[f][s + 2] = 'P';
							type[f + 1][s + 2] = 'P';
							type[f - 1][s + 2] = 'P';
							type[f][s + 1] = 'P';

							tetronimos[f + 1][s + 2] = tetronimos[f][s + 2];
							tetronimos[f + 1][s + 2].x += (T_WIDTH + 1);
							tetronimos[f][s + 2] = Vacio;

							tetronimos[f][s + 2] = tetronimos[f][s + 1];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f - 1][s + 2] = tetronimos[f][s];
							tetronimos[f - 1][s + 2].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f][s + 1] = tetronimos[f + 1][s + 1];
							tetronimos[f][s + 1].x -= (T_WIDTH + 1);
							tetronimos[f + 1][s + 1] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 2];
							red[f][s + 2] = 0;

							red[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 1];
							red[f + 1][s + 1] = 0;

							red[f][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 2];
							green[f][s + 2] = 0;

							green[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 1];
							green[f + 1][s + 1] = 0;

							green[f][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 2];
							blue[f][s + 2] = 0;

							blue[f + 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 1];
							blue[f + 1][s + 1] = 0;

							blue[f][s + 1] = auxiliarColor;
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

						tetronimos[f][s + 2] = tetronimos[f][s];
						tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
						tetronimos[f][s] = Vacio;

						tetronimos[f][s + 1] = tetronimos[f + 1][s];
						tetronimos[f][s + 1].x -= (T_WIDTH + 1);
						tetronimos[f][s + 1].y += (T_HEIGHT + 1);
						tetronimos[f + 1][s] = Vacio;

						tetronimos[f][s] = tetronimos[f + 2][s];
						tetronimos[f][s].x -= (T_WIDTH + 1) * 2;
						tetronimos[f + 2][s] = Vacio;

						auxiliarColor = red[f + 1][s];
						red[f + 1][s] = 0;

						red[f][s + 1] = auxiliarColor;

						auxiliarColor = red[f + 2][s];
						red[f + 2][s] = 0;

						red[f][s + 2] = auxiliarColor;

						auxiliarColor = red[f + 1][s + 1];
						red[f + 1][s + 1] = 0;

						red[f + 1][s + 1] = auxiliarColor;


						auxiliarColor = green[f + 1][s];
						green[f + 1][s] = 0;

						green[f][s + 1] = auxiliarColor;

						auxiliarColor = green[f + 2][s];
						green[f + 2][s] = 0;

						green[f][s + 2] = auxiliarColor;

						auxiliarColor = green[f + 1][s + 1];
						green[f + 1][s + 1] = 0;

						green[f + 1][s + 1] = auxiliarColor;


						auxiliarColor = blue[f + 1][s];
						blue[f + 1][s] = 0;

						blue[f][s + 1] = auxiliarColor;

						auxiliarColor = blue[f + 2][s];
						blue[f + 2][s] = 0;

						blue[f][s + 2] = auxiliarColor;

						auxiliarColor = blue[f + 1][s + 1];
						blue[f + 1][s + 1] = 0;

						blue[f + 1][s + 1] = auxiliarColor;

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

		// blue
		//if(num == 5) -> No hace nada 

		// cyan
		if (num == 6) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P') && (rotar == 2) && (bloque == 0)) {
						if (type[0][s] != 'P')
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f + 1][s + 1] = 'N';
							type[f + 1][s + 2] = 'N';

							type[f - 1][s + 2] = 'P';
							type[f][s + 2] = 'P';
							type[f][s + 1] = 'P';
							type[f + 1][s + 1] = 'P';

							tetronimos[f - 1][s + 2] = tetronimos[f][s];
							tetronimos[f - 1][s + 2].x -= (T_WIDTH + 1);
							tetronimos[f - 1][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f][s + 2] = tetronimos[f][s + 1];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1);
							tetronimos[f][s + 1] = Vacio;

							tetronimos[f][s + 1] = tetronimos[f + 1][s + 1];
							tetronimos[f][s + 1].x -= (T_WIDTH + 1);
							tetronimos[f + 1][s + 1] = Vacio;

							tetronimos[f + 1][s + 1] = tetronimos[f + 1][s + 2];
							tetronimos[f + 1][s + 1].y -= (T_HEIGHT + 1);
							tetronimos[f + 1][s + 2] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 1];
							red[f + 1][s + 1] = 0;

							red[f][s + 1] = auxiliarColor;

							auxiliarColor = red[f + 1][s + 2];
							red[f + 1][s + 2] = 0;

							red[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 1];
							green[f + 1][s + 1] = 0;

							green[f][s + 1] = auxiliarColor;

							auxiliarColor = green[f + 1][s + 2];
							green[f + 1][s + 2] = 0;

							green[f + 1][s + 1] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f - 1][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 1];
							blue[f + 1][s + 1] = 0;

							blue[f][s + 1] = auxiliarColor;

							auxiliarColor = blue[f + 1][s + 2];
							blue[f + 1][s + 2] = 0;

							blue[f + 1][s + 1] = auxiliarColor;
						}
						else
						{
							type[f][s] = 'N';
							type[f][s + 1] = 'N';
							type[f + 1][s + 1] = 'N';
							type[f + 1][s + 2] = 'N';

							type[f][s + 2] = 'P';
							type[f + 1][s + 2] = 'P';
							type[f + 1][s + 1] = 'P';
							type[f + 2][s + 1] = 'P';

							tetronimos[f][s + 2] = tetronimos[f][s];
							tetronimos[f][s + 2].y += (T_HEIGHT + 1) * 2;
							tetronimos[f][s] = Vacio;

							tetronimos[f + 2][s + 1] = tetronimos[f][s + 1];
							tetronimos[f + 2][s + 1].x += (T_WIDTH + 1) * 2;
							tetronimos[f][s + 1] = Vacio;

							auxiliarColor = red[f][s];
							red[f][s] = 0;

							red[f][s + 2] = auxiliarColor;

							auxiliarColor = red[f][s + 1];
							red[f][s + 1] = 0;

							red[f + 2][s] = auxiliarColor;


							auxiliarColor = green[f][s];
							green[f][s] = 0;

							green[f][s + 2] = auxiliarColor;

							auxiliarColor = green[f][s + 1];
							green[f][s + 1] = 0;

							green[f + 2][s] = auxiliarColor;


							auxiliarColor = blue[f][s];
							blue[f][s] = 0;

							blue[f][s + 2] = auxiliarColor;

							auxiliarColor = blue[f][s + 1];
							blue[f][s + 1] = 0;

							blue[f + 2][s] = auxiliarColor;
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

						type[f][s - 1] = 'P';
						type[f][s] = 'P';
						type[f + 1][s] = 'P';
						type[f + 1][s + 1] = 'P';

						tetronimos[f][s - 1] = tetronimos[f + 1][s];
						tetronimos[f][s - 1].x -= (T_WIDTH + 1);
						tetronimos[f][s - 1].y -= (T_HEIGHT + 1);
						tetronimos[f + 1][s] = Vacio;

						tetronimos[f + 1][s] = tetronimos[f][s + 1];
						tetronimos[f + 1][s].x += (T_WIDTH + 1);
						tetronimos[f + 1][s].y -= (T_HEIGHT + 1);
						tetronimos[f][s + 1] = Vacio;

						tetronimos[f + 1][s + 1] = tetronimos[f - 1][s + 1];
						tetronimos[f + 1][s + 1].x += (T_WIDTH + 1) * 2;
						tetronimos[f - 1][s + 1] = Vacio;

						auxiliarColor = red[f][s];
						red[f][s] = 0;

						red[f][s - 1] = auxiliarColor;

						auxiliarColor = red[f + 1][s];
						red[f + 1][s] = 0;

						red[f][s] = auxiliarColor;

						auxiliarColor = red[f][s + 1];
						red[f][s + 1] = 0;

						red[f + 1][s] = auxiliarColor;

						auxiliarColor = red[f - 1][s + 1];
						red[f - 1][s + 1] = 0;

						red[f + 1][s + 1] = auxiliarColor;


						auxiliarColor = green[f][s];
						green[f][s] = 0;

						green[f][s - 1] = auxiliarColor;

						auxiliarColor = green[f + 1][s];
						green[f + 1][s] = 0;

						green[f][s] = auxiliarColor;

						auxiliarColor = green[f][s + 1];
						green[f][s + 1] = 0;

						green[f + 1][s] = auxiliarColor;

						auxiliarColor = green[f - 1][s + 1];
						green[f - 1][s + 1] = 0;

						green[f + 1][s + 1] = auxiliarColor;


						auxiliarColor = blue[f][s];
						blue[f][s] = 0;

						blue[f][s - 1] = auxiliarColor;

						auxiliarColor = blue[f + 1][s];
						blue[f + 1][s] = 0;

						blue[f][s] = auxiliarColor;

						auxiliarColor = blue[f][s + 1];
						blue[f][s + 1] = 0;

						blue[f + 1][s] = auxiliarColor;

						auxiliarColor = blue[f - 1][s + 1];
						blue[f - 1][s + 1] = 0;

						blue[f + 1][s + 1] = auxiliarColor;

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


					tetronimos[f - 1][s] = tetronimos[f][s];
					tetronimos[f][s] = Vacio;
					tetronimos[f - 1][s].x -= (T_WIDTH + 1);

					red[f][s] = 0;
					red[f - 1][s] = r;

					green[f][s] = 0;
					green[f - 1][s] = g;

					blue[f][s] = 0;
					blue[f - 1][s] = b;
				}
			}
		}

		/*if (move != 1) {
			int stop = 0;

			for (int i = 0; i < MAX_TETRONIMOS; i++)
			{
				if ((tetronimos[i + 1] == nullptr) && (stop == 0))
				{
					tetronimos[i]->x -= (T_WIDTH + 1);
					tetronimos[i - 1]->x -= (T_WIDTH + 1);
					tetronimos[i - 2]->x -= (T_WIDTH + 1);
					tetronimos[i - 3]->x -= (T_WIDTH + 1);
					stop = 1;
				}
			}
		}*/

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

					tetronimos[f + 1][s] = tetronimos[f][s];
					tetronimos[f][s] = Vacio;
					tetronimos[f + 1][s].x += (T_WIDTH + 1);

					red[f][s] = 0;
					red[f + 1][s] = r;

					green[f][s] = 0;
					green[f + 1][s] = g;

					blue[f][s] = 0;
					blue[f + 1][s] = b;
				}
			}
		}

		//if (move != 1) {
		//	int stop = 0;

		//	for (int i = 0; i < MAX_TETRONIMOS; i++)
		//	{
		//		if ((tetronimos[i + 1] == nullptr) && (stop == 0))
		//		{
		//			tetronimos[i]->x += (T_WIDTH + 1);
		//			tetronimos[i - 1]->x += (T_WIDTH + 1);
		//			tetronimos[i - 2]->x += (T_WIDTH + 1);
		//			tetronimos[i - 3]->x += (T_WIDTH + 1);
		//			stop = 1;
		//		}
		//	}
		//}

		move = 0;
	}

	// Fall fast (to do)
	if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT)
	{
		if (deltaTime > 500) {
			for (int s = 0; s <= 21; s++) {
				for (int f = 0; f <= 9; f++) {
					if ((type[f][s] == 'P')) {
						type[f][s + 1] = 'P';
						type[f][s] = 'N';

						arr[f][s + 1] = 1;
						arr[f][s] = 0;
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
		rotar = 0;
		SpawnTetronimo();
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

		tetronimos[f][s] = Vacio;
		tetronimos[f - 1][s] = Vacio;
		tetronimos[f - 2][s] = Vacio;
		tetronimos[f - 3][s] = Vacio;
		tetronimos[f - 4][s] = Vacio;
		tetronimos[f - 5][s] = Vacio;
		tetronimos[f - 6][s] = Vacio;
		tetronimos[f - 7][s] = Vacio;
		tetronimos[f - 8][s] = Vacio;
		tetronimos[f - 9][s] = Vacio;

		red[f][s] = 0;
		red[f - 1][s] = 0;
		red[f - 2][s] = 0;
		red[f - 3][s] = 0;
		red[f - 4][s] = 0;
		red[f - 5][s] = 0;
		red[f - 6][s] = 0;
		red[f - 7][s] = 0;
		red[f - 8][s] = 0;
		red[f - 9][s] = 0;

		green[f][s] = 0;
		green[f - 1][s] = 0;
		green[f - 2][s] = 0;
		green[f - 3][s] = 0;
		green[f - 4][s] = 0;
		green[f - 5][s] = 0;
		green[f - 6][s] = 0;
		green[f - 7][s] = 0;
		green[f - 8][s] = 0;
		green[f - 9][s] = 0;

		blue[f][s] = 0;
		blue[f - 1][s] = 0;
		blue[f - 2][s] = 0;
		blue[f - 3][s] = 0;
		blue[f - 4][s] = 0;
		blue[f - 5][s] = 0;
		blue[f - 6][s] = 0;
		blue[f - 7][s] = 0;
		blue[f - 8][s] = 0;
		blue[f - 9][s] = 0;

		for (int m = s - 1; m >= 0; m--) {//SOLO LO BAJA UNO!
			for (int j = 0; j <= 9; j++) {
				if (type[j][m] == 'B') {
					type[j][m] = 'N';
					type[j][m + 1] = 'B';

					tetronimos[j][m + 1] = tetronimos[j][m];
					tetronimos[j][m] = Vacio;
					tetronimos[j][m + 1].y += (T_HEIGHT + 1);

					red[j][m + 1] = red[j][m];
					red[j][m] = 0;

					green[j][m + 1] = green[j][m];
					green[j][m] = 0;

					blue[j][m + 1] = blue[j][m];
					blue[j][m] = 0;
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
			rotar = 0;
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
					rotar = 0;
					return (21 - s);
				}
			}
		}
	}

	return -1;
}

update_status ModuleTetronimo::PostUpdate() {

	for (int s = 0; s <= 21; s++)
	{
		for (int i = 0; i <= 9; i++)
		{
			if ((type[i][s] == 'P') || (type[i][s] == 'B')) {

				App->render->DrawQuad(tetronimos[i][s], red[i][s], green[i][s], blue[i][s], 255);

			}
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

	if (num == 0)		// red
	{
		arr[3][0] = 1;
		arr[4][0] = 1;
		arr[5][0] = 1;
		arr[6][0] = 1;

		type[3][0] = 'P';
		type[4][0] = 'P';
		type[5][0] = 'P';
		type[6][0] = 'P';


		Block[0] = { 113, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };	//150	//30
		Block[1] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };	//160
		Block[2] = { 113 + (T_WIDTH + 1) * 2, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };	//170
		Block[3] = { 113 + (T_WIDTH + 1) * 3, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };	//180


		tetronimos[3][0] = Block[0];
		tetronimos[4][0] = Block[1];
		tetronimos[5][0] = Block[2];
		tetronimos[6][0] = Block[3];

		r = 255;
		g = 0;
		b = 0;

		red[3][0] = r;
		red[4][0] = r;
		red[5][0] = r;
		red[6][0] = r;

		green[3][0] = g;
		green[4][0] = g;
		green[5][0] = g;
		green[6][0] = g;

		blue[3][0] = b;
		blue[4][0] = b;
		blue[5][0] = b;
		blue[6][0] = b;
	}

	if (num == 1)		// orange
	{
		arr[3][0] = 1;
		arr[4][0] = 1;
		arr[4][1] = 1;
		arr[5][1] = 1;

		type[3][0] = 'P';
		type[4][0] = 'P';
		type[4][1] = 'P';
		type[5][1] = 'P';

		Block[0] = { 113, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[1] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[2] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };	// y 40
		Block[3] = { 113 + (T_WIDTH + 1) * 2, 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };

		tetronimos[3][0] = Block[0];
		tetronimos[4][0] = Block[1];
		tetronimos[4][1] = Block[2];
		tetronimos[5][1] = Block[3];

		r = 255;
		g = 152;
		b = 2;

		red[3][0] = r;
		red[4][0] = r;
		red[4][1] = r;
		red[5][1] = r;

		green[3][0] = g;
		green[4][0] = g;
		green[4][1] = g;
		green[5][1] = g;

		blue[3][0] = b;
		blue[4][0] = b;
		blue[4][1] = b;
		blue[5][1] = b;



	}

	if (num == 2)		// pink
	{
		arr[3][0] = 1;
		arr[4][0] = 1;
		arr[5][0] = 1;
		arr[3][1] = 1;

		type[3][0] = 'P';
		type[4][0] = 'P';
		type[5][0] = 'P';
		type[3][1] = 'P';

		Block[0] = { 113, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[1] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[2] = { 113 + (T_WIDTH + 1) * 2, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[3] = { 113, 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };

		tetronimos[3][0] = Block[0];
		tetronimos[4][0] = Block[1];
		tetronimos[5][0] = Block[2];
		tetronimos[3][1] = Block[3];

		r = 255;
		g = 0;
		b = 255;

		red[3][0] = r;
		red[4][0] = r;
		red[5][0] = r;
		red[3][1] = r;

		green[3][0] = g;
		green[4][0] = g;
		green[5][0] = g;
		green[3][1] = g;

		blue[3][0] = b;
		blue[4][0] = b;
		blue[5][0] = b;
		blue[3][1] = b;
	}

	if (num == 3)		// yellow
	{
		arr[3][0] = 1;
		arr[4][0] = 1;
		arr[5][0] = 1;
		arr[5][1] = 1;

		type[3][0] = 'P';
		type[4][0] = 'P';
		type[5][0] = 'P';
		type[5][1] = 'P';

		Block[0] = { 113, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[1] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[2] = { 113 + (T_WIDTH + 1) * 2, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[3] = { 113 + (T_WIDTH + 1) * 2, 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };

		tetronimos[3][0] = Block[0];
		tetronimos[4][0] = Block[1];
		tetronimos[5][0] = Block[2];
		tetronimos[5][1] = Block[3];

		r = 249;
		g = 177;
		b = 7;

		red[3][0] = r;
		red[4][0] = r;
		red[5][0] = r;
		red[5][1] = r;

		green[3][0] = g;
		green[4][0] = g;
		green[5][0] = g;
		green[5][1] = g;

		blue[3][0] = b;
		blue[4][0] = b;
		blue[5][0] = b;
		blue[5][1] = b;
	}

	if (num == 4)		// green
	{
		arr[3][0] = 1;
		arr[4][0] = 1;
		arr[5][0] = 1;
		arr[4][1] = 1;

		type[3][0] = 'P';
		type[4][0] = 'P';
		type[5][0] = 'P';
		type[4][1] = 'P';

		Block[0] = { 113, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[1] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[2] = { 113 + (T_WIDTH + 1) * 2, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[3] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };


		tetronimos[3][0] = Block[0];
		tetronimos[4][0] = Block[1];
		tetronimos[5][0] = Block[2];
		tetronimos[4][1] = Block[3];

		r = 0;
		g = 161;
		b = 0;

		red[3][0] = r;
		red[4][0] = r;
		red[5][0] = r;
		red[4][1] = r;

		green[3][0] = g;
		green[4][0] = g;
		green[5][0] = g;
		green[4][1] = g;

		blue[3][0] = b;
		blue[4][0] = b;
		blue[5][0] = b;
		blue[4][1] = b;
	}

	if (num == 5)		// blue
	{
		arr[3][0] = 1;
		arr[4][0] = 1;
		arr[3][1] = 1;
		arr[4][1] = 1;

		type[3][0] = 'P';
		type[4][0] = 'P';
		type[3][1] = 'P';
		type[4][1] = 'P';

		Block[0] = { 113, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[1] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[2] = { 113, 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };
		Block[3] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };

		tetronimos[3][0] = Block[0];
		tetronimos[4][0] = Block[1];
		tetronimos[3][1] = Block[2];
		tetronimos[4][1] = Block[3];

		r = 16;
		g = 50;
		b = 229;

		red[3][0] = r;
		red[4][0] = r;
		red[3][1] = r;
		red[4][1] = r;

		green[3][0] = g;
		green[4][0] = g;
		green[3][1] = g;
		green[4][1] = g;

		blue[3][0] = b;
		blue[4][0] = b;
		blue[3][1] = b;
		blue[4][1] = b;
	}

	if (num == 6)		// cyan
	{
		arr[3][1] = 1;
		arr[4][1] = 1;
		arr[4][0] = 1;
		arr[5][0] = 1;

		type[3][1] = 'P';
		type[4][1] = 'P';
		type[4][0] = 'P';
		type[5][0] = 'P';

		Block[0] = { 113, 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };
		Block[1] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1) * 2, T_WIDTH, T_HEIGHT };
		Block[2] = { 113 + (T_WIDTH + 1), 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };
		Block[3] = { 113 + (T_WIDTH + 1) * 2, 35 + (T_HEIGHT + 1), T_WIDTH, T_HEIGHT };

		tetronimos[3][1] = Block[0];
		tetronimos[4][1] = Block[1];
		tetronimos[4][0] = Block[2];
		tetronimos[5][0] = Block[3];

		r = 0;
		g = 163;
		b = 173;

		red[3][1] = r;
		red[4][1] = r;
		red[4][0] = r;
		red[5][0] = r;

		green[3][1] = g;
		green[4][1] = g;
		green[4][0] = g;
		green[5][0] = g;

		blue[3][1] = b;
		blue[4][1] = b;
		blue[4][0] = b;
		blue[5][0] = b;


	}

	//AddTetronimo(rectIdle, r, g, b);
	//AddTetronimo(rectIdle1, r, g, b);
	//AddTetronimo(rectIdle2, r, g, b);
	//AddTetronimo(rectIdle3, r, g, b);

	/*SDL_Delay(600);*/

}

void ModuleTetronimo::SpawnTetronimo() {

	bool const r = (srand(time(NULL)), true);
	Spawn();
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
	int next = rand() % 7;

	for (int m = 0; m <= 21; m++) {
		for (int l = 0; l <= 9; l++) {
			if ((type[l][m] == 'P')) {
				type[l][m] = 'B';
			}
		}
	}

	LOG("NUM: %d", num);
	LOG("NEXT: %d", next);
	num = next;
}

//void ModuleTetronimo::AddTetronimo(const SDL_Rect& tetronimo, const int& r, const int& g, const int& b)
//{
//	for (int i = 0; i < MAX_TETRONIMOS; ++i)
//	{
//		if (tetronimos[i] == nullptr)
//		{
//			int* red1 = new int(r);
//			int* green1 = new int(g);
//			int* blue1 = new int(b);
//
//			red[i] = red1;
//			green[i] = green1;
//			blue[i] = blue1;
//
//			SDL_Rect* t = new SDL_Rect(tetronimo);
//
//			tetronimos[i] = t;
//			break;
//		}
//	}
//}

//void ModuleTetronimo::lvl_lose(const char* ch_loseContinue)
//{
//	// Game Over
//	App->tetronimo->Disable();
//
//	if (v_WinLose >= 0 && v_WinLose < 200)
//	{
//		if (v_WinLose == 5) App->audio->PlayFx(App->sLvl_1->fxGameOver, 0);
//		else { App->audio->PauseMusic(); }
//
//		App->render->DrawQuad({ 63, 0, 131, 66 }, 255, 0, 0, 255);
//		App->render->DrawQuad({ 70, 5, 118, 56 }, 0, 0, 255, 255);
//		App->render->DrawQuad({ 80, 15, 98, 34 }, 37, 37, 85, 255);
//		App->render->TextDraw("Game", 95, 16, 255, 255, 255, 255, 16);
//		App->render->TextDraw("Over", 95, 32, 255, 255, 255, 255, 16);
//	}
//	else if (v_WinLose > 200)
//	{
//		if (App->input->keys[SDL_SCANCODE_Z] == KEY_STATE::KEY_DOWN)
//		{
//			App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);
//		}
//
//		App->render->TextDraw("Press", 96, 146, 255, 255, 255, 255, 16);
//		App->render->TextDraw("Start", 96, 177, 255, 255, 255, 255, 16);
//		App->render->TextDraw("To", 128, 210, 255, 255, 255, 255, 16);
//		App->render->TextDraw("Continue", 79, 242, 255, 255, 255, 255, 16);
//
//		App->render->TextDraw(ch_loseContinue, 141, 369, 255, 255, 255, 255, 16);
//
//		if (v_WinLose % 50 == 0)
//		{
//			v_loseContinue--;
//		}
//		LOG("%d win lose", v_WinLose);
//	}
//
//	if (v_loseContinue == 0)
//	{
//		lvl_instaLose = false;
//		App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
//	}
//
//	v_WinLose++;
//}

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