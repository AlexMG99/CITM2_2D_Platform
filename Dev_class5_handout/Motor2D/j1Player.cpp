#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

//Destructor
j1Player::~j1Player()
{
}

// Called before player is available
bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");
	bool ret = true;

	return ret;
}

bool j1Player::Start() 
{
	LOG("start player");
	bool ret = true;
	return ret;
}

bool j1Player::CleanUp()
{
	return true;
}