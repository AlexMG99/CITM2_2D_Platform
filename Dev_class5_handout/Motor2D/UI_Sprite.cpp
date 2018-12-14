#include "j1App.h"
#include "p2Log.h"
#include "UI_Sprite.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"


UI_Sprite::UI_Sprite(SDL_Rect rect, UI_GUI* parent)
{
	sprite_rect = rect;
	this->parent = parent;
}


bool UI_Sprite::PostUpdate()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &sprite_rect);
	return true;
}
