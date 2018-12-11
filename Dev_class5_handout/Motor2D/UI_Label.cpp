#include "j1App.h"
#include "UI_Label.h"
#include "j1Render.h"
#include "j1Fonts.h"


UI_Label::UI_Label(const char* text, Label_Type type, SDL_Color color)
{
	label_text = text;
	label_type = type;
	label_color = color;
}



bool UI_Label::Start()
{
	switch (label_type) {

	case TITLE:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->title);
		break;
	case CONFIG:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->title_config);
		break;
	case BUTTON:
		tex = App->font->Print(label_text.GetString(), label_color, App->font->title_buttons);
		break;
	}
	return true;
}

bool UI_Label::PostUpdate()
{
	App->render->Blit(tex, pos.x, pos.y);
	return true;
}

bool UI_Label::OnHover()
{
	return true;
}
