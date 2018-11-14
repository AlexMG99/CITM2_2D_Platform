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
	//To eliminate Player
	/*j1Entity* DeletePlayer() const;*/

	//Delete Enemy
	bool DeleteEntity(j1Entity* entity);


	bool Load(pugi::xml_node& );
	bool Save(pugi::xml_node& )const;

private:
	p2SString				path;
	p2List<j1Entity*>		entities;
	j1Entity*                player;

	pugi::xml_node config;
	pugi::xml_document entities_file;


	SDL_Texture* entitiesSpritesheet = nullptr;
	SDL_Texture*			debug_tex = nullptr;
};


#endif //__ENTITY_MANAGER_H__
