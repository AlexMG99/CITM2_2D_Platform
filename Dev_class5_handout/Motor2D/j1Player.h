#ifndef __J1PLAYER_H_
#define __J1PLAYER_H_

#include "j1Module.h"

class j1Player : public j1Module
{
public:
	j1Player();

	//Destructor
	virtual ~j1Player();

	//Called before player is aviable
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	//Called before quitting
	bool CleanUp();
};



#endif
