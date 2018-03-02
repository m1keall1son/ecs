#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ecs/utils/Handle.h"
#include "ecs/events/EventData.h"
#include <array>

class DemoEngage : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context) { return std::unique_ptr<DemoEngage>(new DemoEngage(context)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	void onClickOut(EventDataRef);
	void onEngageClick(EventDataRef);
	void onPresenceDetection(EventDataRef);

	DemoEngage(Scene* context) : ISceneLogicState(context) {}

	int mNextState;
	int mPDPSubState;
	Handle mClickedProduct;

	std::array<Handle, 3> mEngageProducts;

};
