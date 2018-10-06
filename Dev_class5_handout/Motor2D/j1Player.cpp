#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Render.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");

	position.y = 0;
	position.x = 0;

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

	path.create(config.child("path").child_value());

	return ret;
}

bool j1Player::Start() 
{
	LOG("start player");
	bool ret = true;

	player_spritesheet = App->tex->Load(path.GetString());
	if (player_spritesheet == nullptr) {
		LOG("Error loading");
		ret = false;
	}
	else {
		LOG("Loaded player texture succesfully");
	}
	return ret;
}

bool j1Player::PostUpdate()
{
	Draw();
	//App->render->camera.x = -position.x;
	return true;
}

bool j1Player::CleanUp()
{
	return true;
}

bool j1Player::Load(pugi::xml_node& player_node) {
	//position.x = player_node.child
	//position.y = data.child("camera").attribute("y").as_int();
	return true;
}


bool j1Player::Draw() {
	SDL_Rect rect;
	rect.x = 1;
	rect.y = 1;
	rect.w = 20;
	rect.h = 40;
	App->render->Blit(player_spritesheet, position.x, position.y, &rect);
	return true;
}