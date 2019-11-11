#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// TODO 2: Place the camera one unit up in Y and one unit to the right
	// experiment with different camera placements, then use LookAt()
	// to make it look at the center
	App->camera->Position.x = 3;
	App->camera->Position.y = 3;
	App->camera->Position.z = 3;

	App->camera->LookAt(vec3(0, 0, 0));
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
	// TODO 1: Create a Plane primitive. This uses the plane formula
	// so you have to express the normal of the plane to create 
	// a plane centered around 0,0. Make that it draw the axis for reference
	Plane plane;
	plane.axis = true;
	plane.Render();


	// TODO 6: Draw a sphere of 0.5f radius around the center
	// Draw somewhere else a cube and a cylinder in wireframe
	Sphere sphere;
	sphere.radius = 0.35f;
	sphere.wire = true;
	sphere.SetPos(0, 0.5f, 1.2f);
	sphere.Render();

	Cube cube;
	cube.size = 1.7f;
	cube.SetPos(-0.7f, 0.8f, -2.3f);
	cube.Render();

	Cylinder cylinder;
	cylinder.radius = 0.3f;
	cylinder.height = 7.0f;
	
	cylinder.SetRotation(90.0f, vec3(0, 0, 1.0f));
	cylinder.SetPos(-2, 0, -1);
	
	cylinder.Render();





	return UPDATE_CONTINUE;
}

