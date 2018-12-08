#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "j1Map.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1SceneMenu::j1SceneMenu() : j1Module()
{
	name.create("scene_menu");
}

// Destructor
j1SceneMenu::~j1SceneMenu()
{}

// Called before render is available
bool j1SceneMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Intro");
	bool ret = true;
	path.create(config.child("path").child_value());
	map_path.create(config.child("map").attribute("path").as_string());
	return ret;
}

// Called before the first frame
bool j1SceneMenu::Start()
{
	bool ret = true;
	App->render->camera = { -336*2,0 };
	LoadLevel();

	//App->gui->gui_list.add(App->gui->CreateSprite((App->render->camera.x) + (App->render->camera.w / 2) - 328 / 2, App->render->camera.y + 50, SDL_Rect({ 485, 829, 328, 103 })));
	App->gui->gui_list.add(App->gui->CreateButton(0, 0, SDL_Rect({ 24,5,174,38 })));
	// Bright Button: App->gui->gui_list.add(App->gui->CreateButton((App->render->camera.x) + (App->render->camera.w / 2) - 328 / 2, App->render->camera.y + 50, SDL_Rect({ 411,169,229,69 })));
	//App->gui->gui_list.add(App->gui->CreateButton((App->render->camera.x) + (App->render->camera.w / 2) - 328 / 2, App->render->camera.y + 50, SDL_Rect({ 642,169,229,69 })));
	App->gui->gui_list.add(App->gui->CreateLabel(30, 30, "BONDIA", { 0,255,0,0 }));
	return ret;
}

// Called each loop iteration
bool j1SceneMenu::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_SceneIntro", Profiler::Color::CornflowerBlue);

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= dt * 200;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += dt * 200;

	return true;
}

// Called each loop iteration
bool j1SceneMenu::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneIntro", Profiler::Color::DarkKhaki);

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1SceneMenu::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneIntro", Profiler::Color::MediumVioletRed);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1SceneMenu::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

//Reload Scene
void j1SceneMenu::Reset() const
{

}


bool j1SceneMenu::Load(pugi::xml_node& node)
{

	return true;
}

//Load Level
void j1SceneMenu::LoadLevel()
{
	App->map->Load(map_path.GetString());
}