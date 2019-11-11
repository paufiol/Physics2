#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

class Body;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
		// On space bar press, create a circle on mouse position
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		//p2List_item<Body *>* item = new p2List_item<Body*>;
		Body item = *App->physics->CreateCircle(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()), PIXEL_TO_METERS(25));
		bodies.add(item);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		// TODO 1: When pressing 2, create a box on the mouse position
		Body item = *App->physics->CreateBox(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()), PIXEL_TO_METERS(50), PIXEL_TO_METERS(25));
		bodies.add(item);
		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!

		float rick_head[60] = {
		40, 0,
		42, 39,
		13, 36,
		29, 63,
		0, 75,
		28, 90,
		8, 102,
		30, 114,
		24, 124,
		41, 125,
		34, 136,
		46, 132,
		58, 144,
		82, 148,
		91, 141,
		98, 126,
		105, 124,
		99, 116,
		103, 104,
		110, 100,
		105, 92,
		110, 87,
		110, 78,
		115, 66,
		106, 62,
		111, 35,
		94, 40,
		93, 22,
		88, 4,
		75, 32
		};

		App->physics->CreateChain(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()), &rick_head[0], 60);

	}
	// TODO 7: Draw all the circles using "circle" texture
	


	if (bodies.getFirst() != nullptr) {
		p2List_item<Body>* i = bodies.getFirst();
		while (i != nullptr) {
			switch (i->data.GetType()) {
			case(0)://circle
				App->renderer->Blit(circle, i->data.GetBodyUpperBound()->x, i->data.GetBodyUpperBound()->y, nullptr, 1.0f, RADTODEG*i->data.GetRotation());
				break;

			case(2)://polygon
				App->renderer->Blit(box, i->data.GetTopLeft()->x, i->data.GetTopLeft()->y, nullptr, 1.0f, RADTODEG*i->data.GetRotation());
				break;
			}
		
			i = i->next;
		}
	}
	
	

	return UPDATE_CONTINUE;
}
