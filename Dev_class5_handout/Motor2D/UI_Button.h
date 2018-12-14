#ifndef _UI_BUTTON__H
#define _UI_BUTTON__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

enum Button_Type
{
	PLAY,
	SETTINGS,
	CREDIT,
	MUTE,
	CAPTO30,
	VOLUME,
	EXIT
};

class  UI_Button :public UI_GUI
{
public:
	UI_Button(const char* text, Button_Type type, SDL_Rect idle_rect, SDL_Rect* rect_hover, SDL_Rect* rect_click, UI_GUI* parent);
	~UI_Button() {};

	bool Start();
	bool PostUpdate();

	bool OnHover();


private:
	SDL_Rect	button_rect[MAX_STATE];
	p2SString	button_text;
	Button_Type	button_type;

	float	velocity = 0.00f;
	

};

#endif  // !_UI_BUTTON__H