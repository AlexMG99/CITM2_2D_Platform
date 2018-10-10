#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Render.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");

	position.y = 0.0F;
	position.x = 0.0F;
	acceleration.y = 0.001F;

}

//Destructor
j1Player::~j1Player()
{
}

// Called before player is available
bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");
	bool ret = true;

	path.create(config.child("path").child_value());

	return ret;
}

bool j1Player::Start() 
{
	LOG("start player");
	bool ret = true;

	pugi::xml_parse_result	result = player_file.load_file(path.GetString());
	pugi::xml_node			player_node = player_file.child("player");
	pugi::xml_node			coll_node = player_node.child("collider");
	pugi::xml_node          animation_node = player_node.child("animation");
	

	if (result == NULL) {
		LOG("Error loading player XML! Error: %s", result.description());
		ret = false;
	}
	else
	{
		player_spritesheet = App->tex->Load(player_node.child("image").attribute("source").value());

		if (player_spritesheet == nullptr) {
			LOG("Error loading player texture!");
			ret = false;
		}
		else {
			LOG("Loaded player texture succesfully");
			//Load Animations & Colliders
			idle = LoadAnimations(animation_node, "idle");
			jump_anim = LoadAnimations(animation_node, "jump");
			run = LoadAnimations(animation_node, "run");
			player_coll = App->collision->AddCollider({ coll_node.attribute("x").as_int(),coll_node.attribute("y").as_int(),coll_node.attribute("w").as_int(),coll_node.attribute("h").as_int() }, COLLIDER_PLAYER);
		}
	}
	return ret;
}

bool j1Player::PreUpdate()
{
	//OTHER METHOD
	/*player_coll->SetPos(position.x, position.y);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		
			state = Player_State::RUN_STATE;

			App->player->position.x += 0.1F;
			flipper = SDL_FLIP_NONE;
		
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		
			state = Player_State::RUN_STATE;

			App->player->position.x -= 0.1F;
			flipper = SDL_FLIP_HORIZONTAL;
		
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		state = Player_State::JUMP_STATE;
		aired = true;
	}
	else (state = Player_State::IDLE_STATE); {
		App->player->position.x = 50.0F;
	}
	position.y += velocity.y;*/
	return true;
}

bool j1Player::Update(float dt)
{
	//OTHER METHOD
	//SDL_Rect CurrentFrame;
	//if (state == Player_State::IDLE_STATE)
	//{
	//	CurrentFrame = idle.GetCurrentFrame();
	//}
	//if (state == Player_State::RUN_STATE)
	//{
	//	CurrentFrame = run.GetCurrentFrame();
	//}
	//if (state == Player_State::JUMP_STATE)
	//{
	//	CurrentFrame = jump_anim.GetCurrentFrame();
	//}
	//
	//
	//current_animation = &idle;
	//SDL_Rect rect = current_animation->GetCurrentFrame();
	//App->render->Blit(player_spritesheet, position.x, position.y, &CurrentFrame, flipper);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) 
	{
		
		App->player->position.x += 0.1F;
		
		flipper = SDL_FLIP_NONE;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->player->position.y += 0.1F;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) 
	{
		App->player->position.x -= 0.1F;
		flipper = SDL_FLIP_HORIZONTAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		aired = true;
	}
	
	return true;
}
bool j1Player::PostUpdate()

{	
	player_coll->SetPos(position.x, position.y);
	CheckState();
	PerformActions();
	Draw();
	
	
//if (grounded)
//	{
//		velocity.y = 0;
//	}
//	else
//	{
//		if (!aired) {
//			AddForce(acceleration.y);
//		}
//	}
//
//	if(aired)
//	{
//		velocity.y -= acceleration.y;
//		if (velocity.y < -0.25F) 
//		{
//			aired = false;
//		}
//		grounded = false;
//	}

	//position.y += velocity.y;
	return true;
}

bool j1Player::CleanUp()
{
	
	return true;
}

bool j1Player::Load(pugi::xml_node& player_node) {

	position.x = player_node.child("position").attribute("x").as_float();
	position.y = player_node.child("position").attribute("y").as_float();
	return true;
}

bool j1Player::Save(pugi::xml_node& player_node) const
{
	pugi::xml_node pos = player_node.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

void j1Player::Draw() {
	
	SDL_Rect rect = current_animation->GetCurrentFrame();
	App->render->Blit(player_spritesheet, position.x, position.y, &rect, flipper);
}

p2Animation j1Player::LoadAnimations(pugi::xml_node& anim_node, p2SString name) {
	SDL_Rect frames;
	p2Animation anim;
	for (pugi::xml_node frames_node = anim_node.child(name.GetString()).child("frame"); frames_node; frames_node = frames_node.next_sibling("frame"))
	{
		frames.x = frames_node.attribute("x").as_int();
		frames.y = frames_node.attribute("y").as_int();
		frames.h = frames_node.attribute("h").as_int();
		frames.w = frames_node.attribute("w").as_int();

		anim.PushBack({ frames.x, frames.y, frames.w, frames.h });
		LOG("Animation: %s", name.GetString());
	}
	anim.speed = 0.06F;

	return anim;

}

void j1Player::AddForce(float gravity) 
{

	velocity.y += acceleration.y;

}

void j1Player::CheckState()
{
	

	switch (state) {

	case IDLE_STATE:
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {

			state=RUN_STATE;
			
		}
		/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN){

			state = Player_State::JUMP_STATE;

		}*/
		
		break;

	case RUN_STATE:
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) {

			state = IDLE_STATE;

		}
		/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {

			state = Player_State::JUMP_STATE;

		}*/

		break;

	case JUMP_STATE:
		
		break;

	case DUCK_STATE:
		break;

	}


}
void j1Player::PerformActions()
{
	/*SDL_Rect CurrentFrame;*/

	switch (state) {

	case IDLE_STATE:
		current_animation = &idle;
		break;



	case RUN_STATE:
			current_animation = &run;
		
	
		
		break;

	case JUMP_STATE:
		/*if (aired)
		{
			velocity.y -= acceleration.y;
			if (velocity.y < -0.25F)
			{
				aired = false;
			}
			grounded = false;
		}
		position.y += velocity.y;
		current_animation = &jump_anim;*/
		break;

	case DUCK_STATE:
		break;



	}
	
}