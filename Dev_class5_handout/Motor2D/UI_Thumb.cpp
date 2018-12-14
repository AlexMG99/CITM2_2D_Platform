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
#include "UI_Thumb.h"


UI_Thumb::UI_Thumb(iPoint pos, SDL_Rect s_thumb, UI_GUI* parent)
{

	thumb = s_thumb;
	this->parent = parent;
}

bool UI_Thumb::Start()
{
	return false;
}


bool UI_Thumb::PostUpdate()
{
	bool ret = true;

	ret = App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y, &thumb);

	return ret;
}



//void UI_Slider::OnClick()
//{
//}