#include "p2Log.h"
#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Bat.h"
#include "j1Entity_Crab.h"
#include "j1Entity_Player.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"

j1Entity_Manager::j1Entity_Manager()
{
	name.create("entityManager");

}

j1Entity_Manager::~j1Entity_Manager()
{
}

bool j1Entity_Manager::Awake(pugi::xml_node& config)
{
	LOG("Init Entity_Manager:");
	bool ret = true;

	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Awake(config.child(entity->data->name.GetString()));
	}

	return ret;
}

bool j1Entity_Manager::Start()
{
	LOG("Start player:");
	bool ret = true;

	debug_tex = App->tex->Load("textures/pathfinding.png");
	graphics = App->tex->Load("textures/player.png");

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);
	player = CreateEntity(PLAYER, App->map->data.player_properties.Get("playerPosition.x"), App->map->data.player_properties.Get("playerPosition.y"));
	player->Start();

	//CreateEntity(BAT, 100, 100);
	//CreateEntity(CRAB, 200, 130);
	
	return ret;
}

bool j1Entity_Manager::PreUpdate(float dt)
{
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

bool j1Entity_Manager::Update(float dt)
{
	
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Entity_Update(dt);
	}

	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* dyn_path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < dyn_path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(dyn_path->At(i)->x, dyn_path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	return true;
}

bool j1Entity_Manager::CleanUp()
{

	p2List_item<j1Entity*>* entity;
	entity = entities.start;

	while (entity != NULL)
	{
		RELEASE(entity->data);
		entity = entity->next;
	}

	entities.clear();

	return true;
}

j1Entity* j1Entity_Manager::CreateEntity(Entity_Type type, int x, int y)
{
	j1Entity* entity=nullptr;

	switch (type)
	{
	case BAT:
		entity = new j1Entity_Bat();
		break;
	case CRAB:
		entity = new j1Entity_Crab();
		break;
	case PLAYER:
		entity = new j1Entity_Player();
	default:
		LOG("Enemy Type Incorrect!");
		break;
	}

	entity->type = type;
	entity->position.x = x;
	entity->position.y = y;
	
	entities.add(entity);

	return entity;
}

void j1Entity_Manager::LoadEnemies(pugi::xml_document& entities_doc)
{
	pugi::xml_node bats = entities_doc.child("entityManager").child("bats");

	for (pugi::xml_node entity_bat = bats.child("bat"); entity_bat; entity_bat = entity_bat.next_sibling("bat"))
	{
		CreateEntity(BAT, entity_bat.child("position").attribute("x").as_int(), entity_bat.child("position").attribute("y").as_int());
	}

	pugi::xml_node crabs = entities_doc.child("entityManager").child("crabs");

	for (pugi::xml_node entity_crabs = crabs.child("crab"); entity_crabs; entity_crabs = entity_crabs.next_sibling("crab"))
	{
		CreateEntity(CRAB, entity_crabs.child("position").attribute("x").as_int(), entity_crabs.child("position").attribute("y").as_int());
	}

}
//j1Entity * j1Entity_Manager::GetPlayer() const
//{
//	j1Entity* ret = nullptr;
//
//	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity->next) 
//	{
//		if (entity->data->type == PLAYER)
//		{
//			ret = entity->data;
//			break;
//		}
//	}
//	return ret;
//}

bool j1Entity_Manager::DeleteEntity(j1Entity * entity)
{
	p2List_item<j1Entity*>* entity_item = entities.start;
	while (entity_item != NULL)
	{
		if (entity_item->data == entity)
		{

			RELEASE(entity);
		}
		entity_item = entity_item->next;
	}
	return true;
}

bool j1Entity_Manager::Load(pugi::xml_node & entity_node)
{
	CleanUp();
	/*GetPlayer()->Load(entity_node.child("player"));*/

	for (pugi::xml_node crab = entity_node.child("crabs").child("crab"); crab; crab = crab.next_sibling("crab"))
	{
		CreateEntity(CRAB, entity_node.child("position").attribute("x").as_float(), entity_node.child("position").attribute("y").as_float());
	}

	for (pugi::xml_node bat = entity_node.child("bats").child("bat"); bat; bat = bat.next_sibling("bat"))
	{
		(CreateEntity(BAT, entity_node.child("position").attribute("x").as_float(), entity_node.child("position").attribute("y").as_float()));
		
	}

	return true;
}

bool j1Entity_Manager::Save(pugi::xml_node &entity_node) const
{
	/*GetPlayer()->Save(entity_node.append_child("player"));*/
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		pugi::xml_node save = entity_node.append_child(entity->data->name.GetString());
		save.append_attribute("position_x") = entity->data->position.x;
		save.append_attribute("position_y") = entity->data->position.y;
	}
	return false;
}

