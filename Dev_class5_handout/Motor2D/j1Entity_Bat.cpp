#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Bat.h"
#include "j1Textures.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1Render.h"


j1Entity_Bat::j1Entity_Bat() : j1Entity("bat")
{
	name.create("entity_bat");

	coll = App->collision->AddCollider({ -200,-100, 30, 26 }, COLLIDER_ENEMY, App->entity_manager);

	fly = true;

}

//Destructor
j1Entity_Bat::~j1Entity_Bat()
{
	
}

bool j1Entity_Bat::Entity_PreUpdate(float dt)
{
	if (!CalculatePath())
	{
		if (!counting)
		{
			do_standard_path.Start();
			counting = true;
		}
		if (do_standard_path.ReadMs() > 1000)
		{
			StandardPath();
		}
		velocity.x = 0;
	}
	else
	{
		FollowPath(dt);
	}

	return true;
}

void j1Entity_Bat::FollowPath(float dt)
{
	iPoint bat_pos = App->map->WorldToMap(position.x, position.y);

	if (pathfinding_path.Count() > 1)
	{
		velocity.x = (pathfinding_path[1].x - pathfinding_path[0].x) * 70;
		velocity.y = (pathfinding_path[1].y - pathfinding_path[0].y) * 70;
	}

	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
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
