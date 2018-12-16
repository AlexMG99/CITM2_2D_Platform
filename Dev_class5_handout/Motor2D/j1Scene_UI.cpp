#include "j1App.h"
#include "j1Scene_UI.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1SceneMenu.h"
#include "p2Log.h"
#include "j1Input.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "UI_Label.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Entity_Manager.h"
#include "j1FadeToBlack.h"


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
	PERF_START(ptimer);
	bool ret = true;
	App->gui->CreateSprite({ 0,0 }, { 649,203,451,24 }, true);
	App->gui->CreateLabel({ 220, 5 }, "SCORE", Label_Type::FONT, { 255,255,255,255 }, true);
	score_label = App->gui->CreateLabel({ 290,5 }, player_score_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	App->gui->CreateLabel({ 80, 5 }, "TIMER", Label_Type::FONT, { 255,255,255,255 }, true);
	timer_label = App->gui->CreateLabel({ 165,5 }, timer_string, Label_Type::CONFIG, { 255,255,255,255 }, true);
	
	
	
	hearts[0]= (App->gui->CreateSprite({ 10,5 }, { 706,6,14,13 }, true));
	hearts[1]= (App->gui->CreateSprite({ 30,5 }, { 706,6,14,13 }, true));
	hearts[2]= (App->gui->CreateSprite({ 30,5 }, { 706,6,14,13 }, true));


	pause_ui_list.add(App->gui->CreateSprite({ 80, 30 }, { 649,228,158,26 }, true));
	pause_ui_list.add(App->gui->CreateSprite({ 80,70 }, { 824, 6, 157, 151 }, true));
	pause_ui_list.add(App->gui->CreateLabel({ 100,35 }, "Pause Menu", FONT, {255,255,255,255}, true));
	SDL_Rect button_rect[3] = { { 705,20,115,29 }, { 705,51,115,29 }, { 705,82,115,29 } };
	button_list.add(App->gui->CreateButton({ 102,75 }, Button_Type::PLAY, button_rect[0], &button_rect[1], &button_rect[2], "Resume", true));
	button_list.add(App->gui->CreateButton({ 102,108 }, Button_Type::SAVE, button_rect[0], &button_rect[1], &button_rect[2], "Save", true));
	button_list.add(App->gui->CreateButton({ 102, 141 }, Button_Type::LOAD, button_rect[0], &button_rect[1], &button_rect[2], "Load", true));
	button_list.add(App->gui->CreateButton({ 102, 185 }, Button_Type::EXIT, button_rect[0], &button_rect[1], &button_rect[2], "Exit", true));

	p2List_item<UI_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		button_item->data->visible = false;
		button_item = button_item->next;
	}
	p2List_item<UI_GUI*>* ui_item = pause_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = false;
		ui_item = ui_item->next;
	}
	return ret;
}

// Called each loop iteration
bool j1Scene_UI::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_SceneUI", Profiler::Color::CornflowerBlue);

	return true;
}

// Called each loop iteration
bool j1Scene_UI::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneUI", Profiler::Color::DarkKhaki);
	float timer = (float)ptimer.ReadMs() / 1000;
	sprintf_s(timer_string, 20, "%.2f", timer);
	timer_label->ChangeText(timer_string);
	

	if (player_lifes == 3) 
	{
		hearts[0]->visible = true;
		hearts[1]->visible = true;
		hearts[2]->visible = true;
	}
	if (player_lifes == 2)
	{
		hearts[0]->visible = true;
		hearts[1]->visible = true;
		hearts[2]->visible = false;
	}
	if (player_lifes == 1)
	{
		hearts[0]->visible = true;
		hearts[1]->visible = false;
		hearts[2]->visible = false;
	}
	if (player_lifes == 0)
	{
		hearts[0]->visible = false;
		hearts[1]->visible = false;
		hearts[2]->visible = false;
		if (App->scene) {
			App->fadeToBlack->FadeToBlack(App->scene, App->scene_menu);
		}
		else
			App->fadeToBlack->FadeToBlack(App->scene, App->scene2);

	}


	p2List_item<UI_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		if (button_item->data->OnClick())
		{
			switch (button_item->data->GetType())
			{
			case PLAY:
				ChangeVisibility();
				App->paused = false;
				break;
			case SAVE:
				App->SaveGame();
				break;
			case LOAD:
				App->LoadGame();
				break;
			case EXIT:
				App->fadeToBlack->FadeToBlack(App->scene, App->scene_menu);
				break;
			}
		}
		button_item = button_item->next;
	}

	return true;
}

// Called each loop iteration
bool j1Scene_UI::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneIntro", Profiler::Color::MediumVioletRed);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ChangeVisibility();
	}

	return ret;
}

// Called before quitting
bool j1Scene_UI::CleanUp()
{
	LOG("Freeing Scene Menu");
	button_list.clear();
	App->gui->Disable();
	return true;
}

void j1Scene_UI::ChangeVisibility()
{
	p2List_item<UI_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		button_item->data->visible = !button_item->data->visible;
		button_item = button_item->next;
	}
	p2List_item<UI_GUI*>* ui_item = pause_ui_list.start;
	while (ui_item != NULL)
	{
		ui_item->data->visible = !ui_item->data->visible;
		ui_item = ui_item->next;
	}
}

