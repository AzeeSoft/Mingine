#include "ropeTest.h"
#include "platform.h"
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Common/b2Draw.h>
#include "GameMath.h"


namespace mingine 
{

	RopeTest::RopeTest(): gravity(0.0f, -10.0f), world(gravity)
	{
		ropeSegments = 20;
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
		dd.SetFlags(flags);

		dd.camera.x = screenWidth / 2;
		dd.camera.y = screenHeight / 2;

		world.SetDebugDraw(&dd);

		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -40.0f);

		b2Body* groundBody = world.CreateBody(&groundBodyDef);

		b2PolygonShape groundBox;
		groundBox.SetAsBox(50.0f, 10.0f);

		groundBody->CreateFixture(&groundBox, 0.0f);


		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0, 10.0f);
		bodyDef.angle = 3.14f / 3;
		
		playerBody = world.CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f, 1.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.6f;

		playerBody->CreateFixture(&fixtureDef);


		b2RevoluteJointDef ropeJointDef;

		b2Body* prevRopeSegmentBody = playerBody;

		for(int i=0; i<ropeSegments; i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;

			float width = 1.0f;
			float height = 0.3f;

			float y = 10.0f;

			bodyDef.position.Set(1 + (i*width), y);
			bodyDef.angle = 0;

			b2Body* ropeSegmentBody = world.CreateBody(&bodyDef);

			b2PolygonShape ropeSegmentShape;
			ropeSegmentShape.SetAsBox(width/2, height/2);

			b2FixtureDef ropeSegmentFixtureDef;
			ropeSegmentFixtureDef.shape = &ropeSegmentShape;
			ropeSegmentFixtureDef.density = 1.0f;
			ropeSegmentFixtureDef.friction = 0.6f;
			
			ropeSegmentBody->CreateFixture(&ropeSegmentFixtureDef);

			b2Vec2 anchorPoint = b2Vec2((i*width), y);

			if (prevRopeSegmentBody != nullptr) {
				ropeJointDef.Initialize(prevRopeSegmentBody, ropeSegmentBody, anchorPoint);
				world.CreateJoint(&ropeJointDef);
			}

			prevRopeSegmentBody = ropeSegmentBody;
		}

		int boxesCount = 20;
		for(int i = 0; i<boxesCount; i++)
		{
			float x = GameMath::getRandomBetween(-50, 50);
			float y = GameMath::getRandomBetween(-20, 0);

			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(x, y);
			bodyDef.angle = 0;

			b2Body* boxBody = world.CreateBody(&bodyDef);

			b2CircleShape circleShape;
			circleShape.m_radius = 1.0f;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.6f;

			boxBody->CreateFixture(&fixtureDef);
		}

	}

	void RopeTest::update()
	{
		world.Step(timeStep, velocityIterations, positionIterations);

		b2Vec2 force = b2Vec2(0, 0);

		if(keys[MOVE_LEFT_KEY_CODE])
		{
			force.x = -1;
		} if(keys[MOVE_RIGHT_KEY_CODE])
		{
			force.x = 1;
		}

		if (keys[MOVE_UP_KEY_CODE])
		{
			force.y = 1;
		} else if (keys[MOVE_DOWN_KEY_CODE])
		{
			force.y = -1;
		}

		if (force.x != 0 || force.y != 0 ) {
			force.x *= 500;
			force.y *= 400;
			playerBody->ApplyForceToCenter(force, true);
		}
	}

	void RopeTest::draw()
	{
		clearScreen(0,0,0);
		world.DrawDebugData();
	}
}
