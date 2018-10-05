#ifndef __J1PLAYER_H_
#define __J1PLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"

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
	bool PostUpdate();

	//Called before quitting
	bool CleanUp();

	bool Draw();

private:
	//Load Player Texture
	//bool LoadPlayerImage(SDL_Texture*);

public:
	p2SString path;
	fPoint position;

private:
	SDL_Texture* player_spritesheet;
	Animation*	 current_animation;
	Animation	 idle;
};



#endif
