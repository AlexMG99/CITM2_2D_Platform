#include "j1App.h"
#include "p2Log.h"
#include "UI_Button.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Window.h"


UI_Button::UI_Button(SDL_Rect rect, const char* text)
{
	button_rect = rect;
	button_rect_hover = {2, 49, 178, 42};
	button_text.create(text);
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
		ret = App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y, &button_rect);
		break;

	case HOVER:
		ret = App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y, &button_rect_hover);
		break;
	}

	return ret;
}

bool UI_Button::OnHover()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	bool ret = pos.x + App->render->camera.x / (int)App->win->GetScale() < x && pos.y + App->render->camera.y < y && pos.x + App->render->camera.x / (int)App->win->GetScale() + button_rect.w > x && pos.y + button_rect.h > y;
	LOG("%i", ret);
	//LOG("x: %i y:%i", x, y);
	//LOG("x1: %i y2:%i", pos.x + App->render->camera.x/3, pos.y);
	return ret;

}

bool UI_Button::OnClick()
{
	return true;
}









