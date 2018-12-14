#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Button.h"



UI_Button::UI_Button(const char* text, Button_Type type, SDL_Rect idle_rect, SDL_Rect* rect_hover, SDL_Rect* rect_click, UI_GUI* parent)
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
	this->parent = parent;
}

bool UI_Button::Start()
{
	if (button_text.Length() > 0)
		App->gui->gui_list.add(App->gui->CreateLabel({ pos.x + 10, pos.y + 5 }, button_text.GetString(), BUTTON, {0,0,0,0}, static_object, this));
	return true;
}

bool UI_Button::PostUpdate()
{
	bool ret = true;

	switch (state)
	{
	case IDLE:

		ret = App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &button_rect[IDLE]); //(-1100) 
		break;

	case HOVER:
		ret = App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &button_rect[HOVER]);
		break;

	case CLICK:
		ret = App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &button_rect[CLICK]);
		break;
	}

	return ret;
}

bool UI_Button::OnHover()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	return pos.x < x && pos.y + App->render->camera.y < y && pos.x + button_rect[state].w > x && pos.y + button_rect[state].h > y;

}











