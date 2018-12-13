#include "j1App.h"
#include "p2Log.h"
#include "UI_Sprite.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"


UI_Sprite::UI_Sprite(SDL_Rect rect)
{
	sprite_rect = rect;
}


bool UI_Sprite::PostUpdate()
{
	App->render->Blit(App->gui->GetAtlas(), (int)(pos.x - (-1100)/ (int)App->win->GetScale()), pos.y, &sprite_rect);
	return true;
}
