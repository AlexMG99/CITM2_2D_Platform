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
	pugi::xml_node entity_node = entities_file.child("entities");

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
		
	}
	return true;
}

bool j1Entity::PreUpdate()
{
	return true;
};

bool j1Entity::Update(float dt)
{
	return true;
};

bool j1Entity::PostUpdate() 
{
	/*Draw();*/
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

p2Animation j1Entity::LoadAnimations(p2SString name)const {



}
