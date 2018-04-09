#pragma once

#include "nativeGameLoop.h"
#include "src/box2dDebugDraw.h"
#include "src/platform.h"
using namespace mingine;

namespace mingine {
	extern char stringBuilderBuffer[MAX_STRING];

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

		b2Body* body = nullptr;

		box2dDebugDraw dd;

	public:
		
		RopeTest();
		~RopeTest();

		void start() override;
		void update() override;
		void draw() override;
	};
}

