#ifndef __J1_FADETOBALCK__
#define __J1_FADETOBALCK__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool PostUpdate();
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 1.0f);
	bool IsFading() const;

	//Save Game State
	bool Save(pugi::xml_node&) const;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	j1Module* to_enable = nullptr;
	j1Module* to_disable = nullptr;

public:
	mutable int scene_id = 0;
};

#endif __J1_FADETOBALCK__
