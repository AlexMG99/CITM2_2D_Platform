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

#include "Brofiler/Brofiler.h"


j1Entity_Bat::j1Entity_Bat() : j1Entity("bat")
{
	name.create("entity_bat");

	coll = App->collision->AddCollider({ -200,-100, 31, 26 }, COLLIDER_ENEMY, App->entity_manager);

	acceleration = { App->map->data.player_properties.Get("enemyAcceleration"), App->map->data.player_properties.Get("enemyAcceleration") };
	
	fly = true;

}

//Destructor
j1Entity_Bat::~j1Entity_Bat()
{
	
}

bool j1Entity_Bat::Entity_PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_EntityBat", Profiler::Color::CornflowerBlue);

	if (!CalculatePath())
	{
		if (!counting)
		{
			do_standard_path.Start();
			counting = true;
		}
		if (do_standard_path.ReadMs() > 1000)
		{
			StandardPath(dt);
		}
		velocity.x = 0;
		velocity.y = 0;
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

	return true;
}

void j1Entity_Bat::FollowPath(float dt)
{

  BROFILER_CATEGORY("Crab_FollowPath", Profiler::Color::Turquoise);

	velocity.x = (pathfinding_path[1].x - pathfinding_path[0].x) * acceleration.x;
	velocity.y = (pathfinding_path[1].y - pathfinding_path[0].y) * acceleration.y;
	
	

	position.x += velocity.x * dt;
	position.y += velocity.y * dt;

}

void j1Entity_Bat::StandardPath(float dt)
{
	
 /*BROFILER_CATEGORY("Crab_FStandardPath", Profiler::Color::LightSalmon);

	iPoint current_position = App->map->WorldToMap(position.x, position.y);

	iPoint right_cell(current_position.x + 1, current_position.y);
	iPoint left_cell(current_position.x - 1, current_position.y);

	iPoint up_cell(current_position.x, current_position.y - 1);
	iPoint right_up_cell(up_cell.x + 1, up_cell.y);
	iPoint left_up_cell(up_cell.x - 1, up_cell.y);

	iPoint down_cell(current_position.x, current_position.y + 1);
	iPoint right_down_cell(down_cell.x + 1, down_cell.y);
	iPoint left_down_cell(down_cell.x - 1, down_cell.y);

	pathfinding_path.PushBack(current_position);

	if (moving_right && App->pathfinding->IsWalkable(right_cell) && !App->pathfinding->IsWalkable(left_cell))

		pathfinding_path.PushBack(current_position);

	else if (!moving_left)
	{
		pathfinding_path.PushBack(current_position);
		moving_right = false;
		moving_left = true;
	}
	if (moving_left && App->pathfinding->IsWalkable(left_cell) && !App->pathfinding->IsWalkable(right_cell))
		pathfinding_path.PushBack(left_cell);

	else if (!moving_right) {
		pathfinding_path.PushBack(current_position);
		moving_right = true;
		moving_left = false;
	}

	iPoint path_pos(App->map->WorldToMap(pathfinding_path[1].x, pathfinding_path[1].y));
	if (pathfinding_path.Count() > 1)
		velocity.x = (pathfinding_path[1].x - pathfinding_path[0].x)* acceleration.x;
	if (moving_left)
		position.x += -velocity.x * dt;

	if (moving_right)
		position.x -= velocity.x*dt;*/

}
