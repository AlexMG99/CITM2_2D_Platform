
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Bat.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Entity.h"
#include "j1Render.h"


j1Entity_Bat::j1Entity_Bat():j1Module()
{
	name.create("entity_bat");

}

//Destructor
j1Entity_Bat::~j1Entity_Bat()
{

}

bool j1Entity_Bat::Awake(pugi::xml_node& config)
{
	
	return true;
};

bool j1Entity_Bat::Start()
{
	
	return true;
}
bool j1Entity_Bat::Update(float dt)
{
	
	return true;
};

bool j1Entity_Bat::PostUpdate()
{

	return true;
};

bool j1Entity_Bat::CleanUp()
{
	
	return true;
}
bool j1Entity_Bat::Save(pugi::xml_node &)
{
	return true;
}
;

bool j1Entity_Bat::Load(pugi::xml_node& entity_node)
{

	
	return true;
}

