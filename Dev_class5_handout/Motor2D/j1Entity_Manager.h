#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1Entity.h"

class j1Entity_Manager : public j1Module 
{
public:
	j1Entity_Manager();

	// Destructor
	virtual ~j1Entity_Manager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool PreUpdate(float dt);

	//Called every loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//Add Enemy in the Map
	j1Entity* CreateEntity(Entity_Type type, int x, int y);

	//Load all enemies from XML
	void LoadEnemies(pugi::xml_document& entities_doc);
	void LoadPlayer();

	//Get Player
	j1Entity* GetPlayer() { return player; }

	bool Load(pugi::xml_node& );
	bool Save(pugi::xml_node& )const;

	//Check Collisions
	void OnCollision(Collider * c1, Collider * c2);

private:
	p2List<j1Entity*>		entities;
	j1Entity*               player;

	SDL_Texture*			debug_tex = nullptr;

public:
	SDL_Texture*			graphics = nullptr;
	SDL_Texture*			player_graphics = nullptr;

};


#endif //__ENTITY_MANAGER_H__
