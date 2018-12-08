#ifndef __j1SCENE_INTRO_H__
#define __j1SCENE_INTRO_H__

#include "j1Module.h"

struct SDL_Texture;

class j1SceneMenu : public j1Module
{
public:

	j1SceneMenu();

	// Destructor
	virtual ~j1SceneMenu();

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

	//Load Level
	void LoadLevel();

private:
	p2SString			path;
	p2SString			map_path;
	p2List<j1Module*>	current_level;

};

#endif // __j1SCENE_INTRO_H__
