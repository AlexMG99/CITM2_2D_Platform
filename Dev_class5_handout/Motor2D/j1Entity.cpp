#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Entity.h"
#include "j1Entity_Manager.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "j1Collision.h"
#include "j1Render.h"



j1Entity::j1Entity(const char* entity_name) 
{
	name.create("entity");

	entities_animations.load_file("entities.xml");

	velocity = { 0,0 };
	acceleration = { App->map->data.player_properties.Get("acceleration.x"), App->map->data.player_properties.Get("acceleration.y") };

	idle_anim = LoadAnimation("idle", entity_name);
	run_anim = LoadAnimation("run", entity_name);
	fall_anim = LoadAnimation("fall", entity_name);
	death_anim = LoadAnimation("death", entity_name);
    state = STATE_IDLE;
	
}


//Destructor
j1Entity::~j1Entity()
{
	if (coll != nullptr)
	{
		coll->to_delete = true;
		coll = nullptr;
	}
	
	entities_animations.reset();
}


bool j1Entity::Entity_Update(float dt)
{
	SetAnimations(dt);

	position.y += velocity.y;
	position.x += velocity.x;

	if(!fly) 
		velocity.y = -acceleration.y * dt;

	coll->SetPos(position.x, position.y);
	Entity_Draw(dt);
	return true;
};

bool j1Entity::Load(pugi::xml_node& entity_node)
{
	return true;
}

bool j1Entity::Save(pugi::xml_node & node) const
{
	return true;
}

bool j1Entity::CalculatePath()
{
	bool ret = false;
	iPoint origin = App->map->WorldToMap(position.x, position.y);
	iPoint player_position = App->map->WorldToMap(App->entity_manager->GetPlayer()->position.x, App->entity_manager->GetPlayer()->position.y - App->entity_manager->GetPlayer()->coll->rect.h);

	if (App->entity_manager->GetPlayer()->state != STATE_DEATH && position.DistanceTo(App->entity_manager->GetPlayer()->position) < RANG)
	{
		App->pathfinding->CreatePath(origin, player_position, type);
		const p2DynArray<iPoint>* entity_path = App->pathfinding->GetLastPath();
		for (int i = 0; i < entity_path->Count(); i++)
		{
			pathfinding_path.PushBack(*entity_path->At(i));
		}

		ret = true;
	}

	return ret;
}

void j1Entity::SetAnimations(float dt)
{
        switch (state)
		{
		case STATE_IDLE:
			current_animation = &idle_anim;
			break;

		case STATE_RUN:
			current_animation = &run_anim;
			break;

		case STATE_FALL:
			current_animation = &fall_anim;
			break;

		case STATE_DEATH:
			current_animation = &death_anim;
			if (death_anim.Finished())
			{
				to_delete = true;
			}
			break;
		default:
			break;
		}
}

p2Animation j1Entity::LoadAnimation(p2SString name, p2SString entity_name) const
{
	SDL_Rect frames;
	p2Animation anim;
	pugi::xml_node entity_node = entities_animations.child("entityManager").child("entities");
	for (pugi::xml_node frames_node = entity_node.child(entity_name.GetString()).child("animation").child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
	{
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();

		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
	}
	anim.speed = entity_node.child(entity_name.GetString()).child("animation").child(name.GetString()).attribute("speed").as_float();

	return anim;
}

bool j1Entity::Entity_Draw(float dt)
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt);
	App->render->Blit(App->entity_manager->graphics, (int)(position.x),(int)(position.y), &rect, flipX);
	return true;
}

void j1Entity::Entity_Collision(Collider* other_coll)
{
	switch (other_coll->type)
	{
	case COLLIDER_PLAYER:
		if (App->entity_manager->GetPlayer()->position.y - other_coll->rect.h / 2 < coll->rect.y)
		{
			state = STATE_DEATH;
			App->entity_manager->GetPlayer()->state = STATE_JUMP;
		}
		else 
		{
			App->entity_manager->GetPlayer()->state = STATE_DEATH;
		}
		break;
	case COLLIDER_GROUND:
		if (coll->rect.y < other_coll->rect.y)
		{
			position.y = (float)(other_coll->rect.y - coll->rect.h + 1);
			velocity.y = 0;
		}
		break;
	}
}


