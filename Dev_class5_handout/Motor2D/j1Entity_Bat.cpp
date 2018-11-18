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

	coll = App->collision->AddCollider({ -200,-100, 30, 26 }, COLLIDER_ENEMY, App->entity_manager);

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

	return true;
}

void j1Entity_Bat::FollowPath(float dt)
{

  BROFILER_CATEGORY("Crab_FollowPath", Profiler::Color::Turquoise);

	if (pathfinding_path.Count() > 1)
	{
		velocity.x = (pathfinding_path[1].x - pathfinding_path[0].x) * acceleration.x;
		velocity.y = (pathfinding_path[1].y - pathfinding_path[0].y) * acceleration.y;
	}
	else
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	position.x += velocity.x * dt;
	position.y += velocity.y * dt;

}

void j1Entity_Bat::StandardPath()
{
	//BROFILER_CATEGORY("Crab_FStandardPath", Profiler::Color::LightSalmon);
 //   //Posició actual del entity
	//iPoint current_position = App->map->WorldToMap(position.x, position.y);
	////Posició cel·la de la dreta
	//iPoint right_cell(current_position.x - 1, current_position.y);
	////Posició cel·la de l'esquerra
	//iPoint left_cell(current_position.x + 1, current_position.y);
}
