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

	//Called every loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//Add Enemy in the Map
	bool AddEnemy(Entity_Type type, int x, int y);

	//Delete Enemy
	bool DeleteEnemy(j1Entity* entity);

private:
	p2SString				path;
	p2List<j1Entity*>		entities;
};


#endif //__ENTITY_MANAGER_H__
