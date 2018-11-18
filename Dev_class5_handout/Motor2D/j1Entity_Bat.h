#ifndef __J1ENTITY_BAT_H_
#define __J1ENTITY_BAT_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Collision.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;


class j1Entity_Bat : public j1Entity {

public:

	j1Entity_Bat();

	//Destructor
	virtual ~j1Entity_Bat();

	bool Entity_PreUpdate(float dt);

	void FollowPath(float dt);
	void StandardPath(float dt);

public:

	bool moving_right = false;
	bool moving_left = false;

	bool moving_down = false;
	bool moving_left_down = false;
	bool moving_down_right = false;

	bool moving_up = false;
	bool moving_up_left = false;
	bool moving_up_right = false;
};


#endif