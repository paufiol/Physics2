#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

p2Point<float>* Body::GetBodyPos() {
	p2Point<float>* ret = new p2Point<float>;

	ret->x = METERS_TO_PIXELS(body->GetPosition().x);
	ret->y = METERS_TO_PIXELS(body->GetPosition().y);

	return ret;
}

p2Point<float>* Body::GetBodyUpperBound() {
	p2Point<float>* ret = new p2Point<float>;

	//This should work For everything but circles. I guess.
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		aabb.Combine(aabb, fixture->GetAABB(0));
	}
	
	ret->x = METERS_TO_PIXELS(aabb.lowerBound.x);
	ret->y = METERS_TO_PIXELS(aabb.lowerBound.y);
	
	return ret;
}

p2Point<float>* Body::GetTopLeft() {

	b2Fixture* f = body->GetFixtureList();
	b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
	b2Vec2 vec = polygonShape->GetVertex(0);
	vec = body->GetWorldPoint(vec);
	
	p2Point<float> ret;
	ret.x = METERS_TO_PIXELS(vec.x);
	ret.y = METERS_TO_PIXELS(vec.y);

	return &ret;
}

float Body::GetRotation() {
	float ret = body->GetAngle();
	return  ret;
}

int Body::GetType() {
	return (int)body->GetFixtureList()->GetType();
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{


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
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}

Body* ModulePhysics::CreateCircle(int x, int y, float r) {
	b2BodyDef body;
	body.type = b2_dynamicBody;
	float radius = r;
	body.position.Set(x, y);

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	Body ret;
	ret.body = b;

	return &ret;
}

Body*  ModulePhysics::CreateBox(int x, int y, float w, float h) {
	b2BodyDef groundDef;
	groundDef.type = b2_dynamicBody;
	groundDef.position.Set(x, y);
	b2Body* groundBody = world->CreateBody(&groundDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(w, h);

	b2FixtureDef fixture;
	fixture.shape = &groundShape;

	groundBody->CreateFixture(&groundShape, 1.0f);

	Body ret;
	ret.body = groundBody;
	return &ret;
}

void ModulePhysics::CreateChain(int x, int y, float* points, uint array_size) {
	b2BodyDef chainDef;
	chainDef.type = b2_dynamicBody;
	chainDef.position.Set(x, y);
	b2Body* groundBody = world->CreateBody(&chainDef);

	b2Vec2 *vecs = new b2Vec2[(array_size/2)];

	for (int i = 0, j = 0; i < (array_size/2); i++) {

		vecs[i].Set(PIXEL_TO_METERS(points[j]), PIXEL_TO_METERS(points[j + 1]));
		j += 2;
	}

	b2ChainShape chain;
	chain.CreateLoop(vecs, 30);

	b2FixtureDef fixture;
	fixture.shape = &chain;

	groundBody->CreateFixture(&chain, 1.0f);
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}
