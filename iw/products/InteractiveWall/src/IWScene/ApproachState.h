#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ecs/events/EventData.h"
#include "IWStates.h"
#include "IWInitializer.h"
#include "ecs/utils/Handle.h"

class ApproachState : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context) { return std::unique_ptr<ApproachState>(new ApproachState(context)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	void onSubCategoryClick(EventDataRef);
	void onResetExperience(EventDataRef);

	ApproachState(Scene* context) : ISceneLogicState(context) {}

	IWState mNextState;
	int mSelectedSubCategory;
	std::vector<Handle> mHandles;

	PersonDetectorRef mDetector;
	bool mPersonPresent;
};
