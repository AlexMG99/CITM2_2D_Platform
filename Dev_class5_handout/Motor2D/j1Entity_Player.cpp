#include "j1App.h"
#include "j1Entity_Player.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"


j1Entity_Player::j1Entity_Player() :j1Entity("player")
{
	state = STATE_IDLE;
	current_animation = &idle_anim;
}

j1Entity_Player::~j1Entity_Player()
{
}

bool j1Entity_Player::Start(const char* entity_name)
{
	LOG("Start player:");

	cling_anim = LoadAnimation("cling", entity_name);

	return true;
}