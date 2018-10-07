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
	pugi::xml_node           animation_node = player_node.child("animation");
	

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
		LoadPlayerAnimations(player_node.child("animation"), &idle);
		
			/*idle.PushBack({ 1,1,20,40 });
			idle.PushBack({ 22,1,20,40 });
			idle.PushBack({ 43,1,20,40 });
			idle.PushBack({ 64,1,20,40 });
			idle.speed;*/
		
		return ret;
	}
		
}

bool j1Player::LoadPlayerAnimations(pugi::xml_node& animation_node, p2Animation* animation) {
	SDL_Rect frames;
	for (pugi::xml_node frames_node = animation_node.child("idle").child("frame"); animation_node; animation_node = animation_node.next_sibling("frame"))
	{
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();

		/*idle.PushBack(Frames);*/
		animation->PushBack({ frames.x, frames.y, frames.w, frames.h });
	
	}
	animation->speed = 0.5f;
	

	return true;

}

bool j1Player::PreUpdate() 
{
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