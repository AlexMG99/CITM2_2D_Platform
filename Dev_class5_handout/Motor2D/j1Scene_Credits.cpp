#include "j1App.h"
#include "j1Scene_Credits.h"
#include "j1SceneMenu.h"
#include "p2Log.h"
#include "j1Input.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "UI_Label.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1FadeToBlack.h"
#include "UI_GUI.h"
#include "j1Window.h"


#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"


#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1Scene_Credits::j1Scene_Credits() : j1Module()
{
	name.create("scene_menu");
}

// Destructor
j1Scene_Credits::~j1Scene_Credits()
{}

// Called before render is available
bool j1Scene_Credits::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Credits");
	bool ret = true;
	path.create(config.child("path").child_value());
	
	return ret;
}

// Called before the first frame
bool j1Scene_Credits::Start()
{
	
	bool ret = true;
	App->render->camera = { 0,0 };

	p2SString credits;
	credits.create("MIT License		Copyright(c)[2018][Alex Morales & Laia Martinez] Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	
	/*uint w, h;
	App->win->GetWindowSize(w, h);*/


	App->gui->CreateSprite({ -2, 0 }, { 1121,4,373,301 });
	/*App->gui->CreateLabel({ 0,0 },credits.GetString(), CRED, { 255,0,0,0 });*/
	App->gui->CreateLabel({ 0,0 }, "MIT License", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,10 }, "Permission is hereby granted, free of charge, to any person obtaining a copy]", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,20 }, "of this software and associated documentation files (the Software), to deal", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,30 }, "in the Software without restriction, including without limitation the rights", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,40 }, "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,50 }, "copies of the Software, and to permit persons to whom the Software is", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,60 }, "furnished to do so, subject to the following conditions:", CRED, { 255,0,0,0 });

	App->gui->CreateLabel({ 0,75 }, "The above copyright notice and this permission notice shall be included in all", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,85 }, "copies or substantial portions of the Software.", CRED, { 255,0,0,0 });

	App->gui->CreateLabel({ 0,100 }, "THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,110 }, "EXPRESS OR, IMPLIED, INCLUDING BUT NOT LIMITED TO THE ", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,120 }, "WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,130 }, "PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THEAUTHORS", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,140 }, "OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,150 }, "OTHERLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,160 }, "OTHERWISE, ARISING FROMOUT OF OR IN CONNECTION WITH THE SOFTWARE OR", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,170 }, "SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.", CRED, { 255,0,0,0 });

	App->gui->CreateLabel({ 0,190 }, "GitHub: [LaiaMartinezMotis]  (https :  (github.com/LaiaMartinezMotis)", CRED, { 255,0,0,0 });
	App->gui->CreateLabel({ 0,200 }, "GitHub : [AlexMG99]  (https :  (github.com/AlexMG99)", CRED, { 255,0,0,0 });



	/*App->gui->CreateSprite({ 0,0 }, {1137,14,452,532});*/
	SDL_Rect button_rect[3] = { { 705,20,115,29 }, { 705,51,115,29 }, { 705,82,115,29 } };
	button_list.add(App->gui->CreateButton({ 98,218 }, Button_Type::BACK, button_rect[0], &button_rect[1], &button_rect[2], "BACK", true));
	//button_list.add(App->gui->CreateButton({ 102, 185 }, Button_Type::EXIT, button_rect[0], &button_rect[1], &button_rect[2], "Exit", true));
	
	App->gui->Enable();
	return ret;
}

// Called each loop iteration
bool j1Scene_Credits::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_SceneCredits", Profiler::Color::CornflowerBlue);



	return true;
}

// Called each loop iteration
bool j1Scene_Credits::Update(float dt)
{
	BROFILER_CATEGORY("Update_SceneCredits", Profiler::Color::DarkKhaki);
	
	bool ret = true;
	
	p2List_item<UI_Button*>* button_item = button_list.start;
	while (button_item != NULL)
	{
		if (button_item->data->OnClick())
		{
			switch (button_item->data->GetType())
			{
			case BACK:
				App->fadeToBlack->FadeToBlack(App->scene_credits, App->scene_menu);
				break;
			}
			
		}
		button_item = button_item->next;
	}	
	return ret; 
}

// Called each loop iteration
bool j1Scene_Credits::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_SceneCredits", Profiler::Color::MediumVioletRed);

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene_Credits::CleanUp()
{
	LOG("Freeing Scene Menu");
	button_list.clear();
	App->gui->Disable();
	return true;
}


