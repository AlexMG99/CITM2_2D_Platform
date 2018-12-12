#ifndef _UI_SLIDER__H
#define _UI_SLIDER__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

enum Slider_type {
	SLIDER
};
class UI_Slider: public UI_GUI
{
public:
	UI_Slider(SDL_Rect rect, Slider_type type);
	~UI_Slider() {};

	bool Start();
	bool PostUpdate();
	/*void OnClick();*/


private:
	SDL_Rect slider_rect;
	SDL_Rect slider_hover;
	SDL_Rect slider_click;


	Slider_type	slider_type;
	SDL_Rect sprite_rect;
	
};







#endif  // !_UI_SLIDER__H
