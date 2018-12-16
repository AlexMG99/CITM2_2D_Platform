//#include "j1App.h"
//#include "p2Log.h"
//#include "j1Gui.h"
//#include "j1Window.h"
//#include "j1Scene.h"
//#include "j1SceneMenu.h"
//#include "j1Input.h"
//#include "j1Render.h"
//#include "j1Fonts.h"
//#include "UI_Slider.h"
//#include "UI_Sprite.h"
//#include "UI_Button.h"
//
//UI_Slider::UI_Slider(SDL_Rect slider_box, SDL_Rect* section, UI_GUI* parent, p2Point<int> pivot = { 0,0 })
//{
//
//	/*s_box = *section;
//
//	SDL_Rect thumb_volume = { 450,182,133,25 };
//	thumb = App->gui->CreateSprite({ 219,282,24,22 },&thumb_volume, this, {0,0});
//	thumb->drag = true;*/
//}
//
//bool UI_Slider::Start()
//{
//	//App->gui->gui_list.add(App->gui->CreateThumb({ pos.x+6, pos.y+1  }, {619,182,24,22},this));
//	return true;
//}
//
//bool UI_Slider::PostUpdate()
//{
//	/*bool ret = true;
//
//	ret = App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), box.x - pivot.x, box.y - pivot.y, &s_box, SDL_FLIP_NONE);
//
//
//
//	return ret;*/
//}
//
//float UI_Slider::GetThumbValue()
//{
//	/*float ipos_bar = thumb->box.x + (thumb->box.w / 2);
//	float fixed_pos = box.x + (thumb->box.w / 2);
//	float fpos_bar = box.x + box.w - (thumb->box.w / 2);
//	float final_pos = (ipos_bar - fixed_pos) / (fpos_bar - fixed_pos);
//
//	//return final_pos;*/
//	///*float ipos_slider = l
//	return 0.0f;
//}
