#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_scancode.h"

#include <iostream>
using namespace std;
#include <sstream>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	// idle animation - just one sprite
	idleAnim.PushBack({ 66, 1, 32, 14 });

	// move upwards
	upAnim.PushBack({ 100, 1, 32, 14 });
	upAnim.PushBack({ 132, 0, 32, 14 });
	upAnim.loop = false;
	upAnim.speed = 0.1f;

	// Move down
	downAnim.PushBack({ 33, 1, 32, 14 });
	downAnim.PushBack({ 0, 1, 32, 14 });
	downAnim.loop = false;
	downAnim.speed = 0.1f;
}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	//texture = App->textures->Load("Assets/ship.png"); // arcade version

	//currentAnimation = &idleAnim;

	texture = App->textures->Load("Assets/Sprites/Tetramino/Pink_Tetra/Pink_3.png");

	fxFall = App->audio->LoadFx("Assets/Audio/FX/block-fall.wav");
	

	position.x = 150;
	position.y = 120;

	// TODO 3: Add a collider to the player
	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);

	return ret;
}

update_status ModulePlayer::Update()
{
	// Moving the player with the camera scroll
	App->player->position.y+=1;

	if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN) {
		position.x -= 10;
	}

	if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN) {
		position.x += 10;
	}

	if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_DOWN) {
		position.y += 10;
	}

	if (App->input->keys[SDL_SCANCODE_F] == KEY_STATE::KEY_DOWN) {

		texture = App->textures->Load("Assets/Sprites/Tetramino/Pink_Tetra/Pink_2.png");
		collider = App->collisions->AddCollider({ position.x, position.y, 16, 32 }, Collider::Type::PLAYER, this);

	}

	if (App->input->keys[SDL_SCANCODE_G] == KEY_STATE::KEY_DOWN) {
		position.y += 10;
	}

	//if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
	//{
	//	position.x -= speed;
	//}

	//if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
	//{
	//	position.x += speed;
	//}

	//if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	//{
	//	position.y += speed;
	//	if (currentAnimation != &downAnim)
	//	{
	//		downAnim.Reset();
	//		currentAnimation = &downAnim;
	//	}
	//}

	//if (App->input->keys[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)
	//{
	//	position.y -= speed;
	//	if (currentAnimation != &upAnim)
	//	{
	//		upAnim.Reset();
	//		currentAnimation = &upAnim;
	//	}
	//}

	// If no up/down movement detected, set the current animation back to idle
	if (App->input->keys[SDL_SCANCODE_S] == KEY_STATE::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == KEY_STATE::KEY_IDLE)
		currentAnimation = &idleAnim;

	// TODO 4: Update collider position to player position
	collider->SetPos(position.x, position.y);

	currentAnimation->Update();

	if (destroyed)
	{
		destroyedCountdown--;
		if (destroyedCountdown <= 0)
			return update_status::UPDATE_STOP;
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	return update_status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	//	// TODO 5: Detect collision with a wall. If so, go back to intro screen.
	//	if (c1 == collider && destroyed == false)
	//	{
	//		App->particles->AddParticle(App->particles->firework1, position.x, position.y, 9);
	//		App->particles->AddParticle(App->particles->firework1, position.x + 8, position.y + 11, 14);
	//		App->particles->AddParticle(App->particles->firework1, position.x - 7, position.y + 12, 40);
	//		App->particles->AddParticle(App->particles->firework1, position.x + 5, position.y - 5, 28);
	//		App->particles->AddParticle(App->particles->firework1, position.x - 4, position.y - 4, 21);
	//
	//		destroyed = true;
	//	}
}