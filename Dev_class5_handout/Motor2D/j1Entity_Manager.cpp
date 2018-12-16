#include "p2Log.h"
#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Bat.h"
#include "j1Entity_Crab.h"
#include "j1Entity_Player.h"
#include "j1Entity_Coin.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"

#include "Brofiler/Brofiler.h"

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

	fx_death_name.create(config.child("fx_death").child_value());
	fx_jump_name.create(config.child("fx_jump").child_value());
	fx_enemy_death_name.create(config.child("fx_enemy_death").child_value());
	fx_coin_name.create(config.child("fx_coin").child_value());

	return ret;
}

bool j1Entity_Manager::Start()
{
	LOG("Start Entity_Manager:");
	bool ret = true;

	debug_tex = App->tex->Load("textures/pathfinding.png");
	graphics = App->tex->Load("textures/entities.png");

	fx_death = App->audio->LoadFx(fx_death_name.GetString());
	fx_jump = App->audio->LoadFx(fx_jump_name.GetString());
	fx_enemy_death = App->audio->LoadFx(fx_enemy_death_name.GetString());
	fx_coin = App->audio->LoadFx(fx_coin_name.GetString());

	return ret;
}

bool j1Entity_Manager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_EntityManager", Profiler::Color::CornflowerBlue);
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Entity_PreUpdate(dt);
		if (entity->data->to_delete == true)
		{
			
			p2List_item<j1Entity*>* aux = entity->next;
			RELEASE(entity->data);
			entities.del(entity);
			entity = aux;
			if (aux == nullptr)
				break;
			if (entity->data->type == COIN) {
				App->audio->PlayFx(App->entity_manager->fx_coin);
			}
			else
			App->audio->PlayFx(App->entity_manager->fx_enemy_death);
			
		}
	}

	return true;
}

bool j1Entity_Manager::Update(float dt)
{
	BROFILER_CATEGORY("Update_EntityManager", Profiler::Color::DarkKhaki);

	
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Entity_Update(dt);
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

	if (graphics != nullptr)
	{
		App->tex->UnLoad(graphics);
		graphics = nullptr;
	}

	if (debug_tex != nullptr)
	{
		App->tex->UnLoad(debug_tex);
		debug_tex = nullptr;
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
		break;
	case COIN:
		entity = new j1Entity_Coin();
		break;
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

	pugi::xml_node coins = entities_doc.child("entityManager").child("coins");

	for (pugi::xml_node entity_coins = coins.child("coin"); entity_coins; entity_coins = entity_coins.next_sibling("coin"))
	{
		CreateEntity(COIN, entity_coins.child("position").attribute("x").as_int(), entity_coins.child("position").attribute("y").as_int());
	}

}
void j1Entity_Manager::LoadPlayer()
{
	player = CreateEntity(PLAYER, App->map->data.player_properties.Get("playerPosition.x"), App->map->data.player_properties.Get("playerPosition.y"));
	player->Entity_Start("player");
	
}

bool j1Entity_Manager::Load(pugi::xml_node & entity_node)
{
	p2List_item<j1Entity*>* entity;
	entity = entities.start;

	while (entity != NULL)
	{
		RELEASE(entity->data);
		entity = entity->next;
	}

	entities.clear();

	LoadPlayer();
	player->Load(entity_node.child("player"));
	
	for (pugi::xml_node crab = entity_node.child("enemies").child("entity_crab"); crab; crab = crab.next_sibling("entity_crab"))
	{
		CreateEntity(CRAB, crab.attribute("x").as_float(), crab.attribute("y").as_float());
	}

	for (pugi::xml_node bat = entity_node.child("enemies").child("entity_bat"); bat; bat = bat.next_sibling("entity_bat"))
	{
		(CreateEntity(BAT, bat.attribute("x").as_float(), bat.attribute("y").as_float()));
	}

	for (pugi::xml_node coin = entity_node.child("enemies").child("entity_coin"); coin; coin = coin.next_sibling("entity_coin"))
	{
		(CreateEntity(COIN, coin.attribute("x").as_float(), coin.attribute("y").as_float()));
	}

	return true;
}

bool j1Entity_Manager::Save(pugi::xml_node &entity_node) const
{
	player->Save(entity_node.append_child("player"));
	pugi::xml_node enemy_group = entity_node.append_child("enemies");
	for (p2List_item<j1Entity*>* entity = entities.start->next; entity; entity = entity->next)
	{
		pugi::xml_node save = enemy_group.append_child(entity->data->name.GetString());
		save.append_attribute("x") = entity->data->position.x;
		save.append_attribute("y") = entity->data->position.y;
	}
	return true;
}

void j1Entity_Manager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->coll == c1)
			entity->data->Entity_Collision(c2);
	}
}

