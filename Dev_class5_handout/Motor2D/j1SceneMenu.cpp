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

	App->gui->gui_list.add(App->gui->CreateButton({ 410, 95 }, SDL_Rect({ 0,0,190,49 }), "Play"));
	//App->gui->gui_list.add(App->gui->CreateButton({ 420, 145 }, SDL_Rect({ 4,1,177,40 }),"Continue" ));
	//App->gui->gui_list.add(App->gui->CreateButton({ 370, 224 }, SDL_Rect({ 5,173,120,28 }), "Exit"));

	////Config Button
	App->gui->gui_list.add(App->gui->CreateButton({ 660, 215 }, SDL_Rect({ 342,98,38,38 })));

	////App->gui->gui_list.add(App->gui->CreateButton({ 85, 35 }, SDL_Rect({ 182,5,176,40 })));
	////App->gui->gui_list.add(App->gui->CreateButton({ 85, 105 }, SDL_Rect({ 182,5,176,40 })));

	////App->gui->gui_list.add(App->gui->CreateButton({ 280, 215 }, SDL_Rect({ 259,149,38,31 })));

	////Back Arrow
	//App->gui->gui_list.add(App->gui->CreateButton({ 760, 215 }, SDL_Rect({ 390,149,38,31 })));

	//
	////Yellow Sprite
	//App->gui->gui_list.add(App->gui->CreateSprite({ 385, 13 }, { 2,93,243,75 }));
	////Orange Sprite
	App->gui->gui_list.add(App->gui->CreateSprite({ 771, 7 }, { 448,4,255,174 }));

	////Life Sprite
	App->gui->gui_list.add(App->gui->CreateSprite({ 880, 28 }, { 450,182,133,30 }));
	//
	////Life Button Move
	//App->gui->gui_list.add(App->gui->CreateButton({ 890, 28 }, SDL_Rect({ 291,247,15,16 })));

	////Cap FPS
	App->gui->gui_list.add(App->gui->CreateButton({ 965, 85 }, SDL_Rect({ 450,215,36,36 })));

	//App->gui->gui_list.add(App->gui->CreateLabel({ 400, 17 }, "Trinity's", TITLE, { 0, 0, 0, 0 }));
	//App->gui->gui_list.add(App->gui->CreateLabel({ 410, 50 }, "Journey", TITLE, { 0,0,0,0 }));
	//

	App->gui->gui_list.add(App->gui->CreateLabel({ 780, 30 }, "Volume", CONFIG, { 0, 0, 0, 0 }));
    App->gui->gui_list.add(App->gui->CreateLabel({ 780, 90 }, "Cap to 30 fps", CONFIG, { 0, 0, 0, 0 }));
	

	
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