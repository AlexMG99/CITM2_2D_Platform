#include "j1App.h"
#include "j1Entity_Manager.h"
#include "j1Entity_Coin.h"
#include "j1Map.h"
#include "j1Collision.h"

j1Entity_Coin::j1Entity_Coin() : j1Entity("coin")
{
	name.create("entity_crab");

	coll = App->collision->AddCollider({ 30, 30, 28, 28 }, COLLIDER_COIN, App->entity_manager);

	fly = true;

}

//Destructor
j1Entity_Coin::~j1Entity_Coin()
{

}

bool j1Entity_Coin::Entity_Update(float dt)
{
	bool ret = true;

	coll->SetPos(position.x, position.y);
	current_animation = &idle_anim;
	ret = Entity_Draw(dt);

	return ret;
}
