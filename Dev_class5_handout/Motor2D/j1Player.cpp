#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Input.h"
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
	pugi::xml_node          animation_node = player_node.child("animation");
	

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
			idle = LoadAnimations(animation_node, "idle");
			jump_anim = LoadAnimations(animation_node, "jump");
		}
	}
	player_coll = App->collision->AddCollider({ 0,0,20,40 }, COLLIDER_PLAYER);
	return ret;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->player->position.x += 0.1F;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->player->position.y += 0.1F;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->player->position.x -= 0.1F;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->player->position.y -= 0.1F;
	return true;
}
bool j1Player::PostUpdate()
{
	Draw();
	player_coll->SetPos(position.x, position.y);
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

void j1Player::Draw() {
	current_animation = &idle;
	SDL_Rect rect = current_animation->GetCurrentFrame();
	App->render->Blit(player_spritesheet, position.x, position.y, &rect);
}

p2Animation j1Player::LoadAnimations(pugi::xml_node& anim_node, p2SString name) {
	SDL_Rect frames;
	p2Animation anim;
	for (pugi::xml_node frames_node = anim_node.child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
	{
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();

		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
	}
	anim.speed = 0.007F;

	return anim;

}