#ifndef __j2SCENE_H__
#define __j2SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene2 : public j1Module
{
public:

	j1Scene2();

	// Destructor
	virtual ~j1Scene2();

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

	//Load Level
	void LoadLevel();

	//Load
	bool Load(pugi::xml_node& node);

private:
	p2SString            path;
	p2SString            map_path;
	p2SString            music_path;

	pugi::xml_document   entities_files;
};

#endif // __j2SCENE_H__
