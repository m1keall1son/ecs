#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ecs/utils/Handle.h"
#include "ecs/events/EventData.h"
#include "IWStates.h"

class PDPState : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context, int subcategory, const Handle& product_handle ) { return std::unique_ptr<PDPState>(new PDPState(context, subcategory, product_handle)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	void onProductClick(EventDataRef);
	void onClickOut(EventDataRef);
	void onResetExperience(EventDataRef);

	PDPState(Scene* context, int subcategory, const Handle&  product_handle) : ISceneLogicState(context), mSubcategory(subcategory), mSelectedProduct(product_handle), mCurrentProduct(product_handle) {}

	IWState mNextState;
	int mSubcategory;
	Handle mCurrentProduct;
	Handle mSelectedProduct;
	std::vector<Handle> mHandles;

};