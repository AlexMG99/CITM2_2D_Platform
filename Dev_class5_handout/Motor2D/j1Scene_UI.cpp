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

	App->gui->gui_list.add(App->gui->CreateLabel({ 250, 5 }, "SCORE", Label_Type::CONFIG, {255,255,255,255}, true));
	App->gui->gui_list.add(App->gui->CreateLabel({ 130, 5 }, "TIMER", Label_Type::CONFIG, { 255,255,255,255 }, true));
	App->gui->gui_list.add(App->gui->CreateSprite({ 10,5 }, { 706,6,14,13 }, true));
	App->gui->gui_list.add(App->gui->CreateSprite({ 30,5 }, { 706,6,14,13 }, true));
	App->gui->gui_list.add(App->gui->CreateSprite({ 50,5 }, { 706,6,14,13 }, true));

	SDL_Rect audio_rect_button[3] = { { 342,178,38,38 }, {342,98,38,38},{ 294,143,39,38 } };
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

