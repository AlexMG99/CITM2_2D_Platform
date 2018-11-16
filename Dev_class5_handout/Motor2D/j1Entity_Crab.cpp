#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Crab.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1Render.h"


j1Entity_Crab::j1Entity_Crab() : j1Entity("crab")
{
	name.create("entity_crab");

	coll = App->collision->AddCollider({ -100, -100, 28, 28 }, COLLIDER_ENEMY, App->entity_manager);
	
	

}

//Destructor
j1Entity_Crab::~j1Entity_Crab()
{

}


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

void j1Entity_Crab::StandardPath()
{
	//Posició actual del entity
	iPoint current_position = App->map->WorldToMap(position.x, position.y);
	//Posició cel·la de la dreta
	iPoint right_cell(current_position.x - 1, current_position.y);
	//Posició cel·la de l'esquerra
	iPoint left_cell(current_position.x + 1, current_position.y);

	
}

void j1Entity_Crab::Entity_Collision(Collider* c2)
{
	switch (c2->type)
	{
	case COLLIDER_PLAYER:
		to_delete = true;
		break;
	}
}
