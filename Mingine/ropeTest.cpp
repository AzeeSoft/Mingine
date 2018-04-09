#include "ropeTest.h"
#include "src/platform.h"
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Common/b2Draw.h>


namespace mingine {
	RopeTest::RopeTest(): gravity(0.0f, -10.0f), world(gravity)
	{

	}


	RopeTest::~RopeTest()
	{
	}

	void RopeTest::start()
	{
		int screenWidth = 1024;
		int screenHeight = 768;

		if (!initPlatform(screenWidth, screenHeight, false))
		{
			freePlatform();
		}

		setWindowTitle("Rope Test");

		uint32 flags = b2Draw::e_shapeBit;

		//uncomment to see bounding boxes.
		//flags += b2Draw::e_aabbBit;

		dd.SetFlags(flags);

		dd.camera.x = screenWidth / 2;
		dd.camera.y = screenHeight / 2;

		world.SetDebugDraw(&dd);

		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -20.0f);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = world.CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(50.0f, 10.0f);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);

		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0, 10.0f);
		bodyDef.angle = 3.14f / 3;
		body = world.CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f, 1.0f);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.6f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);

		// create a circular body
		b2BodyDef bd;
		bd.position.Set(25.5f, 10.0f);
		bd.type = b2_dynamicBody;

		b2Body* body = world.CreateBody(&bd);

		b2CircleShape shape;
		shape.m_radius = 1.5f;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 20.0f;
		fd.restitution = 0.8f;
		body->CreateFixture(&fd);
	}

	void RopeTest::update()
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		snprintf(stringBuilderBuffer, sizeof(stringBuilderBuffer), "%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
		log(stringBuilderBuffer);
	}

	void RopeTest::draw()
	{
		clearScreen(68, 136, 204);
		world.DrawDebugData();
	}
}
