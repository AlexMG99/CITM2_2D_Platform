#ifndef __J1ENTITY_PLAYER_H_
#define __J1ENTITY_PLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;

class j1Entity_Player : public j1Entity
{
public:
	j1Entity_Player();

	//Destructor
	virtual ~j1Entity_Player();

	// Called before the first frame
	bool Entity_Start(const char* entity_name = "player");

	//Called every loop iteration
	bool Entity_PreUpdate(float dt);

	//Called evry loop iteration
	bool Entity_Update(float dt);

	//Draw Player
	bool Entity_Draw(float dt);

	void Entity_Collision(Collider* other_coll);

private:
	void CheckState();
	void PerformActions(float dt);


public:
	fPoint	acceleration;

	SDL_Rect		coll_rect;
	COLLIDER_TYPE	coll_type = COLLIDER_NONE;
	bool			godMode = false;
	bool			falling = false;
	SDL_Texture*	player_tex = nullptr;

private:
	p2Animation		jump_anim;
	p2Animation     duck_anim;
	p2Animation     cling_anim;

};

#endif

