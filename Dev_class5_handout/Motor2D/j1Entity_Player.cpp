#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Player.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "p2Log.h"


j1Entity_Player::j1Entity_Player() :j1Entity("player")
{
	state = STATE_IDLE;
}

j1Entity_Player::~j1Entity_Player()
{
}

bool j1Entity_Player::Entity_Start(const char* entity_name)
{
	LOG("Start player:");

	cling_anim = LoadAnimation("cling", entity_name);
	jump_anim = LoadAnimation("jump", entity_name);
	duck_anim = LoadAnimation("duck", entity_name);

	player_tex = App->tex->Load("textures/player.png");

	acceleration = { App->map->data.player_properties.Get("acceleration.x"), App->map->data.player_properties.Get("acceleration.y")};

	coll = App->collision->AddCollider({ 0,0,20,40 }, COLLIDER_PLAYER, App->entity_manager);
	return true;
}

bool j1Entity_Player::Entity_PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && state != STATE_GOD)
	{
		velocity.x = acceleration.x*dt;
		if (state != STATE_CLING) flipX = SDL_FLIP_NONE;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE && state != STATE_GOD)
	{
		velocity.x = -acceleration.x*dt;
		if (state != STATE_CLING) flipX = SDL_FLIP_HORIZONTAL;
	}

	DebugInputs();

	return true;
}

bool j1Entity_Player::Entity_Update(float dt)
{
	CheckState();
	PerformActions(dt);

	position.x += velocity.x;
	position.y += velocity.y;

	Entity_Draw(dt);

	if(state != STATE_GOD) 
		coll->SetPos((int)(position.x - coll->rect.w / 2), (int)(position.y - coll->rect.h));

	if (state != STATE_JUMP && state != STATE_GOD) velocity.y = -acceleration.y*dt;

	if (godMode) state = STATE_GOD; 

	return true;
}

bool j1Entity_Player::Entity_Draw(float dt)
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt);
	App->render->Blit(player_tex, (int)(position.x - coll->rect.w / 2), (int)(position.y - coll->rect.h), &rect, flipX);
	return true;
}

void j1Entity_Player::CheckState()
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
	case STATE_IDLE:
		if (press_space)
		{
			state = STATE_JUMP;
		}

		if (pressed_left || pressed_right)
		{
			state = STATE_RUN;
		}

		if (pressed_down) {
			state = STATE_DUCK;
		}

		break;

	case STATE_RUN:
		if (released_left || released_right)
		{
			state = STATE_IDLE;
		}

		if (press_space)
		{
			state = STATE_JUMP;
			
		}

		if (pressed_down)
		{
			state = STATE_DUCK;
		}

		break;

	case STATE_JUMP:
		fall_anim.Reset();
		if (jump_anim.Finished())
		{
			state = STATE_FALL;
		}
		break;

	case STATE_FALL:
		jump_anim.Reset();
		if (falling && press_space)
		{
			state = STATE_JUMP;
			falling = false;
			//App->audio->PlayFx(fx_jump);
		}
		break;

		case STATE_CLING:
		if (press_space)
		{
			jump_anim.Reset();
			state = STATE_JUMP;
			//App->audio->PlayFx(fx_jump);
		}
		break;

	case STATE_DUCK:
		if (released_down)
		{
			state = STATE_IDLE;
		}
		break;

	case STATE_GOD:
		if (!godMode) state = STATE_FALL;
		break;

	default:
		break;
	}
}

void j1Entity_Player::PerformActions(float dt)
{
	switch (state)
	{
	case STATE_IDLE:
		velocity.x = 0;
		current_animation = &idle_anim;
		break;

	case STATE_RUN:
		current_animation = &run_anim;
		break;

	case STATE_JUMP:
		velocity.y = acceleration.y*dt;
		current_animation = &jump_anim;
		break;

	case STATE_FALL:
		velocity.y = -acceleration.y*dt;
		current_animation = &fall_anim;
		break;

	case STATE_CLING:
		velocity.x = 0;
		velocity.y = 0;
		current_animation = &cling_anim;
		break;

	case STATE_DUCK:
		velocity.x = 0;
		velocity.y = 0;
		current_animation = &duck_anim;
		break;

	case STATE_DEATH:
		velocity.x = 0;
		current_animation = &death_anim;

		if (App->scene->IsEnabled())
		{
			App->scene->Reset();
			//App->audio->PlayFx(fx_death);
		}
		else if (App->scene2->IsEnabled())
		{
			App->scene2->Reset();
			//App->audio->PlayFx(fx_death);
		}
		break;

	case STATE_GOD:
		current_animation = &idle_anim;
		coll->SetPos(0, 0);
		velocity = { 0,0 };

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			position.x -= 200* dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			position.y -= 200 * dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			position.y += 200 * dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			position.x += 200 * dt;
		}
		break;

	default:
		LOG("There's no State!. Error!");
		break;

	}
}

