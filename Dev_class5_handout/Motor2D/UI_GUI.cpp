#include "UI_GUI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Window.h"


UI_GUI::~UI_GUI()
{
}

bool UI_GUI::PreUpdate(float dt)
{
	switch (state)
	{
	case UI_State::IDLE:
		if (OnHover())
		{
			state = UI_State::HOVER;
		}
		break;

	case UI_State::HOVER:
		if (!OnHover())
		{
			state = UI_State::IDLE;
		}
		if (OnClick())
		{
			state = UI_State::CLICK;
		}
		break;

	case UI_State::CLICK:
		if (!OnClick())
		{
			state = UI_State::HOVER;
		}
		break;

	default:
		break;
	}
	return true;
}

bool UI_GUI::Update(float dt)
{
	if (static_object)
		position = { pos.x - App->render->camera.x / (int)App->win->GetScale(), pos.y - App->render->camera.y / (int)App->win->GetScale() };
	else
		position = { pos.x , pos.y };

	return true;
}

bool UI_GUI::OnClick()
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
