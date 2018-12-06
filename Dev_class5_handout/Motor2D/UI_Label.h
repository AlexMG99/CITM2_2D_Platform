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

	bool PostUpdate();
	void OnHover();
	void Draw() const;

private:
	p2SString label_text;
	SDL_Color label_color = { 0,0,0,255 };

};




#endif  // !_UI_LABEL__H
