#include "j1App.h"
#include "j1Scene_UI.h"
#include "p2Log.h"
#include "j1Input.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "UI_Label.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Timer.h"

#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1Scene_UI::j1Scene_UI() : j1Module()
{
	name.create("scene_menu");
}

// Destructor
j1Scene_UI::~j1Scene_UI()
{}

// Called before render is available
bool j1Scene_UI::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene UI");
	bool ret = true;
	path.create(config.child("path").child_value());
	return ret;
}

// Called before the first frame
bool j1Scene_UI::Start()
{
	bool ret = true;

	App->gui->gui_list.add(App->gui->CreateSprite({ 5,5 }, { 420,0,15,15 }));
	App->gui->gui_list.add(App->gui->CreateLabel({ 250, 55 }, "SCORE", Label_Type::CONFIG, {255,255,255,255}));
	App->gui->gui_list.add(App->gui->CreateLabel({ 130, 55 }, "TIMER", Label_Type::CONFIG, { 255,255,255,255 }));
	App->gui->gui_list.add(App->gui->CreateSprite({ -361,55 }, { 706,6,14,13 }));
	App->gui->gui_list.add(App->gui->CreateSprite({ -341,55 }, { 706,6,14,13 }));
	App->gui->gui_list.add(App->gui->CreateSprite({ -321,55 }, { 706,6,14,13 }));
	return ret;
}

// Called each loop iteration
bool j1Scene_UI::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_SceneUI", Profiler::Color::CornflowerBlue);

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	return true;
}

// Called each loop iteration
bool j1Scene_UI::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneUI", Profiler::Color::DarkKhaki);
	
	return true;
}

// Called each loop iteration
bool j1Scene_UI::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneIntro", Profiler::Color::MediumVioletRed);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene_UI::CleanUp()
{
	LOG("Freeing Scene Menu");
	App->gui->CleanUp();
	return true;
}


bool j1Scene_UI::Load(pugi::xml_node& node)
{

	return true;
}

