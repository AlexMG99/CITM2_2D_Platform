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
	Draw();
	return true;
}

void UI_Sprite::Draw() const
{
	App->render->Blit(App->gui->GetAtlas(), pos_x, pos_y, &sprite_rect);
}