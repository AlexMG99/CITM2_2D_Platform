#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Bat.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1Render.h"


j1Entity_Bat::j1Entity_Bat() : j1Entity("bat")
{
	name.create("entity_bat");

	coll = App->collision->AddCollider({ -200,-100, 30, 26 }, COLLIDER_ENEMY, App->entity_manager);

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
	state = STATE_IDLE;
	
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

bool j1Entity_Bat::Save(pugi::xml_node & entity_node)
{
	return true;
}
;

bool j1Entity_Bat::Load(pugi::xml_node& entity_node)
{

	
	return true;
}

void j1Entity_Bat::FollowPath()
{

}

void j1Entity_Bat::StandardPath()
{
    //Posició actual del entity
	iPoint current_position = App->map->WorldToMap(position.x, position.y);
	//Posició cel·la de la dreta
	iPoint right_cell(current_position.x - 1, current_position.y);
	//Posició cel·la de l'esquerra
	iPoint left_cell(current_position.x + 1, current_position.y);
}

void j1Entity_Bat::Entity_Collision(Collider* c2)
{
	switch (c2->type)
	{
	case COLLIDER_PLAYER:
		to_delete = true;
		break;
	}
}

