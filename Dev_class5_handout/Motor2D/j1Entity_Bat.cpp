#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Entity_Bat.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Collision.h"
#include "j1Render.h"


j1Entity_Bat::j1Entity_Bat() :j1Entity("bat")
{
	name.create("entity_bat");

	graphics = App->tex->Load("textures/bat.png");
	coll = App->collision->AddCollider({ (int)position.x, (int)position.y, 30, 31 }, COLLIDER_ENEMY);

}

//Destructor
j1Entity_Bat::~j1Entity_Bat()
{

}

bool j1Entity_Bat::Awake(pugi::xml_node& config)
{
	
	return true;
};

bool j1Entity_Bat::Start()
{
	state = STATE_IDLE;
	
	return true;
}
bool j1Entity_Bat::Update(float dt)
{
	
	return true;
};

bool j1Entity_Bat::PostUpdate()
{

	return true;
};

bool j1Entity_Bat::CleanUp()
{
	
	return true;
}

void j1Entity_Bat::Draw(float dt)
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt);
	App->render->Blit(graphics, position.x, position.y, &rect, flipX);
}

bool j1Entity_Bat::Save(pugi::xml_node & entity_node)
{
	return true;
}
;

bool j1Entity_Bat::Load(pugi::xml_node& entity_node)
{

	
	return true;
}

void j1Entity_Bat::FollowPath()
{

}

