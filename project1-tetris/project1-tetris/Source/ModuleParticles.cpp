#include "ModuleParticles.h"

#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{
	 
}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = App->textures->Load("Assets/Sprites/ss_firework1.png");

	// firework 1 animation
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			firework1.anim.PushBack({ (87 * j), (i * 82), 87, 82 });
		}
	}
	//firework1.lifetime = 18;
	firework1.anim.loop = false;
	firework1.anim.speed = 0.2f;

	//// firework 2 animation
	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		if (j == 3 && i == 3)
	//		{
	//			break;
	//		}

	//		firework2.anim.PushBack({ (77 * j), (i * 71), 77, 71 });
	//	}
	//}
	////firework2.lifetime = 8;
	//firework2.anim.loop = false;
	//firework2.anim.speed = 0.3f;

	laser.anim.PushBack({ 232, 103, 16, 12 });
	laser.anim.PushBack({ 249, 103, 16, 12 });
	laser.speed.x = 5;
	laser.lifetime = 180;
	laser.anim.speed = 0.2f;


	return true;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

//void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
//{
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		// Always destroy particles that collide
//		if (particles[i] != nullptr && particles[i]->collider == c1)
//		{
//			// TODO 6: Make so every time a particle hits a wall it triggers an explosion particle
//			AddParticle(explosion, particles[i]->position.x, particles[i]->position.y);
//
//			delete particles[i];
//			particles[i] = nullptr;
//			break;
//		}
//	}
//}

update_status ModuleParticles::Update()
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if(particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if(particle->Update() == false)
		{
			delete particle;
			particles[i] = nullptr;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			App->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));

		}


	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, uint delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			Particle* p = new Particle(particle);

			p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			p->position.y = y;

			////Adding the particle's collider
			//if (colliderType != Collider::Type::NONE)
			//	p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = p;
			break;
		}
	}
}

void ModuleParticles::FwTiming(int x_frame)
{
	if (x_frame == 0)
	{
		SDL_Delay(500);		// "5 segundos"
	}

	if (x_frame == 6600)
	{
		App->particles->AddParticle(App->particles->firework1, 430, 100, 100);
		App->particles->AddParticle(App->particles->firework1, 150, 50, 200);
		//SDL_Delay(50);
	}
}