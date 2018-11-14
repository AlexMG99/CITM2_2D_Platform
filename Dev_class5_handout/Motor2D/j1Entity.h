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
	ENTITY_STATE_NONE = -1,

	ENTITY_STATE_IDLE,
	ENTITY_STATE_RIGHT,
	ENTITY_STATE_LEFT,

};

enum Entity_Type {
	ENTITY_NONE = -1,

	ENTITY_PLAYER,
	ENTITY_CRAB,
	ENTITY_BAT,

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

	//Load Game State
	bool Load(pugi::xml_node&);

	//PerformActions
	void SetAnimations(float dt);

	//Pathfinding functions
	


private:

	////Draw Entity on screen
	//void Draw(float dt) const;
	//Load Entity Animations
	/*p2Animation LoadCrabAnimations(p2SString name) const;
	p2Animation LoadBatAnimations(p2SString name) const;*/
	//Debug functionallity

public:
	fPoint crab1_position;
	fPoint bat1_position;
	SDL_Rect crab_rect;
	SDL_Rect bat_rect;

	Collider* crab_coll = nullptr;
	Collider* bat_coll = nullptr;


	
	Entity_State state = ENTITY_STATE_NONE;
	Entity_Type type = ENTITY_NONE;



private:

	p2SString path;
	p2Animation* currentcrab_animation = nullptr;
	p2Animation* currentbat_animation = nullptr;
	p2Animation  crab_idle;
	p2Animation crab_right;
	p2Animation  crab_left;

	p2Animation  bat_idle;
	p2Animation  bat_right;
	p2Animation  bat_left;
	pugi::xml_document entities_file;

	SDL_Texture* entitiesSpritesheet = nullptr;
	//SDL_RendererFlip   flipX = SDL_FLIP_NONE;
};


#endif