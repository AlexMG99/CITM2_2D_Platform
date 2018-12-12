#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "UI_Slider.h"
#include "UI_Button.h"

UI_Slider::UI_Slider(SDL_Rect rect, Slider_type type)
{
	slider_rect = { 593,182,24,24 };
	slider_hover = { 593,182,24,24 };
	slider_click = { 619,182,24,24 };

	slider_type = type;
	sprite_rect = rect;
	
}

bool UI_Slider::Start()
{
	App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y, &sprite_rect);
	return true;
}

bool UI_Slider::PostUpdate()
{
	bool ret = false;

	ret = App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y, &slider_rect);


	return ret;
}

//void UI_Slider::OnClick()
//{
//}
