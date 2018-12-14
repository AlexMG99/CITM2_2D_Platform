#ifndef _UI_SPRITE__H
#define _UI_SPRITE__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

class UI_Sprite : public UI_GUI
{
public:
	UI_Sprite(SDL_Rect rect, UI_GUI* parent);
	~UI_Sprite() {};

	bool PostUpdate();

private:
	SDL_Rect sprite_rect;

};




#endif  // !_UI_SPRITE__H

