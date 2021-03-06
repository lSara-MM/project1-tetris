#include "ModuleRender.h"

#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_scancode.h"

ModuleRender::ModuleRender(bool startEnabled) : Module(startEnabled)
{

}

ModuleRender::~ModuleRender()
{

}

bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;	
	Uint32 flags = 0;

	if (VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	/*if (TTF_Init() != 0)
	{
		LOG("True Type Font could not initialize. SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}*/

	return ret;
}

// Called every draw update
update_status ModuleRender::PreUpdate()
{
	if (TTF_Init() != 0)
	{
		LOG("True Type Font could not initialize. SDL_Error: %s\n", SDL_GetError());
	}
	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Clear rendering target
	SDL_RenderClear(renderer);

if (SCREEN_SIZE == 3 && WIN_FULLSCREEN_DESKTOP == 1)
		SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3);
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRender::Update()
{
	////Handle positive vertical movement
	//if (App->input->keys[SDL_SCANCODE_UP] == KEY_REPEAT)
	//	camera.y -= cameraSpeed+3;

	////Handle negative vertical movement
	//if (App->input->keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)
	//	camera.y += cameraSpeed+3;

	//if (App->input->keys[SDL_SCANCODE_LEFT] == KEY_REPEAT)
	//	camera.x -= cameraSpeed+3;
	//if (camera.x < 0) camera.x = 0;

	//if (App->input->keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
	//	camera.x += cameraSpeed+3;


	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	//Update the screen
	SDL_RenderPresent(renderer);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy the rendering context
	if (renderer != nullptr)
		SDL_DestroyRenderer(renderer);

	

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed)
{
	bool ret = true;

	SDL_Rect rect {
		(int)(-camera.x) + x * SCREEN_SIZE,
		(int)(-camera.y) + y * SCREEN_SIZE,
		0, 0 };
	
	if (section != nullptr)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		//Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, float speed)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect dstRect {
		(int)(-camera.x * speed) + rect.x * SCREEN_SIZE,
		(int)(-camera.y * speed) + rect.y * SCREEN_SIZE,
		rect.w * SCREEN_SIZE, rect.h * SCREEN_SIZE };

	if (SDL_RenderFillRect(renderer, &dstRect) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::TextDraw(const char* text, int x, int y, int red, int green, int blue, int alpha, int size)
{
	bool ret = true;
	ttf_font = TTF_OpenFont("Assets/font_tetris.ttf", size);
	if (!ttf_font)
	{
		LOG("Cannot open font. TTF_OpenFont error: %s", TTF_GetError());
		ret = false;
	}
	SDL_Color ttf_color;
	ttf_color.r = red;
	ttf_color.g = green;
	ttf_color.b = blue;
	ttf_color.a = alpha;

	SDL_Rect ttf_rect;
	ttf_surface = TTF_RenderText_Solid(ttf_font, text, ttf_color);
	ttf_texture = SDL_CreateTextureFromSurface(renderer, ttf_surface);

	if (ttf_surface == nullptr)
	{
		LOG("Cannot open font. SDL_Surface* error: %s", SDL_GetError());
		ret = false;
	}
	else
	{
		ttf_rect.x = x * SCREEN_SIZE;
		ttf_rect.y = y * SCREEN_SIZE;
		ttf_rect.w = ttf_surface->w * SCREEN_SIZE;
		ttf_rect.h = ttf_surface->h * SCREEN_SIZE;

		SDL_FreeSurface(ttf_surface);
		if (SDL_RenderCopy(renderer, ttf_texture, NULL, &ttf_rect) != 0)
		{
			LOG("Cannot render text to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
		SDL_DestroyTexture(ttf_texture);
		ttf_texture = nullptr;
		TTF_CloseFont(ttf_font);
	}

	return ret;
}
void ModuleRender::ttfQuit()
{
	//TTF_Quit();
}
