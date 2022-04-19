#include "ScreenLvl_1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "ModuleInput.h"

#include "ModuleParticles.h"
#include "ModuleWindow.h"
//#include "ModuleFadeToBlack.h"

ScreenLvl_1::ScreenLvl_1(bool startEnabled) : Module(startEnabled)
{


}

ScreenLvl_1::~ScreenLvl_1()
{

}

// Load assets
bool ScreenLvl_1::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bg_texture = App->textures->Load("Assets/upccitm.png");


	return ret;
}

update_status ScreenLvl_1::Update()
{

	start_screen.Update();


	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenLvl_1::PostUpdate()
{


	App->render->Blit(bg_texture, 75, 40, NULL);



	App->render->TextDraw("This is a project made by students of the bachelors ", 25, 200, 255, 255, 255, 255, 12);
	App->render->TextDraw("degree in Video Game Design and Development of the", 25, 230, 255, 255, 255, 255, 12);
	App->render->TextDraw("Centre de la Imatge i de la Tecnologia Multimedia", 25, 260, 255, 255, 255, 255, 12);

	App->render->TextDraw("Project I", 25, 335, 255, 255, 255, 255, 12);
	App->render->TextDraw("Jesus Alonso Alonso", 25, 365, 255, 255, 255, 255, 12);
	App->render->TextDraw("Marc Torres Jimenez", 25, 395, 255, 255, 255, 255, 12);
	App->render->TextDraw("https://github.com/lSara-MM/project1-tetris", 25, 425, 255, 255, 255, 255, 12);


	return update_status::UPDATE_CONTINUE;
}
