#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Crab.h"
#include "j1Pathfinding.h"
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

bool j1Entity_Crab::Entity_PreUpdate(float dt)
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
		/*counting = false;*/
		FollowPath(dt);
	}

	if (App->entity_manager->GetPlayer()->position.x < position.x)
	{
		flipX = SDL_FLIP_NONE;
	}
	else
	{
		flipX = SDL_FLIP_HORIZONTAL;
	}

	position.x += velocity.x * dt;

	return true;
}

void j1Entity_Crab::FollowPath(float dt)
{
	/*iPoint crab_pos = App->map->WorldToMap(position.x, position.y);
	iPoint next_node = { crab_pos.x + 1, crab_pos.y };
	iPoint prev_node = { crab_pos.x - 1, crab_pos.y };*/

	if (pathfinding_path.Count() > 1)
	{
		velocity.x = (pathfinding_path[1].x - pathfinding_path[0].x) * 60;
	}
	else
	{
		velocity.x = 0;
	}

}


void j1Entity_Crab::StandardPath()
{
	/*iPoint current_pos = App->map->WorldToMap(position.x, position.y);
	iPoint right_cell(current_pos.x - 1, current_pos.y);
	iPoint left_cell(current_pos.x + 1, current_pos.y);

	pathfinding_path.PushBack(current_pos);

	if (moving_right && App->pathfinding->IsWalkable(right_cell))
		pathfinding_path.PushBack(right_cell);*/
	
}

