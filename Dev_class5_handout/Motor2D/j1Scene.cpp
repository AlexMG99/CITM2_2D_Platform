#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	map_path.create(config.child("map").attribute("path").as_string());
	music_path.create(config.child("audio").attribute("path").as_string());
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(map_path.GetString());
	App->audio->PlayMusic(music_path.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	
	if (App->player->position.x > (App->win->GetWidth() / App->win->GetScale()) / 4) 
	{
		App->render->camera.x = -App->player->position.x*App->win->GetScale() + App->win->GetWidth() / 4;
	}
	
	App->map->Draw();

	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->CleanUp();
	return true;
}

//Reload Scene
void j1Scene::Reset() const 
{
	App->fadeToBlack->FadeToBlack(App->scene, App->scene, 1.0F);
}
