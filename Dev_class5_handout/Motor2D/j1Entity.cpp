#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1FadeToBlack.h"
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

	path.create(config.child("path").child_value());
	//Fx of enemies
	return ret;
};

bool j1Entity::Start()
{
	LOG("start entities");
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
			crab1_idle = LoadAnimations("idle");
			crab1_right = LoadAnimations("right");
			crab1_left = LoadAnimations("left");

			entity_position = { App->map->data.entity_properties.Get("entityPosition.x"), App->map->data.entity_properties.Get("entityPosition.y") };
			
			state = ENTITY_LEFT;

		}
		
		
	}
	return ret;
}

bool j1Entity::PreUpdate()
{
	if (state == ENTITY_LEFT) 
	{
		flipX = SDL_FLIP_HORIZONTAL;
	}
	return true;
};

bool j1Entity::Update(float dt)
{
	PerformActions();
	return true;
};

bool j1Entity::PostUpdate() 
{
	Draw();
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
	entity_position.x = entity_node.child("position").attribute("x").as_float();
	entity_position.y = entity_node.child("position").attribute("y").as_float();

	p2SString entity_state = entity_node.child("entity_state").attribute("value").as_string();
	if (entity_state == "ENTITY_IDLE")
	{
		state = ENTITY_IDLE;
	}
	else if (entity_state == "ENTITY_RIGHT")
	{
		state = ENTITY_RIGHT;
	}
	else if (entity_state == "ENTITY_LEFT")
	{
		state = ENTITY_LEFT;
	}
	return true;
}

void j1Entity::Draw() {
	
	SDL_Rect entity_rect=current_animation->GetCurrentFrame();
	
	App->render->Blit(entitiesSpritesheet, (int)(entity_position.x), (int)(entity_position.y), &entity_rect, flipX);
}

p2Animation j1Entity::LoadAnimations(p2SString name)const {
	SDL_Rect frames;
	p2Animation anim;
	for (pugi::xml_node frames_node = entities_file.child("entities").child("animation").child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
	{
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();
	
		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
	}
	anim.speed = entities_file.child("entities").child("animation").child(name.GetString()).attribute("speed").as_float();

	return anim;
}

void j1Entity::PerformActions() {
	
	switch (state)
	{
	case ENTITY_IDLE:
		current_animation = &crab1_idle;
		break;
	case ENTITY_RIGHT:
		current_animation = &crab1_right;
		break;
	case ENTITY_LEFT:
		current_animation = &crab1_left;
		break;
	}
}