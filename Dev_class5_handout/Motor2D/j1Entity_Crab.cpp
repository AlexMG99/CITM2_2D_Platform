#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Crab.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1Render.h"


j1Entity_Crab::j1Entity_Crab() :j1Entity()
{
	name.create("entity_crab");

}

//Destructor
j1Entity_Crab::~j1Entity_Crab()
{

}

bool j1Entity_Crab::Awake(pugi::xml_node& config)
{
	
	return true;
};


bool j1Entity_Crab::Start()
{
	

	return true;
}

bool j1Entity_Crab::Update(float dt)
{

	return true;
};

bool j1Entity_Crab::PostUpdate()
{

	return true;
};

bool j1Entity_Crab::CleanUp()
{
	

	return true;
}
bool j1Entity_Crab::Save(pugi::xml_node &)
{
	return true;
}
;

bool j1Entity_Crab::Load(pugi::xml_node& entity_node)
{

	
	return true;
}

