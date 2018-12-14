#ifndef _SCENE_PAUSE__H
#define _SCENE_PAUSE_H

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2List.h"

class j1Scene_UI : public j1Module
{
public:

	j1Scene_UI();

	// Destructor
	virtual ~j1Scene_UI();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Reset scene
	void Reset() const;

	//Load Game State
	bool Load(pugi::xml_node&);

private:
	p2SString			path;
	p2List<j1Module*>	current_level;
	j1PerfTimer			ptimer;

	uint player_score;
	c
};

#endif // !_SCENE_PAUSE__H
