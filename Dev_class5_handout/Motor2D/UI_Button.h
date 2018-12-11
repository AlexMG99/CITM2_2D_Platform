#include"p2Defs.h"
#include "UI_GUI.h"
#include "SDL/include/SDL.h"

class  UI_Button :public UI_GUI
{
public:
	UI_Button(const char* text);
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

};

