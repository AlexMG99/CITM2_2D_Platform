#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "UI_Label.h"
#include "UI_Button.h"
#include "UI_Slider.h"
#include "UI_Sprite.h"
#include "UI_Thumb.h"
#include "SDL/include/SDL.h"

#define CURSOR_WIDTH 2

struct UI_GUI;


// TODO 1: Create your structure of classes

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	UI_Label* CreateLabel(iPoint pos, const char* text, Label_Type type, SDL_Color color = {0,0,0,0}, bool static_obj = false, UI_GUI* parent = nullptr);
	UI_Button* CreateButton(iPoint pos, Button_Type type, SDL_Rect idle_rect, SDL_Rect* idle_hover = NULL, SDL_Rect* idle_click = NULL, const char* text = "", bool static_obj = false, UI_GUI* parent = nullptr);
	UI_Slider* CreateSlider(iPoint pos, SDL_Rect slider_box, UI_GUI* parent = nullptr);
	UI_GUI* CreateSprite(iPoint pos, SDL_Rect rect, bool static_obj = false, UI_GUI * parent = nullptr);
	UI_Thumb* CreateThumb(iPoint pos, SDL_Rect s_thumb, UI_GUI* parent = nullptr);
	SDL_Texture* GetAtlas() const;

public:
	p2List<UI_GUI*> gui_list;
	uint			fx_buton_pressed;
private:
	SDL_Texture*	atlas;
	p2SString		atlas_file_name;
	p2SString		fx_button_pressed_string;
	


};

#endif // __j1GUI_H__
