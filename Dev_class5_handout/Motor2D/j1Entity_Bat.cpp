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
	CalculatePath();
}

bool j1Entity_Bat::Entity_PreUpdate(float dt)
{
	return true;
}

void j1Entity_Bat::FollowPath()
{
	
	int i = 0;
	bat_pos = App->map->WorldToMap(position.x, position.y);
	/*bat_pos = App->map->MapToWorld(bat_pos.x, bat_pos.y);*/

	/*if (bat_pos == bat_path[i + 1] && i<=bat_path.Count())
	{
		
		bat_speed.x = bat_path[i + 1].x - bat_path[i].x;
		bat_speed.y = bat_path[i + 1].y + bat_path[i].y;
		bat_pos.x += bat_speed.x;
		bat_pos.y += bat_speed.y;
		
	}*/

	/*const p2DynArray<iPoint>* bat_path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < bat_path->Count(); ++i)
	{
		bat_pos = App->map->MapToWorld(bat_path->At(i)->x, bat_path->At(i)->y);
	}*/
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
