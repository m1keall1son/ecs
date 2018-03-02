#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ecs/utils/Handle.h"
#include "ecs/events/EventData.h"
#include "cinder/Vector.h"

class DemoPDP : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context, int state, const Handle& engage_product) { return std::unique_ptr<DemoPDP>(new DemoPDP(context, state, engage_product)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	Handle newButton(const std::string& name, const std::string& next_state, const ci::vec2& position, const ci::vec2& size, bool does_nothing = false);

	void onClickOut(EventDataRef);
	void onEngageClick(EventDataRef);
	void onPresenceDetection(EventDataRef);
	void onCollection(EventDataRef);

	DemoPDP(Scene* context, int state, const Handle& engage_product ) : ISceneLogicState(context), mSubState(state), mPDP(engage_product) {}

	int mNextState;
	int mSubState;
	Handle mClickedProduct;

	Handle mPDP;
	Handle mTokenButton;

	std::vector<Handle> mPdpLayout;

};
