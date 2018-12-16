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

	//Check Collision
	void Entity_Collision(Collider* other_coll);

	//Load Player
	bool Load(pugi::xml_node& player_node);

	//Save Player
	bool Save(pugi::xml_node& player_node) const;

private:
	bool CheckState();
	void PerformActions(float dt);
	void DebugInputs();

private:
	bool			godMode = false;
	bool			falling = false;
	SDL_Texture*	player_tex = nullptr;
	p2Animation		jump_anim;
	p2Animation     duck_anim;
	p2Animation     cling_anim;



};

#endif

