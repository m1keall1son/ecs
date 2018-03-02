#include "EngageState.h"
#include "ApproachState.h"
#include "PDPState.h"
#include "IWStates.h"
#include "IWEvents.h"
#include "FarAwayState.h"
#include "ecs/engine/Engine.h"
#include "IWInitializer.h"
#include "cinder/Rand.h"
#include "Ecs.h"

using namespace ci;

void EngageState::update()
{
	
}

void EngageState::enter()
{

	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &EngageState::onClickOut), ClickOutEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &EngageState::onProductClick), ProductClickEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &EngageState::onRestExperience), PresenceDetectionEvent::TYPE);

	if (mPreviousState == IWState::APPROACH_STATE) {

		auto init = getScene()->getInitializer<IWInitializer>();

		std::shared_ptr<IWInitializer> initializer = getScene()->getInitializer<IWInitializer>();

		float w = ci::app::getWindowWidth() / 25.f;
		int cols = ci::app::getWindowWidth() / w;
		float x = 0;
		float y = 0;
		int i = 0;
		int j = 0;
		Json::Value cloudList = initializer->getSubCategoryData(mSubcategory)["clouds"];
		auto & layout = initializer->getLayout(mSubcategory);

		std::vector< std::vector< Handle> > products_by_cloud;
		std::vector<Handle> labels;

		int c = 0;
		for (auto& cloudId : cloudList) {
			Json::Value cloud = initializer->getCloudData(atoi(cloudId.asString().c_str()));

			//create cloud label
			auto label_layout = layout.clouds[c].label;
			auto cloud_name = cloud["name"].asString();
			auto label_handle = composeCloudLabel(cloud_name, label_layout);
			init->getEngageLayout().push_back(label_handle);
			labels.push_back(label_handle);

			std::vector< Handle > product_list;

			//create and place cloud entities
			int p = 0;
			for (auto& productSku : cloud["products"]) {
				auto product_layout = layout.clouds[c].products[p].placement;
				Json::Value & product = initializer->getProductData(productSku.asString());
				auto product_handle = composeProductEntity(product, product_layout);
				init->getEngageLayout().push_back(product_handle);
				product_list.push_back(product_handle);
				p++;
			}

			products_by_cloud.push_back(product_list);
			c++;
		}

		//CREATE SPRINGS

		auto & physics_system = getScene()->getSystem<PhysicsSystem2d>();

		auto & sub_cat_connections = initializer->getSpringConnections(mSubcategory);
		c = 0;
		for (auto & cloud_connections : sub_cat_connections) {

			auto cloud_center = layout.clouds[c].placement.getCenter();

			auto name = "cloud_" + std::to_string(c) + "_anchor";
			auto anchor_handle = getScene()->newEntity(name);

			auto & anchor = *anchor_handle.get_fast<Entity>();

			anchor.setPosition(vec3(cloud_center, 0));
			anchor.createComponent<RigidBodyComponent2d>(RigidBodyComponent2d::Format().type(b2BodyType::b2_staticBody).noFixture().fixedRotation(true).density(2.f).friction(1.f).restitution(.01f).linearDamping(.3));

			init->getEngageLayout().push_back(anchor_handle);

			auto &label_handle = labels[c];

			physics_system->newSpring(anchor_handle, anchor_handle.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), label_handle, label_handle.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), 22000.f, 5.f, glm::distance(cloud_center, layout.clouds[c].label.getCenter()));

			for (auto & label_connection : cloud_connections.first) {

				auto & product_1 = products_by_cloud[c][label_connection];
				physics_system->newSpring(label_handle, label_handle.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), product_1, product_1.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), 22000.f, 5.f, glm::distance(layout.clouds[c].label.getCenter(), layout.clouds[c].products[label_connection].placement.getCenter()));

			}

			for (auto & product_connection : cloud_connections.second) {

				auto & product_1 = products_by_cloud[c][product_connection.first];
				auto & product_2 = products_by_cloud[c][product_connection.second];

				physics_system->newSpring(product_1, product_1.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), product_2, product_2.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), 22000.f, 5.f, glm::distance(layout.clouds[c].products[product_connection.first].placement.getCenter(), layout.clouds[c].products[product_connection.second].placement.getCenter()));
				physics_system->newSpring(anchor_handle, anchor_handle.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), product_1, product_1.get_fast<Entity>()->getComponent<RigidBodyComponent2d>()->getLocalCenter(), 22000.f, 5.f, glm::distance(cloud_center, layout.clouds[c].products[product_connection.first].placement.getCenter()));

			}
			++c;
		}

	}
	RenderSystem& renderSystem = *getScene()->getSystem<RenderSystem>();
	uint32_t render_target = renderSystem.getMainRenderTarget();
	uint32_t rect_mesh_id = renderSystem.getMeshId("product_rect");
	mBG = getScene()->newEntity("EngageBG");

	{
		uint32_t material_id = renderSystem.getMaterialId("BGTexturePoolMaterial");

		Entity& engage_bg = *mBG.get_fast<Entity>();

		engage_bg.setPosition(ci::vec3(ci::app::getWindowCenter(), -10));
		ci::vec3 size = ci::vec3(ci::app::getWindowSize(), 1);
		engage_bg.setLocalScale(size);
		engage_bg.createComponent<RenderComponent>(RenderTechnique(rect_mesh_id).pass<TexturePoolMaterial>(render_target, material_id, 2, ci::vec4(1., 1., 1., 1.)));
	}

	//{
	//	auto wall_handle = getScene()->newEntity("right_wall");
	//	auto wall = wall_handle.get_fast<Entity>();
	//	wall->createComponent<RigidBodyComponent2d>(RigidBodyComponent2d::Format().rectShape().type(b2_staticBody).friction(.0).restitution(.2).density(1));
	//	wall->setScale(vec3(10, ci::app::getWindowHeight(), 1));
	//	wall->setPosition(vec3(ci::app::getWindowWidth() - 10, app::getWindowCenter().y, 0));
	//	mHandles.push_back(wall_handle);
	//}
	//{
	//	auto wall_handle = getScene()->newEntity("left_wall");
	//	auto wall = wall_handle.get_fast<Entity>();
	//	wall->createComponent<RigidBodyComponent2d>(RigidBodyComponent2d::Format().rectShape().type(b2_staticBody).friction(.0).restitution(.2).density(1));
	//	wall->setScale(vec3(10, ci::app::getWindowHeight(), 1));
	//	wall->setPosition(vec3(10, app::getWindowCenter().y, 0));
	//	mHandles.push_back(wall_handle);
	//}
	//{
	//	auto wall_handle = getScene()->newEntity("sub-cats1");
	//	auto wall = wall_handle.get_fast<Entity>();
	//	wall->createComponent<RigidBodyComponent2d>(RigidBodyComponent2d::Format().rectShape().type(b2_staticBody).friction(.1).restitution(.2).density(1));
	//	wall->setScale(vec3(10, ci::app::getWindowHeight(), 1));
	//	wall->setPosition(vec3(10, app::getWindowCenter().y, 0));
	//	mHandles.push_back(wall_handle);
	//}

}

