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
#include "j1Collision.h"
#include "j1Render.h"

j1Entity::j1Entity(const char* entity_name) 
{
	name.create("entity");

	entities_animations.load_file("entities.xml");

	idle_anim = LoadAnimation("idle", entity_name);
	run_anim = LoadAnimation("run", entity_name);
	fall_anim = LoadAnimation("fall", entity_name);

}


//Destructor
j1Entity::~j1Entity()
{
	if (coll != nullptr)
	{
		coll->to_delete = true;
		coll = nullptr;
	}
}

bool j1Entity::Entity_Update(float dt)
{
	//SetAnimations(dt);

	SDL_Rect rect = current_animation->GetCurrentFrame(dt);
	App->render->Blit(App->entity_manager->graphics, position.x, position.y, &rect, flipX);
	return true;
};

bool j1Entity::PostUpdate()
{
	
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

	


	return true;
}

void j1Entity::SetAnimations(float dt)
{

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


