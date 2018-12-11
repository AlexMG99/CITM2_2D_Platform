#ifndef _UI_BUTTON__H
#define _UI_BUTTON__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

enum Button_Type
{
	PLAY,
	EXIT
};

class  UI_Button :public UI_GUI
{
public:
	UI_Button(const char* text, Button_Type type);
	~UI_Button() {};
	bool Start();

	bool PostUpdate();
	bool OnHover();
	bool OnClick();

private:
	SDL_Rect	button_rect;
	SDL_Rect	button_rect_hover;
	SDL_Rect	button_rect_click;
	p2SString	button_text;
	Button_Type	button_type;

};

#endif  // !_UI_BUTTON__H