void EngageState::exit()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &EngageState::onClickOut), ClickOutEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &EngageState::onProductClick), ProductClickEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &EngageState::onRestExperience), PresenceDetectionEvent::TYPE);

}

Handle EngageState::composeProductEntity(const Json::Value & data, const ci::Rectf & layout)
{
	RenderSystem& renderSystem = *getScene()->getSystem<RenderSystem>();
	AnimationSystem& animation_system = *getScene()->getSystem<AnimationSystem>();
	uint32_t color_material_id = renderSystem.getMaterialId("ColorMaterial");
	uint32_t texture_pool_material_id = renderSystem.getMaterialId("ProductTexturePoolMaterial");
	uint32_t render_target = renderSystem.getMainRenderTarget();
	uint32_t rect_mesh_id = renderSystem.getMeshId("product_rect");
	TextSystem& textSystem = *getScene()->getSystem<TextSystem>();
	uint32_t font_id = textSystem.getFontManager().getIndex("Target-Light-15");

	std::shared_ptr<IWInitializer> initializer = getScene()->getInitializer<IWInitializer>();


	auto name = data["name"].asString();
	auto product_handle = getScene()->newEntity(name);

	{
		auto & product = *product_handle.get_fast<Entity>();
		auto size = ci::vec3(layout.getSize(), 1);
		product.setLocalScale(size);
		product.setPosition(vec3(layout.getCenter(), 0));

		Handle pcHandle = product.createComponent<ProductComponent>(data);
		auto productComponent = pcHandle.get_fast<ProductComponent>();

		auto physics = product.createComponent<RigidBodyComponent2d>( RigidBodyComponent2d::Format().rectShape().type(b2BodyType::b2_staticBody).dynamicStartingPosition(layout.getCenter()).fixedRotation(true).density(5.f).friction(0.f).restitution(.01f).linearDamping(2.) );

		animation_system.newAnimation(.5f, Animation::Format().finishFn([physics](double duration) {
			
			if (auto p = physics.get<RigidBodyComponent2d>()) {
				p->setBodyType(b2BodyType::b2_dynamicBody);
			}
		
		}));

		product.createComponent<AnimationComponent>();
		Handle ui_handle = product.createComponent<UIComponent>(rect_mesh_id, 0, InputType::CLICK);

		ui_handle.get<UIComponent>()->connectHandler(InputType::CLICK, InputAction::TRIGGER, [product_handle](const InputSample& sample) {			
			auto eventManager = Engine::get()->scene()->getEventManager();
			eventManager->queueEvent( ProductClickEvent::create(product_handle) );
		});

	}

	{
		Handle image_handle = getScene()->newEntity(name + "::image");
		Entity& image = *image_handle.get_fast<Entity>();

		Entity& product = *product_handle.get_fast<Entity>();
		auto productComponent = product.getComponent<ProductComponent>();

		image.setLocalScale(ci::vec3( layout.getSize().x*.9f, layout.getSize().x*.9f, 1) );
		image.createComponent<RenderComponent>(RenderTechnique(rect_mesh_id).pass<TexturePoolMaterial>(render_target, texture_pool_material_id, initializer->getProductTextureId(productComponent->getSku()), ci::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		image.setPosition(ci::vec3(0, -(layout.getSize().y-layout.getSize().x)*.5f, 0));

	
		product.addChild(image_handle);
	}


	{
		Handle text_box_handle = getScene()->newEntity(name + "::text_box");
		Entity& text_box = *text_box_handle.get_fast<Entity>();

		Entity& product = *product_handle.get_fast<Entity>();

		//text_box.setScale(ci::vec3(1) / product.getScale());
		text_box.setPosition(ci::vec3(-layout.getSize().x*.05f, layout.getSize().x*.8f, 2));
		text_box.setLocalRotation(glm::toQuat(ci::rotate((float)M_PI, ci::vec3(1, 0, 0))));
		text_box.createComponent<TextComponent>(product.getName(), font_id, ci::ColorAf(1., 1., 1., 1.), ci::Rectf(ci::vec2(0), ci::vec2(layout.getWidth())), ci::vec2(0), SdfText::DrawOptions().alignment(SdfText::Alignment::CENTER).leading(-10));

		product.addChild(text_box_handle);
	}

	return product_handle;
}

Handle EngageState::composeCloudLabel(const std::string & label_text, const ci::Rectf & layout)
{

	RenderSystem& renderSystem = *getScene()->getSystem<RenderSystem>();
	AnimationSystem& animation_system = *getScene()->getSystem<AnimationSystem>();
	uint32_t color_material_id = renderSystem.getMaterialId("ColorMaterial");
	uint32_t render_target = renderSystem.getMainRenderTarget();
	uint32_t rect_mesh_id = renderSystem.getMeshId("unit_rect");
	TextSystem& textSystem = *getScene()->getSystem<TextSystem>();
	uint32_t font_id = textSystem.getFontManager().getIndex("Target-Medium-45");

	auto label_handle = getScene()->newEntity(label_text + "::label_container");
	auto label = label_handle.get_fast<Entity>();
	//label->createComponent<RenderComponent>(RenderTechnique(rect_mesh_id).pass<ColorMaterial>(render_target, color_material_id, ci::vec4(.5f, .5f, .5f, 1.f)));
	auto physics = label->createComponent<RigidBodyComponent2d>(RigidBodyComponent2d::Format().rectShape().type(b2BodyType::b2_staticBody).fixedRotation(true).density(1.f).friction(1.f).restitution(.01f).linearDamping(2.));
	label->setLocalScale(ci::vec3(layout.getSize(), 1));
	label->setPosition(vec3(layout.getCenter(), 0));

	animation_system.newAnimation(.5f, Animation::Format().finishFn([physics](double duration) {

		if (auto p = physics.get<RigidBodyComponent2d>()) {
			p->setBodyType(b2BodyType::b2_dynamicBody);
		}

	}));

	auto label_text_handle = getScene()->newEntity(label_text + "::label_text");
	auto label_text_entity = label_text_handle.get_fast<Entity>();
	label_text_entity->createComponent<TextComponent>(label_text, font_id, ci::ColorAf(1., 1., 1., 1.), ci::Rectf(ci::vec2(0), layout.getSize()), ci::vec2(0), SdfText::DrawOptions().alignment(SdfText::Alignment::LEFT).leading(-10).clipHorizontal(false).clipVertical(false));
	label_text_entity->setRotation(glm::toQuat(ci::rotate((float)M_PI, ci::vec3(1, 0, 0))));
	label_text_entity->setPosition(ci::vec3(0, 0, -2));
	label->addChild(label_text_handle);

	return label_handle;

}

ISceneLogicStateRef EngageState::nextLogicState()
{
	switch (mNextState) {
	case IWState::APPROACH_STATE:
	{
		clearLayout();
		return ApproachState::create(getScene());
	}break;
	case IWState::PDP_STATE:
	{
		return PDPState::create( getScene(), mSubcategory, mSelectedProduct );
	}break;
	case IWState::FAR_AWAY_STATE:
	{
		clearLayout();
		return FarAwayState::create(getScene());
	}break;
	default: CI_LOG_E("No proper State selected"); return nullptr; break;
	}
}

void EngageState::clearLayout()
{
	auto init = getScene()->getInitializer<IWInitializer>();

	mBG.destroy();

	for (auto & handle : mHandles) {
		handle.destroy();
	}
	mHandles.clear();

	for (auto& handle : init->getEngageLayout() ) {
		auto name = handle.get_fast<Entity>()->getName();
		if (!handle.destroy()) {
			CI_LOG_E("couldn't destroy handle: " << name);
		}
	}
	init->getEngageLayout().clear();
}

void EngageState::onClickOut( EventDataRef eventData )
{
	mNextState = IWState::APPROACH_STATE;
	//close sub-category
	//end of transition, do...
	getScene()->requestNextLogicState();
}

void EngageState::onProductClick(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<ProductClickEvent>(eventData)) {
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

void EngageState::onRestExperience(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {
		if (!event->isPersonPresent()) {
			mNextState = IWState::FAR_AWAY_STATE;
			getScene()->requestNextLogicState();
		}
	}
}
