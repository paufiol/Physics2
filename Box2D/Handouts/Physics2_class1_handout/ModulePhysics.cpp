#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

// TODO 1: Include Box 2 header and library
#include "Box2D/Box2D/Box2D.h"


#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib")
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib")
#endif


ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	
	b2Vec2 gravity(0.0f, 10.0f);
	world = new	b2World(gravity);

	// TODO 4: Create a a big static circle as "ground"

	
	b2BodyDef circleDef;
	circleDef.type = b2_staticBody;
	circleDef.position.Set(PIXEL_TO_METER(SCREEN_WIDTH / 2), PIXEL_TO_METER(SCREEN_HEIGHT / 2));
	b2Body* circleBody = world->CreateBody(&circleDef);
	b2CircleShape circleShape;
	circleShape.m_radius = PIXEL_TO_METER(SCREEN_HEIGHT / 6);



	b2FixtureDef fixture;
	fixture.shape = &circleShape;

	circleBody->CreateFixture(&circleShape, 0.0f);

	//Ground
	b2BodyDef groundDef;
	groundDef.type = b2_staticBody;
	groundDef.position.Set(PIXEL_TO_METER(0), PIXEL_TO_METER(700));
	b2Body* groundBody = world->CreateBody(&groundDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(PIXEL_TO_METER(SCREEN_WIDTH), PIXEL_TO_METER(50));



	b2FixtureDef groundfixture;
	groundfixture.shape = &groundShape;

	groundBody->CreateFixture(&groundShape, 0.0f);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world->Step(1.0f / 60.0f, 8, 3);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		
		b2BodyDef groundDef;
		groundDef.type = b2_dynamicBody;
		groundDef.position.Set(PIXEL_TO_METER(App->input->GetMouseX()), PIXEL_TO_METER(App->input->GetMouseY()));
		b2Body* groundBody = world->CreateBody(&groundDef);
		b2CircleShape groundShape;
		groundShape.m_radius = PIXEL_TO_METER(SCREEN_HEIGHT / 12);

		b2FixtureDef fixture;
		fixture.shape = &groundShape;

		groundBody->CreateFixture(&groundShape, 0.0f);
	}
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METER_TO_PIXEL(pos.x), METER_TO_PIXEL(pos.y), METER_TO_PIXEL(shape->m_radius), 255, 255, 255);
				}
				break;
				case b2Shape::e_polygon:
				{
					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					SDL_Rect Rect;
					Rect.x = METER_TO_PIXEL(pos.x);
					Rect.y = METER_TO_PIXEL(pos.y);
					Rect.w = METER_TO_PIXEL(shape->m_vertices[2].x);
					Rect.h = METER_TO_PIXEL(shape->m_vertices[3].y);

					App->renderer->DrawQuad(Rect, 128, 0, 0);
				}
				break;
				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;


	return true;
}
