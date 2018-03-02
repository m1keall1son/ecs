#include "ApproachState.h"
#include "EngageState.h"
#include "FarAwayState.h"
#include "ecs/engine/Engine.h"
#include "IWEvents.h"
#include "IWStates.h"
#include "IWInitializer.h"
#include "Ecs.h"

using namespace ci;

void ApproachState::update()
{
}

void ApproachState::enter()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener( fastdelegate::MakeDelegate( this, &ApproachState::onSubCategoryClick ), SubCategoryClickEvent::TYPE );
	eventManager->addListener(fastdelegate::MakeDelegate(this, &ApproachState::onResetExperience), PresenceDetectionEvent::TYPE);

	RenderSystem& renderSystem = *getScene()->getSystem<RenderSystem>();
	TaxonomySystem& taxonomySystem = *getScene()->getSystem<TaxonomySystem>();
	TextSystem& textSystem = *getScene()->getSystem<TextSystem>();
	InputSystem& inputSystem = *getScene()->getSystem<InputSystem>();

	std::shared_ptr<IWInitializer> initializer = getScene()->getInitializer<IWInitializer>();

	uint32_t render_target = renderSystem.getMainRenderTarget();
	uint32_t material_id = renderSystem.getMaterialId("BGTexturePoolMaterial");
	uint32_t rect_mesh_id = renderSystem.getMeshId("product_rect");

	auto bg_handle = getScene()->newEntity("ApproachBG");

	{
		Entity& approach_bg = *bg_handle.get_fast<Entity>();

		approach_bg.setPosition(ci::vec3(ci::app::getWindowCenter(), -5));
		ci::vec3 size = ci::vec3(ci::app::getWindowSize(), 1 );
		approach_bg.setLocalScale(size);
		approach_bg.createComponent<RenderComponent>(RenderTechnique(rect_mesh_id).pass<TexturePoolMaterial>(render_target, material_id, 1, ci::vec4(1., 1., 1., 1.)));
	}
	mHandles.push_back(bg_handle);

	float w = ci::app::getWindowWidth() / 25.f;
	int cols = ci::app::getWindowWidth() / w;

	int i = 0;
	for (auto subcategory : initializer->getSubCategoryMap()) {
		//create top level entity for product
		std::string name = subcategory.second["name"].asString();
		Handle subcategory_handle = getScene()->newEntity(name);
		mHandles.push_back(subcategory_handle);

		std::printf(" \n Approach State: Showing SubCategory %s \n \n", name.c_str());

		{
			Entity& subcat = *subcategory_handle.get_fast<Entity>();

			uint32_t color_material_id = renderSystem.getMaterialId("ColorMaterial");
			uint32_t render_target = renderSystem.getMainRenderTarget();

			uint32_t mesh_id = renderSystem.getMeshId("unit_rect");

			subcat.setScale(vec3(460, 400, 1));
			subcat.setPosition(vec3(1365, 2180, 0) + vec3(vec2(subcat.getWorldScale()*.5f), 0.));

			subcat.createComponent<SubcategoryComponent>(subcategory.second);
			Handle ui_handle = subcat.createComponent<UIComponent>(mesh_id, 0, InputType::CLICK);

			ui_handle.get<UIComponent>()->connectHandler(InputType::CLICK, InputAction::TRIGGER, [subcategory_handle](const InputSample& sample) {
				auto eventManager = Engine::get()->scene()->getEventManager();
				eventManager->queueEvent(SubCategoryClickEvent::create(subcategory_handle));
			});

		}

		i++;
	}

	Handle bg_button_handle = getScene()->newEntity("background");
	mHandles.push_back(bg_button_handle);

	{
		Entity& bg_rect = *bg_button_handle.get_fast<Entity>();

		bg_rect.setPosition(ci::vec3(0));
		bg_rect.setScale(ci::vec3(ci::app::getWindowSize(), 0));

		Handle ui_handle = bg_rect.createComponent<UIComponent>(rect_mesh_id, -1, InputType::CLICK);

		ui_handle.get<UIComponent>()->connectHandler(InputType::CLICK, InputAction::TRIGGER, [=](const InputSample& sample) {
			mNextState = IWState::FAR_AWAY_STATE;
			getScene()->requestNextLogicState();
		});
	}
}

void ApproachState::exit()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate( this, &ApproachState::onSubCategoryClick), SubCategoryClickEvent::TYPE );
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &ApproachState::onResetExperience), PresenceDetectionEvent::TYPE);

	for (auto& handle : mHandles) {
		handle.destroy();
	}
	mHandles.clear();

	// mDetector->clearOnDetectionFns();

}

ISceneLogicStateRef ApproachState::nextLogicState()
{
	switch (mNextState) {
	case IWState::ENGAGE_STATE:
	{
		return EngageState::create(getScene(), IWState::APPROACH_STATE, mSelectedSubCategory);
	}break;
	case IWState::FAR_AWAY_STATE:
	{
		return FarAwayState::create(getScene());
	}break;
	default: CI_LOG_E("No proper State selected"); return nullptr; break;
	}
}

void ApproachState::onSubCategoryClick(const EventDataRef eventData)
{
	if ( auto event = std::dynamic_pointer_cast<SubCategoryClickEvent>(eventData) ) {

		if (auto sub_cat_comp = event->getSubCategory().get_fast<Entity>()->getComponent<SubcategoryComponent>())
		{
			mSelectedSubCategory = atoi(sub_cat_comp->getUUID().c_str());

			mNextState = IWState::ENGAGE_STATE;

			//transition, move all subcategories around and clean up visual state
			//get initializer
			//get all subcategories and manipulate them
			//at the end of that call
			getScene()->requestNextLogicState();
		}
		else {
			CI_LOG_E("event did not contain a subcategory handle!");
		}
	}	else {
		CI_LOG_E("event was not of type: SubCategoryClickEvent");
	}
}

void ApproachState::onResetExperience(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {
		if (!event->isPersonPresent()) {
			mNextState = IWState::FAR_AWAY_STATE;
			getScene()->requestNextLogicState();
		}
	}
}
