#include "cinder/app/App.h"

#include "FarAwayState.h"
#include "ApproachState.h"
#include "IWInitializer.h"
#include "Ecs.h"
#include "IWEvents.h"

using namespace ci;

void FarAwayState::update() {
}

void FarAwayState::enter() {
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &FarAwayState::onClick), ClickOutEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &FarAwayState::onPresenceDetection), PresenceDetectionEvent::TYPE);

	RenderSystem& renderSystem = *getScene()->getSystem<RenderSystem>();
	AnimationSystem& animation_system = *getScene()->getSystem<AnimationSystem>();

	//grab the default render target
	uint32_t render_target = renderSystem.getMainRenderTarget();
	uint32_t material_id = renderSystem.getMaterialId("BGTexturePoolMaterial");
	uint32_t rect_mesh_id = renderSystem.getMeshId("product_rect");

	auto far_away_handle = getScene()->newEntity("FarAwayVisual");

	{
		Entity& far_away = *far_away_handle.get_fast<Entity>();

		far_away.setPosition(ci::vec3(ci::app::getWindowCenter(), 0));
		ci::vec3 size = ci::vec3(ci::app::getWindowSize(), 1);
		far_away.setLocalScale(size);
		far_away.createComponent<RenderComponent>(RenderTechnique(rect_mesh_id).pass<TexturePoolMaterial>(render_target, material_id, 0, ci::vec4(1.,1.,1.,1.)));
	}
	mHandles.push_back(far_away_handle);

}

void FarAwayState::exit() {
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &FarAwayState::onClick), ClickOutEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &FarAwayState::onPresenceDetection), PresenceDetectionEvent::TYPE);

	for (auto& handle : mHandles) {
		handle.destroy();
	}
	mHandles.clear();
}

ISceneLogicStateRef FarAwayState::nextLogicState()
{
	return ApproachState::create(getScene());
}

void FarAwayState::onClick(EventDataRef)
{
	getScene()->requestNextLogicState();
}

void FarAwayState::onPresenceDetection(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {

		if (event->isPersonPresent())
			getScene()->requestNextLogicState();

	}
}
