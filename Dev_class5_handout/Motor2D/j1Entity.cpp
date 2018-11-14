#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Render.h"

j1Entity::j1Entity(const char* entity_name) 
{
	name.create("entity");

	idle_anim = LoadAnimation("idle", entity_name);
	run_anim = LoadAnimation("run", entity_name);
	fall_anim = LoadAnimation("fall", entity_name);
}


//Destructor
j1Entity::~j1Entity()
{
	if(graphics!=nullptr)
	{
		App->tex->UnLoad(graphics);
		graphics = nullptr;
	}
	
	if (coll != nullptr)
	{
		coll->to_delete = true;
		coll = nullptr;
	}
}

bool j1Entity::Awake(pugi::xml_node& config)
{
	LOG("Init SDL entity");
	bool ret = true;

	entity_node = config.child("entities");

	return ret;
};

bool j1Entity::Start()
{
	LOG("Start entities:");
	bool ret = true;

	return ret;
}

bool j1Entity::PreUpdate()
{
	/*if (crab1_state == ENTITY_LEFT)
	{
		flipX = SDL_FLIP_HORIZONTAL;
	}
	if (bat1_state == ENTITY_LEFT)
	{
		flipX = SDL_FLIP_HORIZONTAL;
	}*/
	return true;
};

bool j1Entity::Update(float dt)
{
	SetAnimations(dt);
	return true;
};

bool j1Entity::PostUpdate()
{
	
	return true;
};

bool j1Entity::Load(pugi::xml_node& entity_node)
{
	/*crab1_position.x = entity_node.child("position").attribute("x").as_float();
	crab1_position.y = entity_node.child("position").attribute("y").as_float();

	bat1_position.x = entity_node.child("position").attribute("x").as_float();
	bat1_position.y = entity_node.child("position").attribute("y").as_float();

	crab_rect.x = entity_node.child("collider").attribute("x").as_int();
	crab_rect.y = entity_node.child("collider").attribute("y").as_int();

	bat_rect.x = entity_node.child("collider").attribute("x").as_int();
	bat_rect.y = entity_node.child("collider").attribute("y").as_int();

	p2SString crab_state = entity_node.child("entity_state").attribute("value").as_string();

	if (crab_state == "ENTITY_STATE_IDLE")
	{
		crab1_state = ENTITY_STATE_IDLE;
	}
	else if (crab_state == "ENTITY_STATE_RIGHT")
	{
		crab1_state = ENTITY_STATE_RIGHT;
	}
	else if (crab_state == "ENTITY_STATE_LEFT")
	{
		crab1_state = ENTITY_STATE_LEFT;
	}

	p2SString bat_state = entity_node.child("entity_state").attribute("value").as_string();

	if (bat_state == "ENTITY_STATE_IDLE")
	{
		bat1_state = ENTITY_STATE_IDLE;
	}
	else if (bat_state == "ENTITY_STATE_RIGHT")
	{
		bat1_state = ENTITY_STATE_RIGHT;
	}
	else if (bat_state == "ENTITY_STATE_LEFT")
	{
		bat1_state = ENTITY_STATE_LEFT;
	}*/
	return true;
}

bool j1Entity::Save(pugi::xml_node & node) const
{
	return true;
}

bool j1Entity::CalculatePath() 
{

	return true;
}

void j1Entity::SetAnimations(float dt)
{

	/*switch (state) 
		{
	case ENTITY_STATE_IDLE:
		
		currentcrab_animation = &crab_idle;
		currentbat_animation = &bat_idle;

		break;
	case ENTITY_STATE_RIGHT:

		currentcrab_animation = &crab_right;
		currentbat_animation = &bat_right;

		break;

	case ENTITY_STATE_LEFT:
		currentcrab_animation = &crab_left;
		currentbat_animation = &bat_left;
	}*/
}

p2Animation j1Entity::LoadAnimation(p2SString name, p2SString entity_name) const
{
	SDL_Rect frames;
	p2Animation anim;
	for (pugi::xml_node frames_node = entity_node.child(entity_name.GetString()).child("animation").child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
	{
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();

		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
	}
	anim.speed = entity_node.child("player").child("animation").child(name.GetString()).attribute("speed").as_float();

	return anim;
}

//virtual j1Entity::FollowPath()
//{
//}

