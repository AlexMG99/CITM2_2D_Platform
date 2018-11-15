#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Scene2.h"

j1Scene2::j1Scene2() : j1Module()
{
	name.create("scene2");
}

// Destructor
j1Scene2::~j1Scene2()
{}

// Called before render is available
bool j1Scene2::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	map_path.create(config.child("map").attribute("path").as_string());
	music_path.create(config.child("audio").attribute("path").as_string());
	return ret;
}

// Called before the first frame
bool j1Scene2::Start()
{
	LoadLevel();

	//App->player->position = { App->map->data.player_properties.Get("playerPosition.x"), App->map->data.player_properties.Get("playerPosition.y") };
	App->render->camera = { (int)App->map->data.player_properties.Get("camera.x"), (int)App->map->data.player_properties.Get("camera.y") };

	return true;
}

// Called each loop iteration
bool j1Scene2::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene2::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");
	App->map->CleanUp();
	return true;
}

//Reload Scene
void j1Scene2::Reset() const
{
	App->fadeToBlack->FadeToBlack(App->scene2, App->scene2, 1.0F);
}

bool j1Scene2::Load(pugi::xml_node& node)
{
	App->fadeToBlack->scene_id = node.parent().child("fadetoblack").child("scene").attribute("id").as_int();
	LOG("%i", node.parent().child("fadetoblack").attribute("scene").as_int());
	if (App->scene2->IsEnabled() && App->fadeToBlack->scene_id == 1)
	{
		CleanUp();
		App->scene->LoadLevel();
	}
	return true;
}

//Load Level
void j1Scene2::LoadLevel()
{
	App->map->Load(map_path.GetString());
	App->audio->PlayMusic(music_path.GetString());
	//App->player->state = AIR_STATE;
}