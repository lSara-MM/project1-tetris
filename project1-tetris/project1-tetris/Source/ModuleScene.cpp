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
	for (int i = 0; i < 11; i++)
	{
		start_screen.PushBack({ SCREEN_WIDTH * i, 1, SCREEN_WIDTH, SCREEN_HEIGHT });
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
	//App->collisions->AddCollider({ 0, 400, 3930, 22 }, Collider::Type::WALL);

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
	ModuleParticles fireworks;
	fireworks.FwTiming(start_screen.GetCurrentFrame().x);

	App->render->Blit(bgTexture, 0, 0, &(start_screen.GetCurrentFrame()), 0.3f);

	// Draw everything --------------------------------------
	

	return update_status::UPDATE_CONTINUE;
}