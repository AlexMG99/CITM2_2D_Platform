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
	current_animation = &idle_anim;
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
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
	{
		velocity.x = acceleration.x*dt;
		flipX = SDL_FLIP_NONE;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
	{
		velocity.x = -acceleration.x*dt;
		flipX = SDL_FLIP_HORIZONTAL;
	}
	return true;
}

bool j1Entity_Player::Entity_Update(float dt)
{
	CheckState();
	PerformActions(dt);

	position.x += velocity.x;
	position.y += velocity.y;

	Entity_Draw(dt);

	coll->SetPos((int)(position.x - coll->rect.w / 2), (int)(position.y - coll->rect.h));

	if (state != STATE_JUMP) velocity.y = -acceleration.y*dt;

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
		break;

	default:
		LOG("There's no State!. Error!");
		break;

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

	default:
		LOG("Collider Unknown!");
		break;
	}
}