#include "p2Log.h"
#include "j1App.h"
#include "j1Entity_Manager.h"

#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"

j1Entity_Manager::j1Entity_Manager() : j1Module()
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

	path.create(config.child("path").child_value());
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
		entity->data->Update(dt);
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

bool j1Entity_Manager::AddEnemy(Entity_Type type, int x, int y)
{
	j1Entity* entity;

	switch (type)
	{
	case ENTITY_BAT:
		//entity = new j1Entity_Bat();
		break;
	case ENTITY_CRAB:
		//entity = new j1Entity_Crab();
		break;
	default:
		LOG("Enemy Type Incorrect!");
		break;
	}
	/*entity.type = type;
	entity.position.x = x;
	entity.position.y = x;*/
	entities.add(entity);

	return true;
}

bool j1Entity_Manager::DeleteEnemy(j1Entity * entity)
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

