#ifndef _UI_THUMB__H
#define _UI_THUMB__H

#include "p2Defs.h"
#include "j1Gui.h"
#include "SDL/include/SDL.h"

class SDL_Rect;

class UI_Thumb : public UI_GUI
{
public:
	UI_Thumb(SDL_Rect s_thumb ,UI_GUI* parent);
	~UI_Thumb() {};

	bool Start();
	bool PostUpdate();


private:
	SDL_Rect thumb;


};

#endif  // !_UI_THUMB__H
