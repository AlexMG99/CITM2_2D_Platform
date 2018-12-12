#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "UI_Button.h"



UI_Button::UI_Button(const char* text, Button_Type type, SDL_Rect idle_rect, SDL_Rect* rect_hover, SDL_Rect* rect_click)
{
	button_rect[IDLE] = idle_rect;

	if (rect_hover == NULL)
	{
		button_rect[HOVER] = idle_rect;

	}
	else
	{
		button_rect[HOVER] = *rect_hover;
	}

	if (rect_click == NULL)
	{
		button_rect[CLICK] = *rect_hover;
	}
	else
	{
		button_rect[CLICK] = *rect_click;
	}
	
	
	button_text.create(text);
	button_type = type;
}

bool UI_Button::Start()
{
	if (button_text.Length() > 0)
		App->gui->gui_list.add(App->gui->CreateLabel({ pos.x + 10, pos.y + 5 }, button_text.GetString(),BUTTON));
	return true;
}

bool UI_Button::PostUpdate()
{
	bool ret = false;
	switch (state)
	{
	case IDLE:
		ret = App->render->Blit(App->gui->GetAtlas(), pos.x - App->render->camera.x / (int)App->win->GetScale(), pos.y, &button_rect[IDLE]);
		break;

	case HOVER:
		ret = App->render->Blit(App->gui->GetAtlas(), pos.x - App->render->camera.x / (int)App->win->GetScale(), pos.y, &button_rect[HOVER]);
		break;

	case CLICK:
		ret = App->render->Blit(App->gui->GetAtlas(), pos.x - App->render->camera.x / (int)App->win->GetScale(), pos.y, &button_rect[CLICK]);
		break;
	}

	return ret;
}

bool UI_Button::OnHover()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	bool ret = pos.x < x && pos.y + App->render->camera.y < y && pos.x + button_rect[IDLE].w > x && pos.y + button_rect[IDLE].h > y;
	return ret;

}

bool UI_Button::OnClick()
{
	bool ret = false;

	if (OnHover())
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			ret = true;
		}
	}
	return ret;
}










