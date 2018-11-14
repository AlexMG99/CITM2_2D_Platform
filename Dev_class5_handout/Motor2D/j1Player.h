#ifndef __J1PLAYER_H_
#define __J1PLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"
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
	CLING_STATE,
	DEATH_STATE,
	GOD_STATE,
};

class j1Player : public j1Entity
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
	bool PreUpdate(float dt) override;

	//Called every loop iteration
	bool Update(float dt) override;

	//Called every loop iteration
	bool PostUpdate() override;
  
	//Called before quitting
	bool CleanUp();

	//Load Game State
	bool Load(pugi::xml_node&);

	//Save Game State
	bool Save(pugi::xml_node&) const;

	//Check Player State
	void CheckState();

	//Perform Player Actions
	void PerformActions(float dt);

	//Check Collision
	void OnCollision(Collider* c1, Collider* c2);


private:
	//Draw player in screen
	void Draw(float dt) const;
	//Load Player Animations
	p2Animation LoadAnimations(p2SString name) const;
	//Debug functionallity
	void DebugInput();

public:
	
	fPoint	position;
	fPoint	velocity;
	fPoint	acceleration;

	SDL_Rect		coll_rect;
	Collider*		player_coll = nullptr;
	COLLIDER_TYPE	coll_type = COLLIDER_NONE;
	Player_State	state = NONE_STATE;
	bool			godMode = false;
	bool			falling = false;
	
private:

	p2SString		path;
	p2SString		fx_death_name;
	p2SString		fx_jump_name;
	p2Animation*    current_animation = nullptr;
	p2Animation		idle_anim;
	p2Animation     run_anim;
	p2Animation		jump_anim;
	p2Animation		air_anim;
	p2Animation     duck_anim;
	p2Animation     cling_anim;
	p2Animation     dead_anim;
	uint			fx_death;
	uint			fx_jump;

	pugi::xml_document  player_file;

	SDL_Texture*		playerSpritesheet = nullptr;
	SDL_RendererFlip	flipX = SDL_FLIP_NONE;
	
};

#endif
