#ifndef _UI_SLIDER__H
#define _UI_SLIDER__H

#include "p2Defs.h"
#include "UI_GUI.h"
#include "SDL/include/SDL.h"


class UI_Slider: public UI_GUI
{
public:
	UI_Slider();
	~UI_Slider() {};

	bool Start();
	bool PostUpdate();
	/*void OnClick();*/

private:
	

public:
	
};







#endif  // !_UI_SLIDER__H
