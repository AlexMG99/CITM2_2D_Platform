#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "UI_Label.h"
#include "j1Map.h"
#include "j1Fonts.h"

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
	App->render->camera = { -1100,0 };
	LoadLevel();

	App->gui->gui_list.add(App->gui->CreateButton({ 435, 70 }, PLAY, "Play"));
	//App->gui->gui_list.add(App->gui->CreateButton({ 435, 140 }, SDL_Rect({ 0,0,190,49 }), "Continue"));

	////Config Button
	App->gui->gui_list.add(App->gui->CreateButton({ 435, 130 }, SETTINGS, "Settings"));

	////Orange Sprite
	App->gui->gui_list.add(App->gui->CreateSprite({ 771, 7 }, { 448,4,255,174 }));

	App->font->title = App->font->Load("fonts/kenvector_future.ttf", 34);
	App->font->title_config = App->font->Load("fonts/kenvector_future.ttf", 15);
	App->font->title_buttons = App->font->Load("fonts/kenvector_future.ttf", 26);
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