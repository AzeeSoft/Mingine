#pragma once

#include "nativeGameLoop.h"
#include "box2dDebugDraw.h"
#include "platform.h"
#include <vector>

using namespace mingine;
using namespace std;

namespace mingine 
{

	class RopeTest : public NativeGameLoop
	{

	public:
		static const int FPS = 60;
		static const int FRAME_TIME_NS = (1000 / FPS) * 1000 * 1000;

		float32 timeStep = 1.0f / FPS;
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		b2Vec2 gravity;
		b2World world;

		b2Body* playerBody = nullptr;

		box2dDebugDraw dd;

	private:
		const int MOVE_RIGHT_KEY_CODE = 79;
		const int MOVE_LEFT_KEY_CODE = 80;
		const int MOVE_DOWN_KEY_CODE = 81;
		const int MOVE_UP_KEY_CODE = 82;

		int ropeSegmentsCount;

	public:
		
		RopeTest();
		~RopeTest();

		void start() override;
		void update() override;
		void draw() override;
	};
}

