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
#include "j1Entity.h"
#include "j1Entity_Manager.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Scene2.h"

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
	path.create(config.child("path").child_value());
	map_path.create(config.child("map").attribute("path").as_string());
	music_path.create(config.child("audio").attribute("path").as_string());
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;
	LoadLevel();

	//App->player->position = { App->map->data.player_properties.Get("playerPosition.x"), App->map->data.player_properties.Get("playerPosition.y") };
	App->render->camera = { (int)App->map->data.player_properties.Get("camera.x"), (int)App->map->data.player_properties.Get("camera.y") };

	pugi::xml_parse_result result = entities_files.load_file(path.GetString());

	if (result == NULL)
	{
		LOG("Error loading enemies XML! Error: %s", result.description());
		ret = false;
	}

	App->entity_manager->LoadPlayer();
	App->entity_manager->LoadEnemies(entities_files);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
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
	
	if (App->entity_manager->GetPlayer()->position.x > (App->win->GetWidth() / App->win->GetScale()) / 4 && App->entity_manager->GetPlayer()->position.x < (App->map->data.tile_width*App->map->data.width - App->win->GetWidth() / 4) )
	{
		App->render->camera.x = -App->entity_manager->GetPlayer()->position.x*App->win->GetScale() + App->win->GetWidth() / 4;
	}

	App->render->camera.y = (int)App->map->data.player_properties.Get("camera.y");

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
	App->entity_manager->CleanUp();

	entities_files.reset();
	return true;
}

//Reload Scene
void j1Scene::Reset() const 
{
	App->fadeToBlack->FadeToBlack(App->scene, App->scene, 1.0F);
}


bool j1Scene::Load(pugi::xml_node& node)
{
	App->fadeToBlack->scene_id = node.parent().child("fadetoblack").child("scene").attribute("id").as_int();
	LOG("%i", node.parent().child("fadetoblack").attribute("scene").as_int());
	if (App->scene->IsEnabled() && App->fadeToBlack->scene_id == 2)
	{
		CleanUp();
		App->scene2->LoadLevel();
	}
	return true;
}

//Load Level
void j1Scene::LoadLevel()
{
	App->map->Load(map_path.GetString());
	App->audio->PlayMusic(music_path.GetString());
	//App->player->state = AIR_STATE;
}


