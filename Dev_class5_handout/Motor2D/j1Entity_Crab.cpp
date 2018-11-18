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

#include "Brofiler/Brofiler.h"



j1Entity_Crab::j1Entity_Crab() : j1Entity("crab")
{
	name.create("entity_crab");

	coll = App->collision->AddCollider({ -100, -100, 28, 28 }, COLLIDER_ENEMY, App->entity_manager);
	
	acceleration = { App->map->data.player_properties.Get("enemyAcceleration"), App->map->data.player_properties.Get("enemyAcceleration") };

	fly = false;
}

//Destructor
j1Entity_Crab::~j1Entity_Crab()
{

}

bool j1Entity_Crab::Entity_PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_EntityCrab", Profiler::Color::CornflowerBlue);

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
		counting = false;
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
	BROFILER_CATEGORY("Bat_FollowPath", Profiler::Color::Turquoise);

	if (pathfinding_path.Count() > 1)
	{
		velocity.x = (pathfinding_path[1].x - pathfinding_path[0].x) * acceleration.x;
	}

}


void j1Entity_Crab::StandardPath()
{

	/*BROFILER_CATEGORY("Bat_StandardPath", Profiler::Color::LightSalmon);*/
	/*iPoint current_pos = App->map->WorldToMap(position.x, position.y);
	iPoint right_cell(current_pos.x - 1, current_pos.y);
	iPoint left_cell(current_pos.x + 1, current_pos.y);

	pathfinding_path.PushBack(current_pos);

	if (moving_right && App->pathfinding->IsWalkable(right_cell))
		pathfinding_path.PushBack(right_cell);*/
	
}