void j1Entity_Player::DebugInputs()
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

}

void j1Entity_Player::Entity_Collision(Collider* other_coll)
{
	uint directionLeft = (position.x < other_coll->rect.x);
	uint directionRight = (other_coll->rect.x + other_coll->rect.w < position.x);
	uint directionUp = (position.y < other_coll->rect.y + other_coll->rect.h);
	uint directionCornerDown = (other_coll->rect.y + 5 < position.y);

	switch (other_coll->type)
	{
	case COLLIDER_GROUND:
		//Check collision from right
		if (directionRight && directionCornerDown) {
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
				velocity.x = 0;
		}
		//Check collision from left
		else if (directionLeft && directionCornerDown)
		{
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
				velocity.x = 0;
		}
		//Check collision from up
		else if (directionUp)
		{
			position.y = (float)other_coll->rect.y + 1;
			velocity.y = 0;
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE || state == STATE_FALL)
				state = STATE_IDLE;
		}
		else
		{
			position.y = (float)(other_coll->rect.y + other_coll->rect.h + coll->rect.h);
			velocity.y = 0;
		}
		break;

	case COLLIDER_PLATFORM:
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) 
		{
			state = STATE_FALL;
			position.y--;
		}
		else
		{
			if (velocity.y > 0)
			{
				//Check collision from right
				if (directionRight && directionCornerDown) {
					if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
						velocity.x = 0;
				}
				//Check collision from left
				else if (directionLeft && directionCornerDown)
				{
					if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
						velocity.x = 0;
				}
			}
			if (directionUp)
			{
				position.y = (float)other_coll->rect.y + 1;
				velocity.y = 0;
				if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE || state == STATE_FALL)
					state = STATE_IDLE;
			}
		}
		
		break;

	case COLLIDER_WALL:
		//Check collision from right
		if (directionRight) {
			state = STATE_CLING;
			flipX = SDL_FLIP_NONE;
			position.x = (float)(other_coll->rect.x + other_coll->rect.w + coll->rect.w / 2);
		}
		//Check collision from left
		else if (directionLeft)
		{
			state = STATE_CLING;
			flipX = SDL_FLIP_HORIZONTAL;
			position.x = (float)(other_coll->rect.x - coll->rect.w / 2);
		}
		break;

	case COLLIDER_DEATH:
		state = STATE_DEATH;
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
	}
}

bool j1Entity_Player::Load(pugi::xml_node & player_node)
{
	position.x = player_node.child("position").attribute("x").as_float();
	position.y = player_node.child("position").attribute("y").as_float();
	velocity.x = player_node.child("velocity").attribute("x").as_float();
	velocity.y = player_node.child("velocity").attribute("y").as_float();
	p2SString state_name = player_node.child("state").attribute("value").as_string();
	if (state_name == "STATE_IDLE")
	{
		state = STATE_IDLE;
	}
	else if (state_name == "STATE_RUN")
	{
		state = STATE_RUN;
	}
	else if (state_name == "STATE_FALL")
	{
		state = STATE_FALL;
	}
	else if (state_name == "STATE_DEATH")
	{
		state = STATE_DEATH;
	}
	else if (state_name == "STATE_CLING")
	{
		state = STATE_CLING;
	}
	else if (state_name == "STATE_JUMP")
	{
		state = STATE_JUMP;
	}
	else if (state_name == "STATE_DUCK")
	{
		state = STATE_DUCK;
	}
	else if (state_name == "STATE_GOD")
	{
		state = STATE_GOD;
	}
	return true;
}

bool j1Entity_Player::Save(pugi::xml_node & player_node) const
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
		state_name = "STATE_IDLE";
	}
	else if (state == 1)
	{
		state_name = "STATE_RUN";
	}
	else if (state == 2)
	{
		state_name = "STATE_FALL";
	}
	else if (state == 3)
	{
		state_name = "STATE_DEATH";
	}
	else if (state == 4)
	{
		state_name = "STATE_CLING";
	}
	else if (state == 5)
	{
		state_name = "STATE_JUMP";
	}
	else if (state == 6)
	{
		state_name = "DUCK_STATE";
	}
	else if (state == 7)
	{
		state_name = "STATE_GOD";
	}
	state_node.append_attribute("value") = state_name.GetString();

	return true;
}
