#ifndef _ENTITY_COIN_H
#define _ENTITY_COIN_H

#include "j1Entity.h"

class j1Entity_Coin : public j1Entity {

public:

	j1Entity_Coin();

	//Destructor
	virtual ~j1Entity_Coin();

	bool Entity_Update(float dt);

};


#endif // ! _ENTITY_COIN_H

