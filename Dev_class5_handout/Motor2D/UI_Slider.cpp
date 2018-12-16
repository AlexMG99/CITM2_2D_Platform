#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1SceneMenu.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "UI_Thumb.h"
#include "UI_Slider.h"
#include "UI_Sprite.h"
#include "UI_Button.h"

UI_Slider::UI_Slider(SDL_Rect slider_box, UI_GUI* parent /*p2Point<int> pivo*/)
{
	s_box = slider_box;
	
}

bool UI_Slider::Start()
{
	
	thumb = App->gui->CreateThumb({ pos.x + 1, pos.y + 1}, { 619,182,24,22 }, this);
	thumb->drag = true;
	return true;
}

bool UI_Slider::PostUpdate()
{
	bool ret = true;

	ret = App->render->Blit(App->gui->GetAtlas(), pos.x, pos.y , &s_box);



	return ret;
}












float UI_Slider::GetThumbValue()
{
	float ipos_bar = thumb->thumb_box.x +(thumb->thumb_box.w/2);
	float fixed_pos = box.x + (thumb->thumb_box.w / 2);
	float fpos_bar = box.x + box.w - (thumb->thumb_box.w / 2);
	float final_pos = (ipos_bar - fixed_pos) / (fpos_bar - fixed_pos);

	return final_pos;
	/*float ipos_slider = l
	/*return 0.0f;*/
}
