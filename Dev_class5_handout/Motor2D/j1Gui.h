#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"

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
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	UI_GUI* CreateLabel(int x, int y, const char* text, SDL_Color color);
	UI_GUI* CreateSprite(int x, int y, SDL_Rect rect);
	UI_GUI* CreateButton(int x, int y, SDL_Rect rect);

	SDL_Texture* GetAtlas() const;
	p2List<UI_GUI*> gui_list;

private:
	SDL_Texture* atlas;
	p2SString atlas_file_name;


};

#endif // __j1GUI_H__
