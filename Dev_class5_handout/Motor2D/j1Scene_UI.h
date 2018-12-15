#ifndef _SCENE_PAUSE__H
#define _SCENE_PAUSE_H

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2List.h"
#include "UI_Label.h"

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

	//Load Game State
	bool Load(pugi::xml_node&);

private:
	p2SString			path;
	p2List<j1Module*>	current_level;
	j1PerfTimer			ptimer;

public:
	char		player_score_string[5] = "0";
	char		timer_string[20] = "0";
	uint		player_score = 0;
	UI_Label*	score_label = nullptr;
	UI_Label*	timer_label = nullptr;
};

#endif // !_SCENE_PAUSE__H
