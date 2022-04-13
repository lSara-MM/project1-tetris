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
	texture_fw1 = App->textures->Load("Assets/Sprites/ss_firework1.png");
	texture_fw2 = App->textures->Load("Assets/Sprites/ss_firework2.png");

	// firework 1 animation
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// twice to make the animation smoother
			firework1.anim.PushBack({ (87 * j), (i * 82), 87, 82 });
			firework1.anim.PushBack({ (87 * j), (i * 82), 87, 82 });
		}
	}
	//firework1.lifetime = 18;
	firework1.anim.loop = false;
	firework1.anim.speed = 0.2f;
	firework1.id = 1;

	// firework 2 animation
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// the animation has only 8 sprites in a 3x3
			if (j == 3 && i == 3)
			{
				break;
			}

			// twice to make the animation smoother
			firework2.anim.PushBack({ (77 * j), (i * 71), 77, 71 });
			firework2.anim.PushBack({ (77 * j), (i * 71), 77, 71 });
		}
	}
	//firework2.lifetime = 8;
	firework2.anim.loop = false;
	firework2.anim.speed = 0.2f;
	firework1.id = 2;


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
			// If firework in position 'i' is 1, render animation 1, else render animation 2
			if (particles[i]->id == 1)
			{
				App->render->Blit(texture_fw1, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
			}
			if (particles[i]->id == 2)
			{
				App->render->Blit(texture_fw2, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
			}
		}

	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, int id_fw, uint delay)
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
			p->id = id_fw;

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
		// (Particle, int x position, int y position, int firework id, int delay) -> a millorar la mecanica del id

		// TO DO:
		// ajustar timing
		// canviar tamaño de algunos firework
		// ajustar para que solo se reproduzcan una vez cada uno
		App->particles->AddParticle(App->particles->firework1, 414, 47, 1, 75);
		App->particles->AddParticle(App->particles->firework1, 114, 32, 1, 125);
		App->particles->AddParticle(App->particles->firework2, 332, 32, 2, 200);

		App->particles->AddParticle(App->particles->firework2, 32, 78, 2, 250);
		App->particles->AddParticle(App->particles->firework2, 503, 75, 2, 250);
		App->particles->AddParticle(App->particles->firework1, 114, 33, 2, 275);

		App->particles->AddParticle(App->particles->firework2, 32, 78, 2, 325);
		App->particles->AddParticle(App->particles->firework2, 503, 75, 2, 325);
		App->particles->AddParticle(App->particles->firework1, 414, 47, 1, 350);

		App->particles->AddParticle(App->particles->firework2, 332, 32, 2, 500);

		App->particles->AddParticle(App->particles->firework1, 114, 32, 1, 550);

		App->particles->AddParticle(App->particles->firework2, 32, 78, 2, 600);
		App->particles->AddParticle(App->particles->firework2, 503, 75, 2, 600);

		App->particles->AddParticle(App->particles->firework1, 414, 47, 1, 650);

		App->particles->AddParticle(App->particles->firework2, 332, 32, 2, 700);

		App->particles->AddParticle(App->particles->firework1, 114, 32, 1, 750);

		App->particles->AddParticle(App->particles->firework2, 32, 78, 2, 800);
		App->particles->AddParticle(App->particles->firework2, 503, 75, 2, 800);
		App->particles->AddParticle(App->particles->firework1, 114, 32, 1, 825);

		App->particles->AddParticle(App->particles->firework1, 414, 47, 1, 900);
		App->particles->AddParticle(App->particles->firework2, 503, 75, 2, 900);
		App->particles->AddParticle(App->particles->firework1, 114, 32, 1, 925);

		App->particles->AddParticle(App->particles->firework2, 332, 32, 2, 1000);

		App->particles->AddParticle(App->particles->firework1, 414, 47, 1, 1100);
		App->particles->AddParticle(App->particles->firework1, 114, 32, 1, 1100);

		App->particles->AddParticle(App->particles->firework2, 32, 78, 2, 1175);
		App->particles->AddParticle(App->particles->firework2, 503, 75, 2, 1175);

		App->particles->AddParticle(App->particles->firework2, 332, 32, 2, 2000);

		//SDL_Delay(50);
	}
}
