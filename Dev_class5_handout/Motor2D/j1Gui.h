#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "UI_Label.h"
#include "UI_Button.h"
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

	UI_GUI* CreateLabel(iPoint pos, const char* text, Label_Type type, SDL_Color color = {0,0,0,0});
	UI_GUI* CreateSprite(iPoint pos, SDL_Rect rect);
	UI_GUI* CreateButton(iPoint pos, Button_Type type, const char* text = "");

	SDL_Texture* GetAtlas() const;

public:
	p2List<UI_GUI*> gui_list;

private:
	SDL_Texture* atlas;
	p2SString atlas_file_name;


};

#endif // __j1GUI_H__
