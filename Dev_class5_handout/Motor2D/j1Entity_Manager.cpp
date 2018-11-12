#include "p2Log.h"
#include "j1App.h"
#include "j1Entity_Manager.h"

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

	return ret;
}

bool j1Entity_Manager::Update(float dt)
{

	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Update(dt);
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
	case BAT:
		//entity = new j1Entity_Bat();
		break;
	case CRAB:
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

