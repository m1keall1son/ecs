#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "cinder/app/App.h"
#include "ecs/utils/Handle.h"
#include "ecs/events/EventData.h"
#include "IWStates.h"
#include "jsoncpp/json.h"

class EngageState : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context, IWState previous_state, int subcategory ) { return std::unique_ptr<EngageState>(new EngageState(context, previous_state, subcategory)); }

	void update()override;

	void enter()override;

	void exit()override;

	//todo add other data pieces
	Handle composeProductEntity( const Json::Value& data, const ci::Rectf& layout );
	Handle composeCloudLabel(const std::string& label_text, const ci::Rectf& layout);

	ISceneLogicStateRef nextLogicState()override;

private:

	void clearLayout();

	void onClickOut(EventDataRef);
	void onProductClick(EventDataRef);
	void onRestExperience(EventDataRef);

	EngageState(Scene* context, IWState previous_state, int subcategory) : ISceneLogicState(context), mPreviousState(previous_state), mSubcategory(subcategory) {}

	int mSubcategory;
	IWState mNextState;
	IWState mPreviousState;
	Handle mSelectedProduct;
	Handle mBG;

	std::vector<Handle> mHandles;

};
