#ifndef __J1PLAYER_H_
#define __J1PLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;

enum Player_State {
	IDLE_STATE,
	RUN_STATE,
	JUMP_STATE,
	AIR_STATE,
	DUCK_STATE,
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

private:
	//Draw player in screen
	void Draw();
	//Load Player Animations
	p2Animation LoadAnimations(p2SString name);

public:
	fPoint		position;
	Collider*	player_coll = nullptr;
	
private:
	p2SString	path;
	fPoint		velocity;
	fPoint		maxVelocity;
	fPoint		acceleration;

	pugi::xml_document  player_file;

	SDL_Texture*		player_spritesheet = nullptr;
	SDL_RendererFlip	flipper = SDL_FLIP_NONE;
	Player_State		state = IDLE_STATE;
	p2Animation*        current_animation = nullptr;
	p2Animation			idle;
	p2Animation         run;
	p2Animation			jump_anim;
	p2Animation         duck;
};

#endif
