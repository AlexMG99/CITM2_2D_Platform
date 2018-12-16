#include "j1App.h"
#include "j1Scene_Credits.h"
#include "p2Log.h"
#include "j1Input.h"
#include "UI_GUI.h"
#include "j1Render.h"
#include "UI_Label.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_GUI.h"


#include "Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"

j1Scene_Credits::j1Scene_Credits() : j1Module()
{
	name.create("scene_credits");
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

	credits.create("MIT License		Copyright(c)[2018][Alex Morales & Laia Martinez] Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	SDL_Rect button_rect[3] = { { 705,20,115,29 }, { 705,51,115,29 }, { 705,82,115,29 } };
	App->gui->CreateButton({ 18,215 }, Button_Type::BACK, button_rect[0], &button_rect[1], &button_rect[2], "Button", true);
	
	App->gui->Start();

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
	
	return true;
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
	App->gui->CleanUp();
	return true;
}


