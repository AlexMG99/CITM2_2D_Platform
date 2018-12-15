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

UI_Label::~UI_Label()
{
	if (tex != nullptr)
	{
		App->tex->UnLoad(tex);
		tex = nullptr;
	}
}



bool UI_Label::Start()
{
	ChangeTexture(label_color);
	return true;
}

bool UI_Label::PostUpdate()
{
	App->render->Blit(tex, position.x, position.y);
	
	switch (state)
	{
	case IDLE:
		ChangeTexture({ 0,0,0,0 });
		break;
	case HOVER:
		if (label_type != CONFIG && label_type != TITLE) {
			ChangeTexture({ 230,214,144,255 });
		}
		break;
	case CLICK:
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
	bool ret = false;
	if (parent != nullptr)
	{
		ret = parent->OnHover();
	}
	return ((pos.x < x && pos.y + App->render->camera.y < y && pos.x + (int)w > x && pos.y + (int)h > y) || ret);
}

void UI_Label::ChangeTexture(SDL_Color color)
{
	if(tex != nullptr)
		App->tex->UnLoad(tex);

	label_color = color;

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
}

void UI_Label::ChangeText(const char * text)
{
	if (tex != nullptr)
		App->tex->UnLoad(tex);

	label_text.Clear();
	label_text.create(text);

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
}
