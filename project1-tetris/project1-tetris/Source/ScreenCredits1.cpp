#include "ScreenCredits1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
//#include "ModuleAudio.h"

#include "ModuleInput.h"

#include "ModuleParticles.h"
#include "ModuleFadeToBlack.h"

ScreenCredits1::ScreenCredits1(bool startEnabled) : Module(startEnabled)
{


}

ScreenCredits1::~ScreenCredits1()
{

}

// Load assets
bool ScreenCredits1::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bg_texture = App->textures->Load("Assets/quokka.png");


	return ret;
}

update_status ScreenCredits1::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sCredits2, 90);
	}

	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ScreenCredits1::PostUpdate()
{
	App->render->Blit(bg_texture, 100, -100, NULL);

	App->render->TextDraw("Quokka Games Studios", 125, 270, 255, 157, 0, 255, 20);
	App->render->TextDraw("Team Members:", 125, 310, 255, 255, 255, 255, 12);
	App->render->TextDraw("Sonia Cristina Ojeda Lanz", 160, 335, 255, 255, 255, 255, 12);
	App->render->TextDraw("Sara Qiao Martinez Mauri", 160, 360, 255, 255, 255, 255, 12);
	App->render->TextDraw("Yina Lin", 160, 385, 255, 255, 255, 255, 12);
	App->render->TextDraw("Alberto Hidalgo Garcia", 160, 410, 255, 255, 255, 255, 12);
	App->render->TextDraw("Andreu Nosas Soler", 160, 435, 255, 255, 255, 255, 12);


	return update_status::UPDATE_CONTINUE;
}
