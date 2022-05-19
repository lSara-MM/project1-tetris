#include "ScreenCredits2.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
//#include "ModuleAudio.h"

#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

ScreenCredits2::ScreenCredits2(bool startEnabled) : Module(startEnabled)
{


}

ScreenCredits2::~ScreenCredits2()
{

}

// Load assets
bool ScreenCredits2::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bg_texture = App->textures->Load("Assets/upccitm.png");


	return ret;
}

update_status ScreenCredits2::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sStart, 0);
	}

	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenCredits2::PostUpdate()
{
	App->render->Blit(bg_texture, 75, 40, NULL);

	App->render->TextDraw("This is a project made by students of the bachelors ", 25, 200, 255, 255, 255, 255, 12);
	App->render->TextDraw("degree in Video Game Design and Development of the", 25, 230, 255, 255, 255, 255, 12);
	App->render->TextDraw("Centre de la Imatge i de la Tecnologia Multimedia", 25, 260, 255, 255, 255, 255, 12);

	App->render->TextDraw("Project I", 25, 335, 255, 255, 255, 255, 12);
	App->render->TextDraw("Jesus Alonso Alonso", 25, 365, 255, 255, 255, 255, 12);
	App->render->TextDraw("Marc Torres Jimenez", 25, 395, 255, 255, 255, 255, 12);
	App->render->TextDraw("https://github.com/lSara-MM/project1-tetris", 25, 425, 255, 255, 255, 255, 12);

	//Saltar pantalla nivel 1

	if (App->input->keys[SDL_SCANCODE_F5] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sLvl_1, 0);

	}

	return update_status::UPDATE_CONTINUE;
}
