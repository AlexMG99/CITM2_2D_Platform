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

	bool PreUpdate();

	//// Called each loop iteration
	//bool Update(float dt);

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
	bool Draw();
	//Load Player Texture
	//bool LoadPlayerImage(SDL_Texture*);

public:
	fPoint position;

private:
	p2SString path;

	pugi::xml_document  player_file;
	SDL_Texture*		player_spritesheet;
	p2Animation*        current_animation;
	p2Animation			idle;
};



#endif
