#ifndef __J1PLAYER_H_
#define __J1PLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Collision.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;

enum Player_State {
	NONE_STATE = -1,

	IDLE_STATE,
	RUN_STATE,
	JUMP_STATE,
	AIR_STATE,
	DUCK_STATE,
	DEAD_STATE,
};

class j1Player : public j1Module
{
public:
	j1Player();

	//Destructor
	virtual ~j1Player();

	//Called before player is aviable
	bool Awake(pugi::xml_node&);

	// Called before the first frame
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

	//Save Game State
	bool Save(pugi::xml_node&) const;

	//Check Player State
	void CheckState();

	//Perform Player Actions
	void PerformActions();

	//Check Collision
	void OnCollision(Collider* c1, Collider* c2);

private:
	//Draw player in screen
	void Draw();
	//Load Player Animations
	p2Animation LoadAnimations(p2SString name);

public:
	
	fPoint		position;
	Collider*	player_coll = nullptr;
	bool		grounded = false;
	
private:
	

	fPoint		velocity;
	fPoint		maxVelocity;
	fPoint		acceleration;
	float		jumpAcceleration;
	float		jumpMaxVelocity;

	p2SString		path;
	p2Animation*    current_animation = nullptr;
	p2Animation		idle_anim;
	p2Animation     run_anim;
	p2Animation		jump_anim;
	p2Animation		air_anim;
	p2Animation     duck_anim;
	p2Animation     dead_anim;

	pugi::xml_document  player_file;

	SDL_Texture*		playerSpritesheet = nullptr;
	SDL_Rect			coll_rect;
	SDL_RendererFlip	flipX = SDL_FLIP_NONE;
	Player_State		state = NONE_STATE;
	COLLIDER_TYPE		coll_type = COLLIDER_NONE;
	
};

#endif
