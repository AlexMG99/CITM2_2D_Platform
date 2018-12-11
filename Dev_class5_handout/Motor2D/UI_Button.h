#include"p2Defs.h"
#include "UI_GUI.h"
#include "SDL/include/SDL.h"

enum Button_Type
{
	PLAY,
	CONTINUE,
	SETTINGS,
	CREDITS,
	EXIT
};

class  UI_Button :public UI_GUI
{
public:
	UI_Button(SDL_Rect rect, const char* text);
	~UI_Button() {};
	bool Start();

	bool PostUpdate();
	bool OnHover();
	bool OnClick();

private:
	SDL_Rect	button_rect;
	SDL_Rect	button_rect_hover;
	p2SString	button_text;
	Button_Type	button_type;

};

