#include "DemoFarAway.h"
#include "ECS.h"
#include "DemoApproach.h"
#include "../IWScene/IWEvents.h"
#include "DemoInit.h"
#include "VideoMaterial.h"

using namespace ci;

void DemoFarAway::update()
{
}

void DemoFarAway::enter()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoFarAway::onClick), ClickOutEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoFarAway::onPresenceDetection), PresenceDetectionEvent::TYPE);

	auto& render_system = *getScene()->getSystem<RenderSystem>();
	auto& animation_system = *getScene()->getSystem<AnimationSystem>();

	uint32_t texture_pool_id = render_system.getMaterialId("VisualStateTexturePool");

	auto init = getScene()->getInitializer<DemoInit>();
	auto far_away_handle = init->getFarAway();

	auto product = far_away_handle.get_fast<Entity>();
	auto render = product->getComponent<RenderComponent>();
	render->setVisible(true);

	auto anim = animation_system.newAnimation(1.f, Animation::Format().easeFn(ci::EaseOutCubic()).updateFn([far_away_handle, texture_pool_id](float time) {
		auto product = far_away_handle.get_fast<Entity>();
		auto render = product->getComponent<RenderComponent>();
		auto material = render->getMaterialInstanceHandle(texture_pool_id);
		material.get_fast<TexturePoolMaterialInstance>()->setColor( vec4( 1.f,1.f,1.f,time ) );
	}).finishFn([far_away_handle, texture_pool_id](float time) {
		auto product = far_away_handle.get_fast<Entity>();
		auto render = product->getComponent<RenderComponent>();
		auto material = render->getMaterialInstanceHandle(texture_pool_id);
		material.get_fast<TexturePoolMaterialInstance>()->setColor(vec4(1.f, 1.f, 1.f, 1.f));
	}));


	/*uint32_t video_material_id = render_system.getMaterialId("FarAwayVideoMaterial");
	auto video_material =  std::dynamic_pointer_cast<VideoMaterial>( render_system.retrieveMaterial(video_material_id) );
	video_material->play();

	mVideoTest = getScene()->newEntity("vid_test");
	auto vid = mVideoTest.get_fast<Entity>();
	vid->createComponent<RenderComponent>(RenderTechnique( render_system.getMeshId("flipped_rect") ).pass<VideoMaterial>(0, video_material_id));
	vid->setScale(vec3(app::getWindowSize(), 1));
	vid->setPosition(vec3(app::getWindowCenter(), 3));*/

	init->clearTokens();

}

void DemoFarAway::exit()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoFarAway::onClick), ClickOutEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoFarAway::onPresenceDetection), PresenceDetectionEvent::TYPE);

	mVideoTest.destroy();

	auto& render_system = *getScene()->getSystem<RenderSystem>();
	auto& animation_system = *getScene()->getSystem<AnimationSystem>();
	uint32_t texture_pool_id = render_system.getMaterialId("VisualStateTexturePool");

	auto init = getScene()->getInitializer<DemoInit>();

	auto visual_handle = init->getVisualState();
	auto visual = visual_handle.get_fast<Entity>()->getComponent<RenderComponent>();
	visual->setVisible(true);

	auto far_away_handle = init->getFarAway();

	auto anim = animation_system.newAnimation(1.f, Animation::Format().easeFn(ci::EaseOutCubic()).updateFn([far_away_handle, texture_pool_id](float time) {
		auto product = far_away_handle.get_fast<Entity>();
		auto render = product->getComponent<RenderComponent>();
		auto material = render->getMaterialInstanceHandle(texture_pool_id);
		material.get_fast<TexturePoolMaterialInstance>()->setColor(vec4(1.f, 1.f, 1.f, 1.f-time));
	}).finishFn([far_away_handle, texture_pool_id](float time) {
		auto product = far_away_handle.get_fast<Entity>();
		auto render = product->getComponent<RenderComponent>();
		auto material = render->getMaterialInstanceHandle(texture_pool_id);
		material.get_fast<TexturePoolMaterialInstance>()->setColor(vec4(1.f, 1.f, 1.f, 0));
		render->setVisible(false);
	}));

	//uint32_t video_material_id = render_system.getMaterialId("FarAwayVideoMaterial");
	//auto video_material = std::dynamic_pointer_cast<VideoMaterial>(render_system.retrieveMaterial(video_material_id));
	//video_material->pause();

}

ISceneLogicStateRef DemoFarAway::nextLogicState()
{
	return DemoApproach::create(getScene());
}

void DemoFarAway::onClick(EventDataRef)
{
	getScene()->requestNextLogicState();
}

void DemoFarAway::onPresenceDetection(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {
		if (event->isPersonPresent()) {
			getScene()->requestNextLogicState();
		}
	}
}
