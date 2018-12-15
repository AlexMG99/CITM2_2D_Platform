#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_GUI.h"
#include "UI_Button.h"
#include "j1Render.h"
#include "UI_Label.h"
#include "j1Map.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"


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

	LoadLevel();

	App->render->camera = { (int)App->map->data.player_properties.Get("camera.x"), (int)App->map->data.player_properties.Get("camera.y") };

	//Menu Button
	SDL_Rect rect_button[3] = { {0,0,170,43}, { 190,0,170,43 }, {0,198,170,43} };
	button_list.add(App->gui->CreateButton({ 90, 60 }, PLAY, rect_button[0], &rect_button[1], &rect_button[2], "Play"));
	button_list.add(App->gui->CreateButton({ 90, 160 }, SETTINGS, rect_button[0], &rect_button[1], &rect_button[2], "Settings"));
	button_list.add(App->gui->CreateButton({ 90, 110 }, CONTINUE, rect_button[0], &rect_button[1], &rect_button[2], "Continue"));
	button_list.add(App->gui->CreateButton({ 90, 210 }, EXIT, rect_button[0], &rect_button[1], &rect_button[2], "Exit"));
	SDL_Rect credit_rect_button[3] = { { 587,213,38,38 }, {496,213,38,38},{ 541,213,39,38 } };
	button_list.add(App->gui->CreateButton({ 30, 213 }, CREDIT, credit_rect_button[0], &credit_rect_button[1], &credit_rect_button[2]));

	SDL_Rect audio_rect_button[3] = { { 342,178,38,38 }, {342,98,38,38},{ 294,143,39,38 } };
	button_list.add(App->gui->CreateButton({ 290, 213 }, MUTE, audio_rect_button[0], &audio_rect_button[1], &audio_rect_button[2]));


	//TITLE
	App->gui->CreateSprite({ 48, 7 }, { 718,161,255,41 });

	////Orange Sprite
	App->gui->CreateSprite({ 388, 7 }, { 448,4,255,174 });


	//Slider 
	//App->gui->CreateSlider({ 506, 15}, { 450,182,133,25 });


	//Capto30
	SDL_Rect capto30_rect_button[3] = { { 422,215,36,36 }, { 458,215,36,36 }, { 381,36,36,36 } };
	App->gui->CreateButton({ 590, 89 },CAPTO30, capto30_rect_button[0], &capto30_rect_button[1], &capto30_rect_button[2]);

	
	App->gui->CreateLabel({ 56, 16 }, "Trinity's Journey", TITLE, { 0, 0, 0, 0 });
	App->gui->CreateLabel({ 416, 20 }, "Volume",	CONFIG, { 0, 0, 0, 0 });
	App->gui->CreateLabel({ 416, 60 }, "Fx  Volume", CONFIG, { 0, 0, 0, 0 });
	App->gui->CreateLabel({ 416, 100 }, "Cap  to  30  fps", CONFIG, { 0, 0, 0, 0 });

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
	bool ret = true;
	App->map->Draw();

	p2List_item<UI_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		if (button_item->data->OnClick())
		{
			switch (button_item->data->GetType())
			{
			case PLAY:
				App->fadeToBlack->FadeToBlack(App->scene_menu, App->scene);
				break;
			case SETTINGS:
				App->render->camera.x = -1100;
				break;
			case EXIT:
				ret = false;
				break;
			}
		}
		button_item = button_item->next;
	}

	return ret;
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
	LOG("Freeing Scene Menu");
	App->map->CleanUp();
	App->gui->CleanUp();
	return true;
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