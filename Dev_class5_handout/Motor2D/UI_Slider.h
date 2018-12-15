#ifndef _UI_SLIDER__H
#define _UI_SLIDER__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"


class UI_Slider: public UI_GUI
{
public:
	UI_Slider(SDL_Rect slider_box, UI_GUI* parent);
	~UI_Slider() {};

	bool Start();
	bool PostUpdate();


private:
	SDL_Rect s_box;
	
	
};







#endif  // !_UI_SLIDER__H
