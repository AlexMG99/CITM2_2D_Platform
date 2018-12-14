#include "p2Defs.h"
#include "p2Log.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_GUI.h"
#include "UI_Label.h"
#include "UI_Slider.h"
#include "UI_Sprite.h"
#include "UI_Thumb.h"
#include "SDL/include/SDL.h"



j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	p2List_item<UI_GUI*>* item_gui = gui_list.start;
	while (item_gui != nullptr)
	{
		item_gui->data->Start();
		item_gui = item_gui->next;
	}

	return true;
}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	p2List_item<UI_GUI*>* item_gui = gui_list.start;
	while (item_gui != nullptr)
	{
		item_gui->data->PreUpdate(dt);
		item_gui = item_gui->next;
	}
	return true;
}

bool j1Gui::Update(float dt)
{
	p2List_item<UI_GUI*>* item_gui = gui_list.start;
	while (item_gui != nullptr)
	{
		item_gui->data->Update(dt);
		item_gui = item_gui->next;
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<UI_GUI*>* item_gui = gui_list.start;
	while (item_gui != nullptr)
	{
		item_gui->data->PostUpdate();
		item_gui = item_gui->next;
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI_GUI*>* item;
	item = gui_list.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	gui_list.clear();

	return true;
}


UI_GUI* j1Gui::CreateLabel(iPoint pos, const char * text, Label_Type type, SDL_Color color, UI_GUI* parent)
{
	UI_GUI* label = nullptr;
	label = new UI_Label(text, type, color, parent);
	label->pos.x = pos.x;
	label->pos.y = pos.y;
	interactive = true;

	return label;
}

UI_GUI* j1Gui::CreateSprite(iPoint pos, SDL_Rect rect)
{
	UI_GUI* sprite = nullptr;
	sprite = new UI_Sprite(rect);
	sprite->pos.x = pos.x;
	sprite->pos.y = pos.y;
	interactive = true;

	return sprite;
}

UI_GUI* j1Gui::CreateButton(iPoint pos, Button_Type type, SDL_Rect idle_rect, SDL_Rect* idle_hover, SDL_Rect* idle_click, const char* text)
{
	UI_GUI* button = nullptr;
	button = new UI_Button(text, type, idle_rect, idle_hover, idle_click);
	button->pos.x = pos.x;
	button->pos.y = pos.y;
	interactive = true;
	return button;
}
UI_GUI * j1Gui::CreateSlider(iPoint pos, SDL_Rect slider_box)
{
	UI_GUI*slider = nullptr;
	slider = new UI_Slider(slider_box);
	slider->pos.x = pos.x;
	slider->pos.y = pos.y;
	/*interactive = true;*/



	return slider;
};


// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

