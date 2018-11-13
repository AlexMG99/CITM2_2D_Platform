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
#include "j1Scene2.h"
#include "j1Collision.h"
#include "j1Render.h"

j1Entity::j1Entity() :j1Module()
{
	name.create("entity");

}


//Destructor
j1Entity::~j1Entity()
{
}

bool j1Entity::Awake(pugi::xml_node& config)
{
	LOG("Init SDL entity");
	bool ret = true;

	//path.create(config.child("path").child_value());
	//Fx of enemies
	return ret;
};

bool j1Entity::Start()
{
	//LOG("start entities");
	bool ret = true;

	pugi::xml_parse_result result = entities_file.load_file(path.GetString());
	pugi::xml_node         entity_node = entities_file.child("entities");

	if (result == NULL)
	{
		LOG("Error loading entities XML! Error: %s", result.description());
		ret = false;
	}
	else
	{
		entitiesSpritesheet = App->tex->Load(entity_node.child("image").attribute("source").value());
		if (entitiesSpritesheet == nullptr)
		{
			LOG("Error loadring entities texture!");
			ret = false;
		}
		else
		{
			/*crab_idle = LoadCrabAnimations("idle");
			crab_right = LoadCrabAnimations("right");
			crab_left = LoadCrabAnimations("left");

			bat_idle = LoadBatAnimations("idle");
			bat_right = LoadBatAnimations("right");
			bat_left = LoadBatAnimations("left");*/


			crab1_position = { App->map->data.entity_properties.Get("Crab1Position.x"), App->map->data.entity_properties.Get("Crab1Position.y") };
			bat1_position = { App->map->data.entity_properties.Get("Bat1Position.x"), App->map->data.entity_properties.Get("Bat1Position.y") };

			//crab_rect = { (int)App->map->data.entity_properties.Get("crabcoll.x"),(int)App->map->data.entity_properties.Get("crabcoll.y"),(int)App->map->data.entity_properties.Get("crabcoll.w"),(int)App->map->data.entity_properties.Get("crabcoll.h") };
			/*crab_coll = App->collision->AddCollider(crab_rect, COLLIDER_CRAB, App->entity_manager);*/
			//bat_rect = { (int)App->map->data.entity_properties.Get("crabcoll.x"),(int)App->map->data.entity_properties.Get("crabcoll.y"),(int)App->map->data.entity_properties.Get("crabcoll.w"),(int)App->map->data.entity_properties.Get("crabcoll.h") };
			/*bat_coll = App->collision->AddCollider(bat_rect, COLLIDER_BAT, App->entity);*/

		}


	}
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

bool j1Entity::CleanUp()
{
	if (entitiesSpritesheet != nullptr)
	{
		App->tex->UnLoad(entitiesSpritesheet);
		entitiesSpritesheet = nullptr;
	}
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

void j1Entity::SetAnimations(float dt)
{

	switch (state) 
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
	}
}

//virtual j1Entity::FollowPath()
//{
//}

/*void j1Entity::Draw(float dt) const  {

	SDL_Rect crab_rect = currentcrab_animation->GetCurrentFrame(dt);
	SDL_Rect bat_rect = currentbat_animation->GetCurrentFrame(dt);

	App->render->Blit(entitiesSpritesheet, (int)(crab1_position.x - crab_rect.w / 2), (int)(crab1_position.y - crab_rect.h), &crab_rect, flipX);
	App->render->Blit(entitiesSpritesheet, (int)(bat1_position.x), (int)(bat1_position.y), &bat_rect, flipX);
}*/

//p2Animation j1Entity::LoadCrabAnimations(p2SString name)const {
//	SDL_Rect frames;
//	p2Animation anim;
//	//for (pugi::xml_node frames_node = //entities_file.child("entities").child("animation").child("crab").child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
//	//{
//	//	frames.x = frames_node.attribute("x").as_int();
//	//	frames.y = frames_node.attribute("y").as_int();
//	//	frames.h = frames_node.attribute("h").as_int();
//	//	frames.w = frames_node.attribute("w").as_int();
//
//	//	anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
//	//}
//	////anim.speed = entities_file.child("entities").child("animation").child("crab").child(name.GetString()).attribute("speed").as_float();
//
//	return anim;
//}
//
//p2Animation j1Entity::LoadBatAnimations(p2SString name)const {
//	SDL_Rect frames;
//	p2Animation anim;
//	/*for (pugi::xml_node frames_node = entities_file.child("entities").child("animation").child("bat").child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
//	{
//		frames.x = frames_node.attribute("x").as_int();
//		frames.y = frames_node.attribute("y").as_int();
//		frames.h = frames_node.attribute("h").as_int();
//		frames.w = frames_node.attribute("w").as_int();
//
//		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
//	}
//	anim.speed = entities_file.child("entities").child("animation").child("bat").child(name.GetString()).attribute("speed").as_float();*/
//
//	return anim;
//}
