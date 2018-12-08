#include "p2Defs.h"
#include "p2Log.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_GUI.h"
#include "UI_Label.h"
#include "UI_Sprite.h"
#include "UI_Button.h"

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
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	p2List_item<UI_GUI*>* item_gui = gui_list.start;
	while (item_gui != nullptr)
	{
		item_gui->data->PreUpdate();
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

	return true;
}


UI_GUI* j1Gui::CreateLabel(int x, int y, const char * text, SDL_Color color)
{
	UI_GUI* label = nullptr;
	label = new UI_Label(text, color);
	label->pos_x = x;
	label->pos_y = y;
	label->type = LABEL;

	return label;
}

UI_GUI* j1Gui::CreateSprite(int x, int y, SDL_Rect rect)
{
	UI_GUI* sprite = nullptr;
	sprite = new UI_Sprite(rect);
	sprite->pos_x = x;
	sprite->pos_y = y;
	sprite->type = SPRITE;

	return sprite;
}

UI_GUI*j1Gui::CreateButton(int x, int y, SDL_Rect rect)
{
	UI_GUI* button = nullptr;
	button = new UI_Button(rect);
	button->pos_x = x;
	button->pos_y = y;
	button->type = BUTTON;

	return button;
};
// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

