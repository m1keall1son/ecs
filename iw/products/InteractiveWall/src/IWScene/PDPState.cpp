#include "PDPState.h"
#include "EngageState.h"
#include "FarAwayState.h"
#include "IWEvents.h"
#include "ecs/engine/Engine.h"
#include "IWInitializer.h"
#include "Ecs.h"

void PDPState::update()
{
}

void PDPState::enter()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &PDPState::onClickOut), ClickOutEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &PDPState::onProductClick), ProductClickEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &PDPState::onResetExperience), PresenceDetectionEvent::TYPE);

	auto prod = mCurrentProduct.get_fast<Entity>();
	//animate body size up
	auto rigid_body = prod->getComponentHandle<RigidBodyComponent2d>();
	rigid_body.get_fast<RigidBodyComponent2d>()->setBodyType(b2BodyType::b2_staticBody);
	auto animation = prod->getComponent<AnimationComponent>();
	//at the end of the animation reset body to dynamic
	animation->scaleWorldTo(ci::vec3(2,2,1), .5f, Animation::Format().easeFn(ci::EaseOutCubic()).finishFn([&, rigid_body](double finish_time) {
		//rigid_body.get_fast<RigidBodyComponent2d>()->setBodyType(b2BodyType::b2_dynamicBody);
	}));

}

void PDPState::exit()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &PDPState::onClickOut), ClickOutEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &PDPState::onProductClick), ProductClickEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &PDPState::onResetExperience), PresenceDetectionEvent::TYPE);

	//animate body size out
	auto old_prod = mCurrentProduct.get_fast<Entity>();
	auto rigid_body = old_prod->getComponentHandle<RigidBodyComponent2d>();
	//rigid_body.get_fast<RigidBodyComponent2d>()->setBodyType(b2BodyType::b2_staticBody);
	auto animation = old_prod->getComponent<AnimationComponent>();
	//at the end of the animation reset body to dynamic
	animation->scaleWorldTo(ci::vec3(1, 1, 1), .5f, Animation::Format().easeFn(ci::EaseOutCubic()).finishFn([&, rigid_body](double finish_time) {
		rigid_body.get_fast<RigidBodyComponent2d>()->setBodyType(b2BodyType::b2_dynamicBody);
	}));

}

ISceneLogicStateRef PDPState::nextLogicState()
{
	switch (mNextState) {
	case IWState::ENGAGE_STATE:
	{
		return EngageState::create(getScene(), IWState::PDP_STATE, mSubcategory);
	}break;
	case IWState::PDP_STATE:
	{
		return PDPState::create(getScene(), mSubcategory, mSelectedProduct);
	}break;
	case IWState::FAR_AWAY_STATE:
	{
		return FarAwayState::create(getScene());
	}break;
	default: CI_LOG_E("No proper State selected"); return nullptr; break;
	}
}

void PDPState::onProductClick(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<ProductClickEvent>(eventData)) {

		if (event->getProductHandle() == mSelectedProduct)return;

		auto prod = event->getProductHandle().get_fast<Entity>();
		if (auto product_comp = prod->getComponent<ProductComponent>())
		{
			//set next state and selected product handle
			mNextState = IWState::PDP_STATE;
			mSelectedProduct = event->getProductHandle();
			getScene()->requestNextLogicState();

		}
		else {
			CI_LOG_E("event did not contain a product handle!");
		}
	}
	else {
		CI_LOG_E("event was not of type: ProductClickEvent");
	}
}

void PDPState::onClickOut(EventDataRef)
{
	mNextState = IWState::ENGAGE_STATE;
	getScene()->requestNextLogicState();
}

void PDPState::onResetExperience(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {
		if (!event->isPersonPresent()) {
			mNextState = IWState::FAR_AWAY_STATE;
			getScene()->requestNextLogicState();
		}
	}
}
