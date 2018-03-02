#include "DemoApproach.h"
#include "ECS.h"
#include "../IWScene/IWEvents.h"
#include "DemoEngage.h"
#include "DemoFarAway.h"
#include "DemoInit.h"

using namespace ci;

void DemoApproach::update()
{
}

void DemoApproach::enter()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoApproach::onPresenceDetection), PresenceDetectionEvent::TYPE);

	RenderSystem& render_system = *getScene()->getSystem<RenderSystem>();
	uint32_t texture_pool_id = render_system.getMaterialId("VisualStateTexturePool");

	auto init = getScene()->getInitializer<DemoInit>();
	auto visual_state = init->getVisualState();

	{
		auto product = visual_state.get_fast<Entity>();
		auto render = product->getComponent<RenderComponent>();
		auto material = render->getMaterialInstanceHandle(texture_pool_id);
		material.get_fast<TexturePoolMaterialInstance>()->setTextureId( init->getVisualState("Approach") );
	}

	//1365, 2180
	//460 400

	{
		mSubcatButton = getScene()->newEntity("SubCatButton");
		auto token_button = mSubcatButton.get_fast<Entity>();
		token_button->setScale(vec3(460, 400, 1));
		token_button->setPosition(vec3(1365, 2180, 0) + vec3(vec2(token_button->getWorldScale()*.5f), 0.) );
		auto ui_handle = token_button->createComponent<UIComponent>(Rectf( vec2(-.5), vec2(.5) ), 1, InputType::CLICK);
		auto ui = ui_handle.get_fast<UIComponent>();
		ui->connectClickHandler([&](const InputSample& sample) {
			mNextState = 1;
			getScene()->requestNextLogicState();
		});
	}

}

void DemoApproach::exit()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoApproach::onPresenceDetection), PresenceDetectionEvent::TYPE);

	mSubcatButton.destroy();
}

ISceneLogicStateRef DemoApproach::nextLogicState()
{
	switch (mNextState)
	{
	case 0:
		return DemoFarAway::create(getScene());
		break;
	case 1:
		return DemoEngage::create(getScene());
		break;
	}
}


void DemoApproach::onPresenceDetection(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {
		if (!event->isPersonPresent()) {
			mNextState = 0;
			getScene()->requestNextLogicState();
		}
	}
}
