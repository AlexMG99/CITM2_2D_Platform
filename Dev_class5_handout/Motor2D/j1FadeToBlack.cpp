#include <math.h>
#include "j1App.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1SceneMenu.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "p2Log.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

#include "Brofiler/Brofiler.h"

j1FadeToBlack::j1FadeToBlack()
{
	name.create("fadetoblack");
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	uint   width = 0;
	uint   height = 0;
	App->win->GetWindowSize(width, height);

	screen = { 0, 0, (int)(width*App->win->GetScale()), (int)(height*App->win->GetScale()) };

	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_FadeToBlack", Profiler::Color::MediumVioletRed);
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			to_disable->Disable();
			to_enable->Enable();
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_enable = module_on;
		to_disable = module_off;
		ret = true;
	}

	return ret;
}

bool j1FadeToBlack::IsFading() const
{
	return current_step != fade_step::none;
}

//Save Game State
bool j1FadeToBlack::Save(pugi::xml_node& data) const
{
	pugi::xml_node fade= data.append_child("scene");
	if (App->scene_menu->IsEnabled())
	{
		scene_id = 0;
	}
	else if (App->scene->IsEnabled())
	{
		scene_id = 1;
	}
	else if(App->scene2->IsEnabled())
	{
		scene_id = 2;
	}
	fade.append_attribute("id") = scene_id;

	return true;
}