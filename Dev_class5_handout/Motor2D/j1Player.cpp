#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Scene2.h"
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

// Called before player is available
bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");
	bool ret = true;

	path.create(config.child("path").child_value());
	fx_death_name.create(config.child("fx_death").child_value());
	fx_jump_name.create(config.child("fx_jump").child_value());

	return ret;
}

bool j1Player::Start() 
{
	LOG("Start player:");
	bool ret = true;

	pugi::xml_parse_result	result = player_file.load_file(path.GetString());
	pugi::xml_node			player_node = player_file.child("player");

	if (result == NULL) {
		LOG("Error loading player XML! Error: %s", result.description());
		ret = false;
	}
	else
	{
		playerSpritesheet = App->tex->Load(player_node.child("image").attribute("source").value());

		if (playerSpritesheet == nullptr) 
		{
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

			coll_rect = { (int)App->map->data.player_properties.Get("coll.x"), (int)App->map->data.player_properties.Get("coll.y"), (int)App->map->data.player_properties.Get("coll.w") , (int)App->map->data.player_properties.Get("coll.h") };
			player_coll = App->collision->AddCollider(coll_rect , COLLIDER_PLAYER, App->player);
			fx_death = App->audio->LoadFx(fx_death_name.GetString());
			fx_jump = App->audio->LoadFx(fx_jump_name.GetString());

			position = { App->map->data.player_properties.Get("playerPosition.x"), App->map->data.player_properties.Get("playerPosition.y") };
			velocity = { App->map->data.player_properties.Get("velocity.x"), App->map->data.player_properties.Get("velocity.y") };
			maxVelocity = { App->map->data.player_properties.Get("maxVelocity.x"), App->map->data.player_properties.Get("maxVelocity.y") };
			jumpMaxVelocity = App->map->data.player_properties.Get("jumpMaxVelocity");
			jumpAcceleration = App->map->data.player_properties.Get("jumpAcceleration");
			acceleration = { App->map->data.player_properties.Get("gravity.x") , App->map->data.player_properties.Get("gravity.y") };
			falling = App->map->data.player_properties.Get("falling");

			state = AIR_STATE;

		}
	}
	return ret;
}

bool j1Player::PreUpdate(float dt)
{
	DebugInput();

	if (state != DUCK_STATE && state != CLING_STATE)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
		{
			velocity.x = (acceleration.x*maxVelocity.x + (10 - acceleration.x)*velocity.x) * dt;
			flipX = SDL_FLIP_NONE;
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
		{

			velocity.x = (acceleration.x*-maxVelocity.x + (10 - acceleration.x)*velocity.x) * dt;
			flipX = SDL_FLIP_HORIZONTAL;
		}
	}

	if(!godMode)CheckState();
	if(state !=JUMP_STATE && state != CLING_STATE)velocity.y = acceleration.y*-maxVelocity.y + (1 - acceleration.y)*velocity.y;

	return true;
}

bool j1Player::Update(float dt)
{
	if (!godMode)
	{
		position.x += velocity.x;
		position.y -= velocity.y;
		if (state != DUCK_STATE && state != RUN_STATE)player_coll->SetPos((int)(position.x - coll_rect.w / 2), (int)(position.y - coll_rect.h));
		else if (state == RUN_STATE) player_coll->SetPos((int)(position.x - coll_rect.w / 2), (int)(position.y - coll_rect.h));
		else { player_coll->SetPos((int)(position.x - coll_rect.w / 2), (int)(position.y - coll_rect.h / 2)); }
	}

	PerformActions(dt);

	return true;
}
bool j1Player::PostUpdate()
{	
	
	return true;
}

