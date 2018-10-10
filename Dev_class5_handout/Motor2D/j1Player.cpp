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

	position.y = 0.0F;
	position.x = 0.0F;
	acceleration.y = 0.001F;
	acceleration.x = 0.02F;
	acceleration.y = 0.02F;
	maxVelocity.x = 0.3F;
	maxVelocity.y = 0.3F;

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
			//Load Animations & Colliders
			idle = LoadAnimations("idle");
			jump_anim = LoadAnimations("jump");
			run = LoadAnimations("run");
			duck = LoadAnimations("duck");
			player_coll = App->collision->AddCollider({ coll_node.attribute("x").as_int(),coll_node.attribute("y").as_int(),coll_node.attribute("w").as_int(),coll_node.attribute("h").as_int() }, COLLIDER_PLAYER);
		}
	}
	return ret;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		velocity.x = acceleration.x*maxVelocity.x + (1 - acceleration.x)*velocity.x;
		flipper = SDL_FLIP_NONE;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->player->position.y += 0.1F;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		velocity.x = acceleration.x*-maxVelocity.x + (1 - acceleration.x)*velocity.x;
		flipper = SDL_FLIP_HORIZONTAL;
	}

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
	SDL_Rect rect = current_animation->GetCurrentFrame();
	App->render->Blit(player_spritesheet, position.x, position.y, &rect, flipper);
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
		LOG("Animation: %s", name.GetString());
	}
	anim.speed = player_file.child("player").child("animation").child(name.GetString()).child("speed").attribute("value").as_float();

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
			state=RUN_STATE;
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
		if(jump_anim.Finished())
		{
			state = AIR_STATE;
		}
		break;

	case AIR_STATE:
		LOG("Hello");
		break;

	case DUCK_STATE:
		if (released_down) 
		{
			state = IDLE_STATE;
		}
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
		current_animation = &idle;
		break;

	case RUN_STATE:
		current_animation = &run;
		break;

	case JUMP_STATE:
		velocity.y = acceleration.y*maxVelocity.y + (1 - acceleration.y)*velocity.y;
		current_animation = &jump_anim;
		break;

	case AIR_STATE:
		velocity.y = acceleration.y*-maxVelocity.y + (1 - acceleration.y)*velocity.y;
		current_animation = &idle;
		break;

	case DUCK_STATE:
		current_animation = &duck;
		break;
	}
	
}