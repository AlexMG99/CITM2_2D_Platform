#include "j1App.h"
#include "UI_Button.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "j1Gui.h"


UI_Button::UI_Button(SDL_Rect rect)
{
	button_rect = rect;

};

bool UI_Button::PostUpdate()
{
	Draw();
	return true;
}
void UI_Button::OnHover()
{
}
void UI_Button::OnClick()
{
}
;

void UI_Button::Draw() const
{
	App->render->Blit(App->gui->GetAtlas(), pos_x, pos_y, &button_rect);
};








