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
	~UI_GUI() {};

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
	UI_GUI*		parent = nullptr;

	bool		static_object = true;

};




#endif  // !_UI_GUI__H