bool j1Player::CleanUp()
{
	if (playerSpritesheet != nullptr)
	{
		App->tex->UnLoad(playerSpritesheet);
		playerSpritesheet = nullptr;
	}
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
	coll_rect.x = player_node.child("collider").attribute("x").as_int();
	coll_rect.y = player_node.child("collider").attribute("y").as_int();
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
	else if (state_name == "CLING_STATE")
	{
		state = DEATH_STATE;
	}
	else if (state_name == "DEATH_STATE")
	{
		state = CLING_STATE;
	}
	else if (state_name == "GOD_STATE")
	{
		state = GOD_STATE;
	}
	return true;
}

bool j1Player::Save(pugi::xml_node& player_node) const
{
	pugi::xml_node pos = player_node.append_child("position");
	pugi::xml_node vel = player_node.append_child("velocity");
	pugi::xml_node state_node = player_node.append_child("state");
	pugi::xml_node coll = player_node.append_child("coll");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;
	vel.append_attribute("x") = velocity.x;
	vel.append_attribute("y") = velocity.y;
	coll.append_attribute("x") = coll_rect.x;
	coll.append_attribute("y") = coll_rect.y;
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
		state_name = "CLING_STATE";
	}
	else if (state == 6)
	{
		state_name = "DEATH_STATE";
	}
	else if (state == 7)
	{
		state_name = "GOD_STATE";
	}
	state_node.append_attribute("value") = state_name.GetString();

	return true;
}

void j1Player::Draw(float dt) const 
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt);
	App->render->Blit(playerSpritesheet, (int)(position.x - coll_rect.w/2), (int)(position.y - coll_rect.h), &rect, flipX);
}

