#include "j1App.h"
#include "UI_Label.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "j1Fonts.h"


UI_Label::UI_Label(const char* text, SDL_Color color)
{
	label_text = text;
	label_color = color;
}



bool UI_Label::Start()
{
	tex = App->font->Print(label_text.GetString(), label_color, App->font->title);
	return true;
}

bool UI_Label::PostUpdate()
{
	Draw();
	return true;
}

void UI_Label::OnHover()
{
}

void UI_Label::Draw()const
{
	App->render->Blit(tex, pos_x, pos_y);
}