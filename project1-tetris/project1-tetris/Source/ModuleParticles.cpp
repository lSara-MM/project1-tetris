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

	int v = 0;		// counter
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
			if (j == 2 && i == 2)
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

	if (x_frame == (SCREEN_WIDTH * 10))
	{
		// (Particle, int x position, int y position, int firework id, int delay) -> a millorar la mecanica del id

		// TO DO:
		// canviar tamaño de algunos firework (pending / optional)
		switch (v)
		{
			case 0:
				App->particles->AddParticle(App->particles->firework1, 464, 62, 1, 10);
				App->particles->AddParticle(App->particles->firework1, 128, 36, 1, 50);

				App->particles->AddParticle(App->particles->firework2, 372, 36, 2, 125);
				v++;

				break;
			case 1:
				App->particles->AddParticle(App->particles->firework2, 36, 89, 2, 175);
				App->particles->AddParticle(App->particles->firework2, 490, 88, 2, 180);

				App->particles->AddParticle(App->particles->firework1, 128, 33, 1, 225);
				v++;

				break;
			case 2:
				App->particles->AddParticle(App->particles->firework1, 400, 62, 1, 300);

				App->particles->AddParticle(App->particles->firework2, 36, 89, 2, 350);
				App->particles->AddParticle(App->particles->firework2, 554, 88, 2, 350);
				v++;

				break;
			case 3:
				App->particles->AddParticle(App->particles->firework2, 372, 36, 2, 450);

				App->particles->AddParticle(App->particles->firework1, 128, 36, 1, 525);
				v++;

				break;
			case 4:
				App->particles->AddParticle(App->particles->firework2, 36, 89, 2, 600);
				App->particles->AddParticle(App->particles->firework2, 490, 88, 2, 600);

				
				v++;
				break;
			case 5:
				App->particles->AddParticle(App->particles->firework1, 464, 62, 1, 700);

				App->particles->AddParticle(App->particles->firework2, 372, 36, 2, 850);

				App->particles->AddParticle(App->particles->firework1, 128, 36, 1, 950);
				v++;

				break;
			case 6:
				App->particles->AddParticle(App->particles->firework2, 36, 89, 2, 1050);
				App->particles->AddParticle(App->particles->firework2, 490, 88, 2, 1060);
				App->particles->AddParticle(App->particles->firework1, 128, 36, 1, 1105);
				v++;
				break;
			case 7:
				App->particles->AddParticle(App->particles->firework1, 400, 62, 1, 1175);

				App->particles->AddParticle(App->particles->firework2, 36, 89, 2, 1250);
				App->particles->AddParticle(App->particles->firework2, 554, 88, 2, 1260);
				v++;

				break;
			case 8:
				App->particles->AddParticle(App->particles->firework2, 372, 36, 2, 1300);

				App->particles->AddParticle(App->particles->firework1, 464, 62, 1, 1350);
				App->particles->AddParticle(App->particles->firework1, 128, 36, 1, 1350);
				v++;

				break;
			case 9:
				App->particles->AddParticle(App->particles->firework2, 36, 89, 2, 1450);
				App->particles->AddParticle(App->particles->firework2, 490, 85, 2, 1460);

				App->particles->AddParticle(App->particles->firework2, 372, 36, 2, 1500);
				v++;
				break;
			default:
				break;
		}
		
		//SDL_Delay(50);
	}
}
