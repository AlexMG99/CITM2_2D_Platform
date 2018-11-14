#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Crab.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1Render.h"


j1Entity_Crab::j1Entity_Crab() : j1Entity("crab")
{
	name.create("entity_crab");

	coll = App->collision->AddCollider({ (int)position.x, (int)position.y, 28, 28 }, COLLIDER_ENEMY);
	idle_anim.PushBack({ 1, 1, 28, 28 });
	idle_anim.PushBack({ 30, 1, 28, 28 });
	idle_anim.PushBack({ 59, 1, 28, 28 });
	idle_anim.PushBack({ 88, 1, 28, 28 });
	idle_anim.speed = 10.0F;
	current_animation = &idle_anim;

}

//Destructor
j1Entity_Crab::~j1Entity_Crab()
{

}


bool j1Entity_Crab::Entity_Update(float dt)
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

void j1Entity_Crab::FollowPath()
{

}

