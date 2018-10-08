#ifndef __J1PLAYER_H_
#define __J1PLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct SDL_Rect;

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

private:
	//Draw player in screen
	void Draw();
	//Load Player Animations
	p2Animation LoadAnimations(pugi::xml_node&, p2SString name);
	//Gravity Effect
	void AddForce(float gravity);

public:
	fPoint position;
	
	
private:
	p2SString path;
	fPoint velocity;
	fPoint acceleration;
	bool grounded = false;
	bool aired = false;

	pugi::xml_document  player_file;

	SDL_Texture*		player_spritesheet = nullptr;
	SDL_RendererFlip	flipper = SDL_FLIP_NONE;
	p2Animation*        current_animation = nullptr;
	p2Animation			idle;
	p2Animation			jump_anim;
	Collider*			player_coll = nullptr;
};

#endif
