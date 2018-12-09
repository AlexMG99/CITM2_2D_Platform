#include "j1App.h"
#include "UI_Button.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "j1Gui.h"


UI_Button::UI_Button(SDL_Rect rect, const char* text)
{
	button_rect = rect;
	button_text.create(text);
}

bool UI_Button::Start()
{
	if (button_text.Length() > 0)
		App->gui->gui_list.add(App->gui->CreateLabel({ pos_x + 10, pos_y + 5 }, button_text.GetString()));
	return true;
}

bool UI_Button::PostUpdate()
{
	App->render->Blit(App->gui->GetAtlas(), pos_x, pos_y, &button_rect);
	return true;
}
void UI_Button::OnHover()
{
}
void UI_Button::OnClick()
{
}









