#include "DemoPDP.h"
#include "DemoEngage.h"
#include "ECS.h"
#include "../IWScene/IWEvents.h"
#include "DemoFarAway.h"
#include "DemoApproach.h"
#include "DemoEvents.h"
#include "DemoInit.h"

using namespace ci;

void DemoPDP::update()
{
}

void DemoPDP::enter()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoPDP::onClickOut), ClickOutEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoPDP::onEngageClick), EngageClickEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoPDP::onPresenceDetection), PresenceDetectionEvent::TYPE);
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoPDP::onCollection), CollectionEvent::TYPE);

	{
		auto product = mPDP.get_fast<Entity>();
		auto ui = product->getComponent<UIComponent>();
		ui->setEnabled(false);
	}

	auto& render_system = *getScene()->getSystem<RenderSystem>();
	auto texture_pool_id = render_system.getMaterialId("VisualStateTexturePool");

	auto init = getScene()->getInitializer<DemoInit>();
	auto visual_state = init->getVisualState();

	{
		auto product = visual_state.get_fast<Entity>();
		auto render = product->getComponent<RenderComponent>();
		auto material = render->getMaterialInstanceHandle(texture_pool_id);
		material.get_fast<TexturePoolMaterialInstance>()->setTextureId(mSubState);
	}

	auto mesh_id = render_system.getMeshId( "unit_rect" );
	auto color_material_id = render_system.getMaterialId("ColorMaterial");

	{

		mTokenButton = getScene()->newEntity("TokenButton");
		auto token_button = mTokenButton.get_fast<Entity>();
		token_button->setScale(vec3(145, 145, 1));
		token_button->setPosition(vec3( init->getTokenButtonPosition(mSubState) + vec2(145)*.5f , 0));
		auto ui_handle = token_button->createComponent<UIComponent>(mesh_id, 1, InputType::CLICK);
		auto ui = ui_handle.get_fast<UIComponent>();
		ui->connectClickHandler([init](const InputSample& sample) {
			init->createToken(sample.getPosition());
		});
	}

	switch (mSubState)
	{
	case 3: {

		//american dream
		{
			auto pdp1 = init->getEngageProducts()[0];
			auto product = pdp1.get_fast<Entity>();
			product->setPosition( vec3( vec2( 1135, 405 ) + vec2(product->getWorldScale())*.5f , 0. ) );
		}
		{
			auto pdp3 = init->getEngageProducts()[2];
			auto product = pdp3.get_fast<Entity>();
			product->setPosition(vec3(vec2(1380, 2550) + vec2(product->getWorldScale())*.5f, 0.));
		}
	}break;
	case 4: {

		//vanilla bean

		{
			auto pdp1 = init->getEngageProducts()[0];
			auto product = pdp1.get_fast<Entity>();
			product->setPosition(vec3(vec2(1300, 360) + vec2(product->getWorldScale())*.5f, 0.));
		}
		{
			auto pdp2 = init->getEngageProducts()[1];
			auto product = pdp2.get_fast<Entity>();
			product->setPosition(vec3(vec2(1480, 1575) + vec2(product->getWorldScale())*.5f, 0.));
		}
	}break;
	case 5:
	{
		//vanilla

		{
			auto pdp2 = init->getEngageProducts()[1];
			auto product = pdp2.get_fast<Entity>();
			product->setPosition(vec3(vec2(1530, 1830) + vec2(product->getWorldScale())*.5f, 0.));
		}
		{
			auto pdp3 = init->getEngageProducts()[2];
			auto product = pdp3.get_fast<Entity>();
			product->setPosition(vec3(vec2(1390, 2575) + vec2(product->getWorldScale())*.5f, 0.));
		}

		mPdpLayout.push_back( newButton("ProductBack", "PDPMain-Back", vec2(1250, 915), vec2(200, 60) ) );
		mPdpLayout.push_back( newButton("3.6oz", "PDPMain-size1", vec2(1195, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("14oz", "PDPMain-size1", vec2(1299, 1380), vec2(100, 50), true));
		mPdpLayout.push_back(newButton("28oz", "PDPMain-size2", vec2(1403, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("product-info", "PDPMain-info1", vec2(1195-70, 1380-125), vec2(400, 100)));

	}break;

	case 6:
	{
		//vanilla

		{
			auto pdp2 = init->getEngageProducts()[1];
			auto product = pdp2.get_fast<Entity>();
			product->setPosition(vec3(vec2(1530, 1830) + vec2(product->getWorldScale())*.5f, 0.));
		}
		{
			auto pdp3 = init->getEngageProducts()[2];
			auto product = pdp3.get_fast<Entity>();
			product->setPosition(vec3(vec2(1390, 2575) + vec2(product->getWorldScale())*.5f, 0.));
		}

		mPdpLayout.push_back(newButton("ProductBack", "PDPMain-Front", vec2(1250, 915), vec2(200, 60)));
		mPdpLayout.push_back(newButton("3.6oz", "PDPMain-size1", vec2(1195, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("14oz", "PDPMain-Front", vec2(1299, 1380), vec2(100, 50), true));
		mPdpLayout.push_back(newButton("28oz", "PDPMain-size2", vec2(1403, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("product-info", "PDPMain-info1", vec2(1195 - 70, 1380 - 125), vec2(400, 100)));

	}break;

	case 7:
	{
		//vanilla

		{
			auto pdp2 = init->getEngageProducts()[1];
			auto product = pdp2.get_fast<Entity>();
			product->setPosition(vec3(vec2(1530, 1830) + vec2(product->getWorldScale())*.5f, 0.));
		}
		{
			auto pdp3 = init->getEngageProducts()[2];
			auto product = pdp3.get_fast<Entity>();
			product->setPosition(vec3(vec2(1390, 2575) + vec2(product->getWorldScale())*.5f, 0.));
		}

		mPdpLayout.push_back(newButton("ProductBack", "PDPMain-Front", vec2(1250, 915), vec2(200, 60)));
		mPdpLayout.push_back(newButton("3.6oz", "PDPMain-size1", vec2(1195, 1380), vec2(100, 50), true));
		mPdpLayout.push_back(newButton("14oz", "PDPMain-Front", vec2(1299, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("28oz", "PDPMain-size2", vec2(1403, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("product-info", "PDPMain-info1", vec2(1195 - 70, 1380 - 125), vec2(400, 100)));

	}break;

	case 8:
	{
		//vanilla

		{
			auto pdp2 = init->getEngageProducts()[1];
			auto product = pdp2.get_fast<Entity>();
			product->setPosition(vec3(vec2(1530, 1830) + vec2(product->getWorldScale())*.5f, 0.));
		}
		{
			auto pdp3 = init->getEngageProducts()[2];
			auto product = pdp3.get_fast<Entity>();
			product->setPosition(vec3(vec2(1390, 2575) + vec2(product->getWorldScale())*.5f, 0.));
		}

		mPdpLayout.push_back(newButton("ProductBack", "PDPMain-Front", vec2(1250, 915), vec2(200, 60)));
		mPdpLayout.push_back(newButton("3.6oz", "PDPMain-size1", vec2(1195, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("14oz", "PDPMain-Front", vec2(1299, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("28oz", "PDPMain-size2", vec2(1403, 1380), vec2(100, 50), true));
		mPdpLayout.push_back(newButton("product-info", "PDPMain-info1", vec2(1195 - 70, 1380 - 125), vec2(400, 100)));

	}break;

	case 9:
	{
		//vanilla

		{
			auto pdp2 = init->getEngageProducts()[1];
			auto product = pdp2.get_fast<Entity>();
			product->setPosition(vec3(vec2(1530, 1830) + vec2(product->getWorldScale())*.5f, 0.));
		}
		{
			auto pdp3 = init->getEngageProducts()[2];
			auto product = pdp3.get_fast<Entity>();
			product->setPosition(vec3(vec2(1390, 2575) + vec2(product->getWorldScale())*.5f, 0.));
		}

		mPdpLayout.push_back(newButton("ProductBack", "PDPMain-Front", vec2(1250, 915), vec2(200, 60)));
		mPdpLayout.push_back(newButton("3.6oz", "PDPMain-size1", vec2(1195, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("14oz", "PDPMain-Front", vec2(1299, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("28oz", "PDPMain-size2", vec2(1403, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("product-info", "PDPMain-Front", vec2(1195 - 70, 1380 - 125), vec2(400, 100)));
		mPdpLayout.push_back(newButton("product-info2", "PDPMain-info2", vec2(932, 1380), vec2(100, 50)));


	}break;

	case 10:
	{
		//vanilla

		{
			auto pdp2 = init->getEngageProducts()[1];
			auto product = pdp2.get_fast<Entity>();
			product->setPosition(vec3(vec2(1530, 1830) + vec2(product->getWorldScale())*.5f, 0.));
		}
		{
			auto pdp3 = init->getEngageProducts()[2];
			auto product = pdp3.get_fast<Entity>();
			product->setPosition(vec3(vec2(1390, 2575) + vec2(product->getWorldScale())*.5f, 0.));
		}

		mPdpLayout.push_back(newButton("ProductBack", "PDPMain-Front", vec2(1250, 915), vec2(200, 60)));
		mPdpLayout.push_back(newButton("3.6oz", "PDPMain-size1", vec2(1195, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("14oz", "PDPMain-Front", vec2(1299, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("28oz", "PDPMain-size2", vec2(1403, 1380), vec2(100, 50)));
		mPdpLayout.push_back(newButton("product-info", "PDPMain-Front", vec2(1195 - 70, 1380 - 125), vec2(400, 100)));
		mPdpLayout.push_back(newButton("product-info1", "PDPMain-info1", vec2(932, 1380), vec2(100, 50)));

	}break;

	default: break;
	}

}

void DemoPDP::exit()
{
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoPDP::onClickOut), ClickOutEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoPDP::onEngageClick), EngageClickEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoPDP::onPresenceDetection), PresenceDetectionEvent::TYPE);
	eventManager->removeListener(fastdelegate::MakeDelegate(this, &DemoPDP::onCollection), CollectionEvent ::TYPE);

	mTokenButton.destroy();

	for ( auto & handle : mPdpLayout ) {
		handle.destroy();
	}
	mPdpLayout.clear();

	if (mNextState == 0)
	{
		auto init = getScene()->getInitializer<DemoInit>();
		auto engage_products = init->getEngageProducts();

		{
			auto product = engage_products[0].get_fast<Entity>();
			auto ui = product->getComponent<UIComponent>();
			ui->setEnabled(false);
		}
		{
			auto product = engage_products[1].get_fast<Entity>();
			auto ui = product->getComponent<UIComponent>();
			ui->setEnabled(false);
		}
		{
			auto product = engage_products[2].get_fast<Entity>();
			auto ui = product->getComponent<UIComponent>();
			ui->setEnabled(false);
		}
	}else {
		auto product = mPDP.get_fast<Entity>();
		auto ui = product->getComponent<UIComponent>();
		ui->setEnabled(true);
	}

}

ISceneLogicStateRef DemoPDP::nextLogicState()
{
	switch (mNextState)
	{
	case 0:
		return DemoFarAway::create(getScene());
		break;
	case 1:
		return DemoEngage::create(getScene());
		break;
	case 2:
		return DemoPDP::create(getScene(), mSubState, mClickedProduct );
		break;
	}
}


Handle DemoPDP::newButton(const std::string & name, const std::string & next_state, const ci::vec2 & position, const ci::vec2 & size, bool does_nothing)
{
	auto init = getScene()->getInitializer<DemoInit>();
	auto& render_system = *getScene()->getSystem<RenderSystem>();
	auto mesh_id = render_system.getMeshId("unit_rect");

	auto ent = getScene()->newEntity(name);
	auto button = ent.get_fast<Entity>();
	button->setScale(vec3(size, 1));
	button->setPosition(vec3(position + vec2(button->getScale())*.5f, 0));
	auto ui_handle = button->createComponent<UIComponent>(mesh_id, 1, InputType::CLICK);
	auto ui = ui_handle.get_fast<UIComponent>();
	auto handle = mPDP;
	ui->connectClickHandler([handle, init,next_state, does_nothing](const InputSample& sample) {
		if (!does_nothing) {
			auto eventManager = Engine::get()->scene()->getEventManager();
			eventManager->queueEvent(EngageClickEvent::create( init->getVisualState(next_state), handle));
		}
	});

	return ent;
}

void DemoPDP::onClickOut(EventDataRef)
{
	mNextState = 1;
	getScene()->requestNextLogicState();
}

void DemoPDP::onEngageClick(EventDataRef eventData)
{
	auto event = std::dynamic_pointer_cast<EngageClickEvent>(eventData);
	mNextState = 2;
	mClickedProduct = event->getProduct();
	mSubState = event->getSubState();
	getScene()->requestNextLogicState();
}

void DemoPDP::onPresenceDetection(EventDataRef eventData)
{
	if (auto event = std::dynamic_pointer_cast<PresenceDetectionEvent>(eventData)) {
		if (!event->isPersonPresent()) {
			mNextState = 0;
			getScene()->requestNextLogicState();
		}
	}
}

void DemoPDP::onCollection(EventDataRef)
{
	mNextState = 1;
	getScene()->requestNextLogicState();
}
