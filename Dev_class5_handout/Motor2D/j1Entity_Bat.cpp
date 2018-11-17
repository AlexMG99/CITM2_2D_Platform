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
	}
	else
	{
		FollowPath();
	}

	return true;
}

void j1Entity_Bat::FollowPath()
{
	
	int i = 0;
	iPoint bat_pos = App->map->WorldToMap(position.x, position.y);

	velocity.x = pathfinding_path[i + 1].x - pathfinding_path[i].x;
	velocity.y = pathfinding_path[i + 1].y + pathfinding_path[i].y;
	position.x += velocity.x;
	position.y -= velocity.y;


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
