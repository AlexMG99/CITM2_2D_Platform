#ifndef __J1ENTITY_H_
#define __J1ENTITY_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Collision.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;

enum Entity_State {
	STATE_NONE = -1,

	STATE_IDLE,
	STATE_RUN,
	STATE_FALL,
	STATE_DEATH,
	STATE_JUMP,
	STATE_CLING,
	STATE_DUCK,
	STATE_GOD

};

enum Entity_Type {
	NONE = -1,

	PLAYER,
	CRAB,
	BAT

};


class j1Entity : public j1Module
{
public:

	j1Entity(const char* entity_name);
	
	//Destructor 
	virtual ~j1Entity();

	//Called before the first frame
	virtual bool Entity_Start(const char* entity_name) { return true; }

	//Called every loop iteration
	virtual bool Entity_PreUpdate(float dt) { return true; }

	//Called every loop iteration
	virtual bool Entity_Update(float dt);

	//Called every loop iteration
	virtual bool Entity_PostUpdate();

	//Load Game State
	bool Load(pugi::xml_node& node);

	//Save Game State
	bool Save(pugi::xml_node& node) const;

	//Check Collision
	virtual void Entity_Collision(Collider* c2) ;

protected:

	//Load Entity Animations
	p2Animation LoadAnimation(p2SString name, p2SString entity_name) const;
	
	//PerformActions
	void SetAnimations(float dt);

	//Draw Entity
	virtual bool Entity_Draw(float dt);
	//Create path from enemy to player
	bool CalculatePath();
	//Follow the path to the player
	virtual void FollowPath() {};

public:
	fPoint				position;
	fPoint				velocity;
	fPoint				acceleration;
	bool				fly = false;

	Entity_State		state = STATE_NONE;
	Entity_Type			type = NONE;

	p2SString			path;
	p2Animation*		current_animation = nullptr;
	p2Animation			idle_anim;
	p2Animation			run_anim;
	p2Animation			fall_anim;
	p2Animation			death_anim;
	Collider*			coll = nullptr;
	SDL_RendererFlip	flipX = SDL_FLIP_NONE;

	pugi::xml_document	entities_animations;

	bool                to_delete = false;


};


#endif