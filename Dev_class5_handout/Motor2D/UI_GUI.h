#ifndef _UI_GUI__H
#define _UI_GUI__H

#include "j1Module.h"
#include "p2Point.h"

enum UI_State
{
	IDLE,
	HOVER,
	CLICK,
	MAX_STATE
};


class UI_GUI
{
public:
	UI_GUI() {};
	virtual ~UI_GUI();

	virtual bool Start() { return true; };
	bool PreUpdate(float dt) ;
	virtual bool Update(float dt);
	virtual bool PostUpdate() { return true; };

	virtual bool OnHover() { return true; };
	bool OnClick();




public:
	UI_State	state = IDLE;
	iPoint		pos;
	iPoint		position;
	iPoint		mouse_position;
	UI_GUI*		parent = nullptr;
	/*SDL_Rect box = { 0,0,0,0 };*/
	p2Point<int> pivot = { 0,0 };

	/*SDL_Rect elementRect;*/ //interaction rect
	bool		drag = false;
	bool		static_object = false;
};




#endif  // !_UI_GUI__H

