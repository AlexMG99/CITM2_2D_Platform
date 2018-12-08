#ifndef _UI_LABEL__H
#define _UI_LABEL__H

#include "p2Defs.h"
#include "UI_GUI.h"
#include "SDL/include/SDL.h"

class UI_Label : public UI_GUI
{
public:
	UI_Label(const char* text, SDL_Color color);
	~UI_Label() {};

	bool Start();
	bool PostUpdate();
	void OnHover();

private:
	p2SString label_text;
	SDL_Color label_color = { 0,0,0,255 };
public:
	mutable SDL_Texture* tex = nullptr;

};




#endif  // !_UI_LABEL__H