p2Animation j1Player::LoadAnimations(p2SString name) const {
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
			App->audio->PlayFx(fx_jump);
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
		break;

	case AIR_STATE:
		jump_anim.Reset();
		if (falling && press_space)
		{
			state = JUMP_STATE;
			falling = false;
			App->audio->PlayFx(fx_jump);
		}
		break;

	case CLING_STATE:
		if (press_space)
		{
			jump_anim.Reset();
			state = JUMP_STATE;
			App->audio->PlayFx(fx_jump);
		}
		break;

	case DUCK_STATE:
		if (released_down)
		{
			state = IDLE_STATE;
		}
		break;

	case DEATH_STATE:
		velocity.x = 0;
		velocity.y = 0;
		break;

	case GOD_STATE:
		if (!godMode)state = AIR_STATE;
		break;
	default:
		break;
	}
}
void j1Player::PerformActions(float dt)
{
	switch (state) 
	{
	case IDLE_STATE:
		velocity.x = ((10 - acceleration.x)*velocity.x)*dt;
		velocity.y = ((5 - acceleration.y)*velocity.y) * dt;
		current_animation = &idle_anim;
		App->collision->ChangeSize(player_coll, 40, 20);
		break;

	case RUN_STATE:
		velocity.y = ((5 - acceleration.y)*velocity.y) * dt;
		current_animation = &run_anim;
		/*App->collision->ChangeSize(player_coll, 40, 30);*/
		
		break;

	case JUMP_STATE:
		velocity.y = (jumpAcceleration*jumpMaxVelocity + (1 - acceleration.y)*velocity.y) * dt;
		current_animation = &jump_anim;
		App->collision->ChangeSize(player_coll, 40, 20);
		break;

	case AIR_STATE:
		velocity.y = (acceleration.y*-maxVelocity.y + (5 - acceleration.y)*velocity.y) * dt;
		current_animation = &air_anim;
		App->collision->ChangeSize(player_coll, 40, 20);
		break;

	case CLING_STATE:
		velocity.x = 0;
		velocity.y = 0;
		current_animation = &cling_anim;
		App->collision->ChangeSize(player_coll, 40, 20);
		break;

	case DUCK_STATE:
		velocity.x = 0;
		velocity.y = 0;
		current_animation = &duck_anim;
		App->collision->ChangeSize(player_coll, 20, 20);
		break;

	case DEATH_STATE:
		current_animation = &dead_anim;
		
		if (App->scene->IsEnabled())
		{
			App->scene->Reset();
			App->audio->PlayFx(fx_death);
		}
		else if (App->scene2->IsEnabled())
		{
			App->scene2->Reset();
			App->audio->PlayFx(fx_death);
		}
		break;

	case GOD_STATE:
		player_coll->SetPos(-200, -200);
		current_animation = &idle_anim;
		velocity.y = acceleration.y*-maxVelocity.y + (1 - acceleration.y)*velocity.y;
		break;
	default:
		break;
	}
	
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	uint directionLeft = (position.x < c2->rect.x);
	uint directionRight = (c2->rect.x + c2->rect.w < position.x);
	uint directionUp = (position.y < c2->rect.y + c2->rect.h);
	uint directionCornerDown = (c2->rect.y + 5 < position.y);
	if (state != DEATH_STATE) {
		switch (c2->type)
		{
		case COLLIDER_GROUND:
			//Check if leaving the ground
			//Check collision from right
			if (directionRight && directionCornerDown) {
				position.x = (float)(c2->rect.x + c2->rect.w + c1->rect.w / 2);
				velocity.x = 0;
			}
			//Check collision from left
			else if (directionLeft && directionCornerDown)
			{
				position.x = (float)(c2->rect.x - c1->rect.w / 2);
				velocity.x = 0;
			}
			//Check collision from up
			else if (directionUp)
			{
				position.y = (float)c2->rect.y;
				if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE) || state == AIR_STATE)
				{
					state = IDLE_STATE;
				}
			}
			//Check collision from down
			else
			{
				velocity.y = 0;
				state = AIR_STATE;
				position.y = (float)(c2->rect.y + c2->rect.h + c1->rect.h);
			}
			break;

		case COLLIDER_WALL:
			if (c2->type == COLLIDER_WALL)
			{
				//Check collision from right
				if (directionRight) {
					state = CLING_STATE;
					flipX = SDL_FLIP_NONE;
					position.x = (float)(c2->rect.x + c2->rect.w + c1->rect.w / 2);
				}
				//Check collision from left
				else if (directionLeft)
				{
					state = CLING_STATE;
					flipX = SDL_FLIP_HORIZONTAL;
					position.x = (float)(c2->rect.x - c1->rect.w / 2);
				}
			}
			break;

		case COLLIDER_PLATFORM:
			//Check if leaving the ground
			//Check if it's jumping or falling
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				state = AIR_STATE;
				position.y--;
			}
			else {

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
				if (directionRight && directionCornerDown)
				{
					position.x += maxVelocity.x;
				}
				//Check collision form left
				else if (directionLeft && directionCornerDown)
				{
					position.x -= maxVelocity.x;
				}
				else {
					position.y = (float)c2->rect.y;
					jump_anim.Reset();
					if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) && (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE) || state == AIR_STATE)
					{
						state = IDLE_STATE;
					}
				}
			  }
			}
			break;

		case COLLIDER_DEATH:
			state = DEATH_STATE;
			break;

		case COLLIDER_WIN:
			if (App->scene->IsEnabled())
			{
				App->fadeToBlack->FadeToBlack(App->scene, App->scene2);
			}
			else if (App->scene2->IsEnabled())
			{
				App->fadeToBlack->FadeToBlack(App->scene2, App->scene);
			}
			break;
		default:
			break;
		}
	}
}

void j1Player::DebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (App->scene->IsEnabled())
		{
			App->scene->Reset();
		}
		else if (App->scene2->IsEnabled())
		{
			App->fadeToBlack->FadeToBlack(App->scene2, App->scene);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (App->scene->IsEnabled())
		{
			App->scene->Reset();
		}
		else if (App->scene2->IsEnabled())
		{
			App->scene2->Reset();
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->fadeToBlack->FadeToBlack(App->scene, App->scene2);
	}
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	if (godMode)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			position.x -= 5;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			position.y -= 5;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			position.y += 5;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			position.x += 5;
		}

		state = GOD_STATE;
	}
	
}