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

	pugi::xml_parse_result	result = player_file.load_file(path.GetString());
	pugi::xml_node			player_node = player_file.child("player");
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

	return ret;
}


bool j1Player::PreUpdate() 
{
	idle.PushBack({ 1,1,20,40 });
	idle.PushBack({ 22,1,20,40 });
	idle.PushBack({ 43,1,20,40 });
	idle.PushBack({ 64,1,20,40 });
	return true;
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
	current_animation = &idle;
	SDL_Rect rect = current_animation->GetCurrentFrame();
	App->render->Blit(player_spritesheet, position.x, position.y, &rect);
	return true;
}