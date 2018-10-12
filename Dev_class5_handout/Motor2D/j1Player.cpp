#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
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
			duck_anim = LoadAnimations("duck");
			dead_anim = LoadAnimations("dead");
			air_anim = LoadAnimations("air");
			cling_anim = LoadAnimations("cling");

			player_coll = App->collision->AddCollider({ coll_rect.x, coll_rect.y, coll_rect.w, coll_rect.h }, coll_type, App->player);
		}
	}
	return ret;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
	{
		if (state != (DUCK_STATE))velocity.x = acceleration.x*maxVelocity.x + (1 - acceleration.x)*velocity.x;
		flipX = SDL_FLIP_NONE;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
	{
		if (state != DUCK_STATE)velocity.x = acceleration.x*-maxVelocity.x + (1 - acceleration.x)*velocity.x;
		flipX = SDL_FLIP_HORIZONTAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		App->fadeToBlack->FadeToBlack(App->scene, App->scene);
	}

	CheckState();

	return true;
}

bool j1Player::Update(float dt)
{
	if(state!=DUCK_STATE)player_coll->SetPos(position.x - coll_rect.w / 2, position.y - coll_rect.h);
	else { player_coll->SetPos(position.x - coll_rect.w / 2, position.y - coll_rect.h / 2); }

	PerformActions();

	return true;
}
bool j1Player::PostUpdate()
{	
	position.x += velocity.x;
	if (!grounded) 
	{
		position.y -= velocity.y;
	}
	Draw();
	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(playerSpritesheet);
	if (player_coll != nullptr)
	{
		player_coll->to_delete = true;
	}
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
		state = DEATH_STATE;
	}
	else if (state_name == "CLING_STATE")
	{
		state = CLING_STATE;
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
	else if (state == 6)
	{
		state_name = "CLING_STATE";
	}
	state_node.append_attribute("value") = state_name.GetString();

	return true;
}

void j1Player::Draw() {
	SDL_Rect rect = current_animation->GetCurrentFrame();
	App->render->Blit(playerSpritesheet, position.x - coll_rect.w/2, position.y - coll_rect.h, &rect, flipX);
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
	bool press_letter = App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT;
	bool released_letter = App->input->GetKey(SDL_SCANCODE_C) == KEY_UP;
	
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
		air_anim.Reset();
		if (jump_anim.Finished())
		{
			state = AIR_STATE;
		}
		if (press_letter) {
			state = CLING_STATE;
		}
		break;

	case AIR_STATE:
		jump_anim.Reset();
		break;
	case CLING_STATE:
		if (released_letter) {
			state = AIR_STATE;
		}
		break;

	case DUCK_STATE:
		if (released_down)
		{
			state = IDLE_STATE;
		}
		break;
	case DEATH_STATE:
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
		App->collision->ChangeSize(player_coll, 40, 20);
		break;

	case RUN_STATE:
		velocity.y = (1 - acceleration.y)*velocity.y;
		current_animation = &run_anim;
		break;

	case JUMP_STATE:
		grounded = false;
		velocity.y = jumpAcceleration*jumpMaxVelocity + (1 - acceleration.y)*velocity.y;
		current_animation = &jump_anim;
		break;

	case AIR_STATE:
		grounded = false;
		velocity.y = acceleration.y*-maxVelocity.y + (1 - acceleration.y)*velocity.y;
		current_animation = &air_anim;
		break;

	case CLING_STATE:
		velocity.x = 0;
		velocity.y = 0;
		current_animation = &cling_anim;
		break;

	case DUCK_STATE:
		velocity.x = 0;
		velocity.y = 0;
		current_animation = &duck_anim;
		App->collision->ChangeSize(player_coll, 20, 20);
		break;

	case DEATH_STATE:
		current_animation = &dead_anim;
		App->fadeToBlack->FadeToBlack(App->scene, App->scene);
		break;
	}
	
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	switch (c2->type)
	{
	case COLLIDER_GROUND:
		//Check collision from right
		if (c2->rect.x + c2->rect.w < position.x) {
			position.x += maxVelocity.x;
		}
		//Check collision from left
		else if (position.x < c2->rect.x)
		{
			position.x -= maxVelocity.x;
		}
		//Check collision from up
		else if (position.y < c2->rect.y + c2->rect.h)
		{
			position.y = c2->rect.y;
			state = IDLE_STATE;
			grounded = true;
		}
		//Check collision from down
		else
		{
			velocity.y = 0;
			state = AIR_STATE;
			position.y = c2->rect.y + c2->rect.h + c1->rect.h;
		}
		break;

	//case COLLIDER_WALL:
	//	if (c2->type == COLLIDER_WALL)
	//	{
	//		if (position.x < c2->rect.x) {
	//			state = CLING_STATE;
	//			flipX = SDL_FLIP_HORIZONTAL;
	//		}
	//		else if (position.x > c2->rect.x) {
	//			state = CLING_STATE;
	//			flipX = SDL_FLIP_HORIZONTAL;
	//		}
	//	}
	//	break;

	case COLLIDER_PLATFORM:
		//Check if it's jumping or falling
		if (velocity.y > 0)
		{
			if (position.y < c2->rect.y)
			{
				jump_anim.Reset();
				state = IDLE_STATE;
			}
		}
		else
		{
			//Check collision form right
			if (position.x > c2->rect.x + c2->rect.w)
			{
				position.x += maxVelocity.x;
			}
			//Check collision form left
			else if (position.x < c2->rect.x)
			{
				position.x -= maxVelocity.x;
			}
			else {
				position.y = c2->rect.y;
				jump_anim.Reset();
				state = IDLE_STATE;
			}
		}
		break;
	case COLLIDER_DEATH:
		state = DEATH_STATE;
		break;
	}

}