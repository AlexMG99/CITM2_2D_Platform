#ifndef __J1ENTITY_H_
#define __J1ENTITY_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Collision.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;

enum Entity_State {

	IDLE_STATE,
	RIGHT_STATE,
	LEFT_STATE,
	FALL_STATE,
	DEATH_STATE,

};

enum Entity_Type {

	CRAB,
	BAT,

};

class j1Entity : public j1Module
{
public:
	j1Entity();

	//Destructor 
	virtual ~j1Entity();


	//Called before entities are aviable
	bool Awake(pugi::xml_node&);

	//Called before the first frame
	bool Start();

	//Called every loop iteration
	bool PreUpdate();

	//Called every loop iteration
	bool Update(float dt);

	//Called every loop iteration
	bool PostUpdate();

	//Called before quitting
	bool CleanUp();

	//Pathfinding functions

	
private:

	//Draw Entity on screen
	/*void Draw()*/;
	//Load Entity Animations
	p2Animation LoadAnimations(p2SString name) const;

public: 
	fPoint position;
	COLLIDER_TYPE coll_type = COLLIDER_NONE;
	Collider* crab_coll=nullptr;

private:
	/*Entity_State estate = IDLE;*/
	p2SString path;

	p2Animation idel_anim;
	p2Animation left_anim;
	p2Animation right_anim;


	pugi::xml_document entities_file;
	SDL_Texture* entitiesSpritesheet = nullptr;
};


#endif