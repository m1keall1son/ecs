#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ecs/utils/Handle.h"
#include "ecs/events/EventData.h"

class DemoFarAway : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context) { return std::unique_ptr<DemoFarAway>(new DemoFarAway(context)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	void onClick(EventDataRef);
	void onPresenceDetection(EventDataRef);

	DemoFarAway(Scene* context) : ISceneLogicState(context) {}

	Handle mFarAway;
	Handle mVideoTest;

};