#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Scene2.h"

j1Scene2::j1Scene2() : j1Module()
{
	name.create("scene2");
}

// Destructor
j1Scene2::~j1Scene2()
{}

// Called before render is available
bool j1Scene2::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene2::Start()
{
	App->map->Load("Level2_Map.tmx");
	App->audio->PlayMusic("audio/music/latin_industries.ogg");
	return true;
}

// Called each loop iteration
bool j1Scene2::PreUpdate()
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

	if (App->player->position.x > (App->win->GetWidth() / App->win->GetScale()) / 4)
	{
		App->render->camera.x = -App->player->position.x*App->win->GetScale() + App->win->GetWidth() / 4;
	}
	if (App->render->camera.h < App->map->data.height * App->map->data.tile_height*App->win->GetScale())
	{
		App->render->camera.y = -(int)(App->map->data.height * App->map->data.tile_height*App->win->GetScale()) + App->render->camera.h - App->player->position.y + App->win->GetHeight() / App->win->GetScale();
	}

	LOG("%f", App->player->position.y );

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

	App->map->CleanUp();

	//App->collision->CleanUp();
	LOG("Freeing scene");
	return true;
}

//Reload Scene
void j1Scene2::Reset() const
{
	App->fadeToBlack->FadeToBlack(App->scene2, App->scene2);
}