#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ecs/utils/Handle.h"
#include "ecs/events/EventData.h"

class DemoApproach : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context) { return std::unique_ptr<DemoApproach>(new DemoApproach(context)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	void onPresenceDetection(EventDataRef);

	DemoApproach(Scene* context) : ISceneLogicState(context) {}

	int mNextState;

	Handle mSubcatButton;

};
