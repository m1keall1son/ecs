#include "DemoEngage.h"
#include "ECS.h"
#include "../IWScene/IWEvents.h"
#include "DemoFarAway.h"
#include "DemoApproach.h"
#include "DemoPDP.h"
#include "DemoEvents.h"
#include "DemoInit.h"

using namespace ci;

void DemoEngage::update()
{
}

void DemoEngage::enter()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoEngage::onClickOut), ClickOutEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoEngage::onEngageClick), EngageClickEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoEngage::onPresenceDetection), PresenceDetectionEvent::TYPE);

	RenderSystem& render_system = *getScene()->getSystem<RenderSystem>();
	uint32_t texture_pool_id = render_system.getMaterialId("VisualStateTexturePool");

	auto init = getScene()->getInitializer<DemoInit>();
	auto visual_state = init->getVisualState();

	{
		auto product = visual_state.get_fast<Entity>();
		auto render = product->getComponent<RenderComponent>();
		auto material = render->getMaterialInstanceHandle(texture_pool_id);
		material.get_fast<TexturePoolMaterialInstance>()->setTextureId(2);
	}

	mEngageProducts = init->getEngageProducts();

	{
		auto product = mEngageProducts[0].get_fast<Entity>();
		product->setPosition(vec3(vec2(1300, 500) + vec2(product->getWorldScale()*.5f), 0) );
		auto ui = product->getComponent<UIComponent>();
		ui->setEnabled(true);
	}
	{
		auto product = mEngageProducts[1].get_fast<Entity>();
		product->setPosition(vec3(vec2(1477, 1701) + vec2(product->getWorldScale()*.5f), 0));
		auto ui = product->getComponent<UIComponent>();
		ui->setEnabled(true);
	}
	{
		auto product = mEngageProducts[2].get_fast<Entity>();
		product->setPosition(vec3(vec2(1398, 2454) + vec2(product->getWorldScale()*.5f), 0));
		auto ui = product->getComponent<UIComponent>();
		ui->setEnabled(true);
	}

}

void DemoEngage::exit()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoEngage::onClickOut), ClickOutEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoEngage::onEngageClick), EngageClickEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoEngage::onPresenceDetection), PresenceDetectionEvent::TYPE);

	if (mNextState != 2)
	{
		{
			auto product = mEngageProducts[0].get_fast<Entity>();
			auto ui = product->getComponent<UIComponent>();
			ui->setEnabled(false);
		}
		{
			auto product = mEngageProducts[1].get_fast<Entity>();
			auto ui = product->getComponent<UIComponent>();
			ui->setEnabled(false);
		}
		{
			auto product = mEngageProducts[2].get_fast<Entity>();
			auto ui = product->getComponent<UIComponent>();
			ui->setEnabled(false);
		}
	}


}

ISceneLogicStateRef DemoEngage::nextLogicState()
{
	switch (mNextState)
	{
	case 0:
		return DemoFarAway::create(getScene());
		break;
	case 1:
		return DemoApproach::create(getScene());
		break;
	case 2:
		return DemoPDP::create(getScene(), mPDPSubState, mClickedProduct);
		break;
	}
}

void DemoEngage::onClickOut(EventDataRef)
{
	mNextState = 1;
	getScene()->requestNextLogicState();
}

void DemoEngage::onEngageClick(EventDataRef eventData)
{
	auto event = std::dynamic_pointer_cast<EngageClickEvent>(eventData);
	mNextState = 2;
	mClickedProduct = event->getProduct();
	mPDPSubState = event->getSubState();
	getScene()->requestNextLogicState();
}

void DemoEngage::onPresenceDetection(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {
		if (!event->isPersonPresent()) {
			mNextState = 0;
			getScene()->requestNextLogicState();
		}
	}
}
