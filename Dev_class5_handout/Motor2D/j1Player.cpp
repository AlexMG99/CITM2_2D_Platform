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

	acceleration.x = 0.2F;
	acceleration.y = 1.0F;

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
	pugi::xml_node			coll_node = player_node.child("collider");

	if (result == NULL) {
		LOG("Error loading player XML! Error: %s", result.description());
		ret = false;
	}
	else
	{
		playerSpritesheet = App->tex->Load(player_node.child("image").attribute("source").value());

		if (playerSpritesheet == nullptr) {
			LOG("Error loading player texture!");
			ret = false;
		}
		else {
			idle_anim = LoadAnimations("idle");
			jump_anim = LoadAnimations("jump");
			run_anim = LoadAnimations("run");
			duckAnim = LoadAnimations("duck");
			deadAnim = LoadAnimations("dead");
			p2SString coll_name(coll_node.attribute("type").as_string());
			p2SString state_name(player_node.child("state").attribute("type").as_string());
			if (coll_name == "COLLIDER_PLAYER") {
				coll_type = COLLIDER_PLAYER;
			}
			if (state_name == "AIR_STATE")
			{
				state = AIR_STATE;
			}
			coll_rect = { coll_node.attribute("x").as_int() ,coll_node.attribute("y").as_int(),coll_node.attribute("w").as_int(),coll_node.attribute("h").as_int() };
			player_coll = App->collision->AddCollider({ coll_rect.x, coll_rect.y, coll_rect.w, coll_rect.h }, coll_type, App->player);
			position.x = player_node.child("position").attribute("x").as_float();
			position.y = player_node.child("position").attribute("y").as_float();
			velocity.x = player_node.child("velocity").attribute("x").as_float();
			velocity.y = player_node.child("velocity").attribute("y").as_float();
			maxVelocity.x = player_node.child("maxVelocity").attribute("x").as_float();
			maxVelocity.y = player_node.child("maxVelocity").attribute("y").as_float();
			jumpAcceleration = player_node.child("jump").child("acceleration").attribute("value").as_float();
			jumpMaxVelocity = player_node.child("jump").child("maxVelocity").attribute("value").as_float();
		}
	}
	return ret;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(state != DUCK_STATE)velocity.x = acceleration.x*maxVelocity.x + (1 - acceleration.x)*velocity.x;
		flipX = SDL_FLIP_NONE;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->player->position.y += 0.1F;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (state != DUCK_STATE)velocity.x = acceleration.x*-maxVelocity.x + (1 - acceleration.x)*velocity.x;
		flipX = SDL_FLIP_HORIZONTAL;
	}

	CheckState();

	return true;
}

bool j1Player::Update(float dt)
{
	player_coll->SetPos(position.x, position.y);
	CheckState();
	PerformActions();

	return true;
}
bool j1Player::PostUpdate()
{	
	position.x += velocity.x;
	position.y -= velocity.y;
	Draw();
	return true;
}

bool j1Player::CleanUp()
{
	
	return true;
}

bool j1Player::Load(pugi::xml_node& player_node) {

	position.x = player_node.child("position").attribute("x").as_float();
	position.y = player_node.child("position").attribute("y").as_float();
	velocity.x = player_node.child("velocity").attribute("x").as_float();
	velocity.y = player_node.child("velocity").attribute("y").as_float();
	p2SString state_name = player_node.child("state").attribute("value").as_string();
	if (state_name == "IDLE_STATE") 
	{
		state = IDLE_STATE;
	}
	else if (state_name == "RUN_STATE")
	{
		state = RUN_STATE;
	}
	else if (state_name == "JUMP_STATE")
	{
		state = JUMP_STATE;
	}
	else if (state_name == "AIR_STATE")
	{
		state = AIR_STATE;
	}
	else if (state_name == "DUCK_STATE")
	{
		state = DUCK_STATE;
	}
	else if (state_name == "DEAD_STATE")
	{
		state = DEAD_STATE;
	}
	
	return true;
}

bool j1Player::Save(pugi::xml_node& player_node) const
{
	pugi::xml_node pos = player_node.append_child("position");
	pugi::xml_node vel = player_node.append_child("velocity");
	pugi::xml_node state_node = player_node.append_child("state");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;
	vel.append_attribute("x") = velocity.x;
	vel.append_attribute("y") = velocity.y;
	p2SString state_name;
	if (state == 0)
	{
		state_name = "IDLE_STATE";
	}
	else if (state == 1)
	{
		state_name = "RUN_STATE";
	}
	else if (state == 2)
	{
		state_name = "JUMP_STATE";
	}
	else if (state == 3)
	{
		state_name = "AIR_STATE";
	}
	else if (state == 4)
	{
		state_name = "DUCK_STATE";
	}
	else if (state == 5)
	{
		state_name = "DEAD_STATE";
	}
	state_node.append_attribute("value") = state_name.GetString();

	return true;
}

void j1Player::Draw() {
	SDL_Rect rect = current_animation->GetCurrentFrame();
	App->render->Blit(playerSpritesheet, position.x, position.y, &rect, flipX);
}

p2Animation j1Player::LoadAnimations(p2SString name) {
	SDL_Rect frames;
	p2Animation anim;
	for (pugi::xml_node frames_node = player_file.child("player").child("animation").child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
	{
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();

		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
	}
	anim.speed = player_file.child("player").child("animation").child(name.GetString()).attribute("speed").as_float();

	return anim;

}

void j1Player::CheckState()
{
	bool pressed_right = App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT;
	bool pressed_left = App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT;
	bool pressed_down = App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT;
	bool released_right = App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP;
	bool released_left = App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP;
	bool released_down = App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP;
	bool press_space = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	
	switch (state)
	{
	case IDLE_STATE:
		if (pressed_right || pressed_left)
		{
			state = RUN_STATE;
		}

		if (press_space)
		{
			state = JUMP_STATE;
		}

		if (pressed_down) {
			state = DUCK_STATE;
		}
		break;

	case RUN_STATE:
		if (released_right || released_left)
		{
			state = IDLE_STATE;
		}

		if (press_space)
		{
			state = JUMP_STATE;
		}

		if (pressed_down)
		{
			state = DUCK_STATE;
		}
		
		break;

	case JUMP_STATE:
		if (jump_anim.Finished())
		{
			state = AIR_STATE;
			jump_anim.Reset();
		}
		break;

	case AIR_STATE:
		jump_anim.Reset();
		break;

	case DUCK_STATE:
		if (released_down)
		{
			state = IDLE_STATE;
		}
		break;
	case DEAD_STATE:
		break;
	}
}
void j1Player::PerformActions()
{
	switch (state) 
	{
	case IDLE_STATE:
		velocity.x = (1 - acceleration.x)*velocity.x;
		velocity.y = (1 - acceleration.y)*velocity.y;
		current_animation = &idle_anim;
		break;

	case RUN_STATE:
		velocity.y = (1 - acceleration.y)*velocity.y;
		current_animation = &run_anim;
		break;

	case JUMP_STATE:
		velocity.y = jumpAcceleration*jumpMaxVelocity + (1 - acceleration.y)*velocity.y;
		current_animation = &jump_anim;
		break;

	case AIR_STATE:
		velocity.y = acceleration.y*-maxVelocity.y + (1 - acceleration.y)*velocity.y;
		current_animation = &idle_anim;
		break;

	case DUCK_STATE:
		current_animation = &duckAnim;
		break;

	case DEAD_STATE:
		current_animation = &deadAnim;
		break;
	}
	
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{

	position.y--;
	state = IDLE_STATE;

}