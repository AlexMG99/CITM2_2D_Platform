#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "UI_Slider.h"
#include "UI_Button.h"

UI_Slider::UI_Slider(SDL_Rect slider_box)
{
	s_box = slider_box;
}

bool UI_Slider::Start()
{
	App->gui->gui_list.add(App->gui->CreateThumb({ pos.x+6, pos.y+1  }, {619,182,24,22},this));
	return true;
}

bool UI_Slider::PostUpdate()
{
	bool ret = true;

	ret = App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y, &s_box);


	return ret;
}

//void UI_Slider::OnClick()
//{
//}
