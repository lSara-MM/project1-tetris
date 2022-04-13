#include "ModuleScene.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "ModuleParticles.h"


ModuleScene::ModuleScene()
{
	// Start Screen
	for (int i = 0; i < 12; i++)
	{
		start_screen.PushBack({ 600 * i, 1, 600, 422 });
	}

	start_screen.loop = false;
	start_screen.speed = 0.09f;
}

ModuleScene::~ModuleScene()
{

}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	bgTexture = App->textures->Load("Assets/ss_startBg.png");
	
	App->audio->PlayMusic("Assets/Audio/Music (.ogg)/Loginska", 1.0f);

	// Colliders ---
	App->collisions->AddCollider({ 0, 400, 3930, 22 }, Collider::Type::WALL);

	return ret;
}

update_status ModuleScene::Update()
{
	start_screen.Update();
	
	App->render->camera.x += 0;

	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleScene::PostUpdate()
{
	if (start_screen.GetCurrentFrame().x == 0)
	{
		SDL_Delay( 500 );		// "5 seconds"
	}
	if (start_screen.GetCurrentFrame().x == 6600)
	{
		App->particles->AddParticle(App->particles->firework1, 430, 100, 1);
		App->particles->AddParticle(App->particles->firework1, 150, 50, 200);
		//SDL_Delay(50);
	}


	App->render->Blit(bgTexture, 0, 0, &(start_screen.GetCurrentFrame()), 0.3f);

	// Draw everything --------------------------------------
	

	return update_status::UPDATE_CONTINUE;
}