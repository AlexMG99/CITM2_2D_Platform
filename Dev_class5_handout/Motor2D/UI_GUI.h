#ifndef _UI_GUI__H
#define _UI_GUI__H

#include "j1Module.h"
#include "p2Point.h"

enum UI_State
{
	IDLE,
	HOVER,
	CLICK
};


class UI_GUI
{
public:
	UI_GUI() {};
	~UI_GUI() {};

	virtual bool Start() { return true; };
	bool PreUpdate(float dt) ;
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };

	virtual bool OnClick() { return true; };
	virtual bool OnHover() { return true; };




public:
	UI_State	state = IDLE;
	iPoint		pos;

};




#endif  // !_UI_GUI__H

