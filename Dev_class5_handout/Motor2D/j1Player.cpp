#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Render.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");

}

//Destructor
j1Player::~j1Player()
{
}

// Called before render is available
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

	pugi::xml_parse_result	result = player_file.load_file(path.GetString());
	pugi::xml_node			player_node = player_file.child("player");
	pugi::xml_node			player_collider = player_node.child("collider");
	if (result == NULL) {
		LOG("Error loading player XML! Error: %s", result.description());
		ret = false;
	}
	else
	{
		player_spritesheet = App->tex->Load(player_node.child("image").attribute("source").value());

		if (player_spritesheet == nullptr) {
			LOG("Error loading player texture!");
			ret = false;
		}
		else {
			LOG("Loaded player texture succesfully");
		}
	}

	player_coll = App->collision->AddCollider({ player_collider.attribute("x").as_int(),
		player_collider.attribute("y").as_int(),
		player_collider.attribute("w").as_int(),
		player_collider.attribute("h").as_int() },
		COLLIDER_PLAYER);

	return ret;
}

bool j1Player::PostUpdate()
{
	Draw();
	player_coll->SetPos(position.x, position.y);
	//App->render->camera.x = -position.x;
	return true;
}

bool j1Player::CleanUp()
{
	if (player_coll != nullptr) {
		player_coll->to_delete = true;
	}
	return true;
}

bool j1Player::Load(pugi::xml_node& player_node) {

	position.x = player_node.child("position").attribute("x").as_float();
	position.y = player_node.child("position").attribute("y").as_float();
	return true;
}

bool j1Player::Save(pugi::xml_node& player_node) const
{
	pugi::xml_node pos = player_node.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

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