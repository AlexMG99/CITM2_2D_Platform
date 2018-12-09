#include"p2Defs.h"
#include "UI_GUI.h"
#include "SDL/include/SDL.h"

class  UI_Button :public UI_GUI
{
public:
	UI_Button(SDL_Rect rect, const char* text);
	~UI_Button() {};
	bool Start();

	bool PostUpdate();
	void OnHover();
	void OnClick();

private:
	SDL_Rect	button_rect;
	p2SString	button_text;

};

