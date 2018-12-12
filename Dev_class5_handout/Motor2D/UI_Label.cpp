#include "j1App.h"
#include "p2Log.h"
#include "UI_Label.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Textures.h"


UI_Label::UI_Label(const char* text, Label_Type type, SDL_Color color, UI_GUI* parent)
{
	label_text = text;
	label_type = type;
	label_color = color;

	this->parent = parent;
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
	App->render->Blit(tex, pos.x - App->render->camera.x / (int)App->win->GetScale(), pos.y);
	
	switch (state)
	{
	case IDLE:
		ChangeTexture({ 0,0,0,0 });
		pos.y = parent->pos.y + 5;
		pos.x = parent->pos.x + 10;
		break;
	case HOVER:
		ChangeTexture({ 230,214,144,255 });
		pos.y = parent->pos.y + 5;
		pos.x = parent->pos.x + 12;
		break;
	case CLICK:
		pos.y = parent->pos.y + 8;
		pos.x = parent->pos.x + 12;
		break;
	}
	return true;
}

bool UI_Label::OnHover()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	uint w, h;
	App->tex->GetSize(tex, w, h);
	return ((pos.x < x && pos.y + App->render->camera.y < y && pos.x + (int)w > x && pos.y + (int)h > y) || parent->OnHover());
}

void UI_Label::ChangeTexture(SDL_Color color)
{
	App->tex->UnLoad(tex);
	label_color = color;
	tex = App->font->Print(label_text.GetString(), label_color, App->font->title_buttons);
}
