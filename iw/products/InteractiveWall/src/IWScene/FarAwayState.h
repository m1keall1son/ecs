#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ECS.h"
#include "IWInitializer.h"

class FarAwayState : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context) { return std::unique_ptr<FarAwayState>(new FarAwayState(context)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	void onClick(EventDataRef);
	void onPresenceDetection(EventDataRef);

	FarAwayState(Scene* context) : ISceneLogicState(context) {}
	std::vector<Handle> mHandles;
	PersonDetectorRef mDetector;
	bool mPersonPresent;
};
