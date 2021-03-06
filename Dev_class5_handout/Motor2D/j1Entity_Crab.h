#ifndef __J1ENTITY_CRAB_H_
#define __J1ENTITY_CRAB_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Collision.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;


class j1Entity_Crab : public j1Entity {

public:

	j1Entity_Crab();

	//Destructor
	virtual ~j1Entity_Crab();

	bool Entity_PreUpdate(float dt);

	void FollowPath(float dt);

	void StandardPath(float dt);

private:
	bool moving_right = false; //uml
	bool moving_left = true; //uml
}; 


#endif
