#ifndef _UI_SLIDER__H
#define _UI_SLIDER__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

class UI_Sprite;

class UI_Slider: public UI_GUI
{
public:
	UI_Slider(SDL_Rect slider_box, SDL_Rect* section, UI_GUI* parent, p2Point<int> pivot = {0,0});

	~UI_Slider() {};

	bool Start();
	bool PostUpdate();

	float GetThumbValue();

private:
	SDL_Rect s_box;
	UI_Slider* thumb = nullptr;
	
};







#endif  // !_UI_SLIDER__H
