#include "j1App.h"
#include "UI_Sprite.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "j1Gui.h"


UI_Sprite::UI_Sprite(SDL_Rect rect)
{
	sprite_rect = rect;
}


bool UI_Sprite::PostUpdate()
{
	App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y, &sprite_rect);
	return true;
}
