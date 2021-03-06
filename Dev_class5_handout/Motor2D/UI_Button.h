#ifndef _UI_BUTTON__H
#define _UI_BUTTON__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

enum Button_Type
{
	PLAY,
	SETTINGS,
	CONTINUE,
	CREDIT,
	MUTE,
	UNMUTE,
	CAPTO30,
	VOLUME,
	BACK,
	SAVE,
	LOAD,
	EXIT
};

class  UI_Button :public UI_GUI
{
public:
	UI_Button(const char* text, Button_Type type, SDL_Rect idle_rect, SDL_Rect* rect_hover, SDL_Rect* rect_click, UI_GUI* parent);
	~UI_Button();

	bool Start();
	bool PostUpdate();
	Button_Type	GetType();

	bool OnHover();


private:
	SDL_Rect	button_rect[MAX_STATE];
	p2SString	button_text;
	Button_Type	button_type;
};

#endif  // !_UI_BUTTON__H