#include "DemoInit.h"
#include "ECS.h"
#include "DemoFarAway.h"
#include "../IWScene/IWEvents.h"
#include "DemoEvents.h"
#include "VideoMaterial.h"
#include "ecs/rendering/materials/TextureMaterial.h"

using namespace ci;

void DemoInit::load(std::atomic<float>* progress, float progress_step)
{
	
	auto eventManager = Engine::get()->scene()->getEventManager();
	eventManager->addListener(fastdelegate::MakeDelegate(this, &DemoInit::onCollection), CollectionEvent::TYPE);

	//get the default color material
	auto& render_system = *getScene()->getSystem<RenderSystem>();

	auto color_material_id = render_system.getMaterialId("ColorMaterial");

	auto token_tex = gl::Texture::create(ci::loadImage(app::loadAsset("images/demo/token.png")), gl::Texture2d::Format().mipmap().minFilter(GL_LINEAR_MIPMAP_LINEAR));
	render_system.registerMaterial("TokenMaterial", TextureMaterial::create( token_tex, true ) );

	//create product rect
	auto src = geom::Rect(Rectf(vec2(-.5), vec2(.5))) >> geom::AttribFn<vec2, vec2>(geom::Attrib::TEX_COORD_0, [](ci::vec2 tc) {
		tc.y = 1. - tc.y;
		return tc;
	});
	auto product_rect = geom::SourceRef(src.clone());
	auto mesh_id = render_system.registerMesh("flipped_rect", product_rect);
	auto circle_src = std::make_shared<geom::Circle>();
	circle_src->subdivisions(32);
	render_system.registerMesh( "unit_cirlce", circle_src );

	auto texture_pool_id = render_system.registerMaterial("VisualStateTexturePool", TexturePoolMaterial::create(false));
	auto texturePool = std::dynamic_pointer_cast<TexturePoolMaterial>(render_system.retrieveMaterial(texture_pool_id));

	ci::Surface far_away_image = ci::loadImage(ci::app::loadAsset("images/demo/frozenDesserts_s1.jpg"));
	ci::Surface approach_image = ci::loadImage(ci::app::loadAsset("images/demo/frozenDesserts_s2.jpg"));
	ci::Surface engage_image = ci::loadImage(ci::app::loadAsset("images/demo/03_161216-frozenDesserts-engage-Final.jpg"));
	ci::Surface pdp_1 = ci::loadImage(ci::app::loadAsset("images/demo/10_161216-frozenDesserts-engage_Americone_Dream_PDP_V2.png"));
	ci::Surface pdp_2 = ci::loadImage(ci::app::loadAsset("images/demo/11_frozenDesserts_pdp.jpg"));
	ci::Surface pdp_main_middle_size = ci::loadImage(ci::app::loadAsset("images/demo/04_PDP Frozen Dessert Default Vanilla Default.jpg"));
	ci::Surface pdp_main_back = ci::loadImage(ci::app::loadAsset("images/demo/05_PDP Frozen Dessert Default Vanilla - Back.jpg"));

	ci::Surface pdp_main_left_size = ci::loadImage(ci::app::loadAsset("images/demo/08_PDP Frozen Dessert Default Vanilla - Size 1.jpg"));
	ci::Surface pdp_main_right_size = ci::loadImage(ci::app::loadAsset("images/demo/09_PDP Frozen Dessert Default Vanilla - Size 2.jpg"));

	ci::Surface pdp_main_product_info_1 = ci::loadImage(ci::app::loadAsset("images/demo/06_PDP Frozen Dessert Default Vanilla - Product Info 1.jpg"));
	ci::Surface pdp_main_product_info_2 = ci::loadImage(ci::app::loadAsset("images/demo/07_PDP Frozen Dessert Default Vanilla - Product Info 2.jpg"));

	auto state_0 = texturePool->addSurface(std::move(far_away_image));
	auto state_1 = texturePool->addSurface(std::move(approach_image));
	auto state_2 = texturePool->addSurface(std::move(engage_image));
	auto state_3 = texturePool->addSurface(std::move(pdp_1));
	auto state_4 = texturePool->addSurface(std::move(pdp_2));
	auto state_5 = texturePool->addSurface(std::move(pdp_main_middle_size));
	auto state_6 = texturePool->addSurface(std::move(pdp_main_back));
	auto state_7 = texturePool->addSurface(std::move(pdp_main_left_size));
	auto state_8 = texturePool->addSurface(std::move(pdp_main_right_size));
	auto state_9 = texturePool->addSurface(std::move(pdp_main_product_info_1));
	auto state_10 = texturePool->addSurface(std::move(pdp_main_product_info_2));

	mVisualStateMap.insert({ "FarAway", state_0 });
	mVisualStateMap.insert({ "Approach", state_1 });
	mVisualStateMap.insert({ "Engage", state_2 });

	mVisualStateMap.insert({ "PDP1", state_3 });
	mVisualStateMap.insert({ "PDP2", state_4 });

	mVisualStateMap.insert({ "PDPMain-Front", state_5 });
	mVisualStateMap.insert({ "PDPMain-Back", state_6 });
	mVisualStateMap.insert({ "PDPMain-size1", state_7 });
	mVisualStateMap.insert({ "PDPMain-size2", state_8 });
	mVisualStateMap.insert({ "PDPMain-info1", state_9 });
	mVisualStateMap.insert({ "PDPMain-info2", state_10 });

	mTokenButtonPositions.insert({ state_3, vec2(1478,2299) });
	mTokenButtonPositions.insert({ state_4, vec2(1356,2149) });
	mTokenButtonPositions.insert({ state_5, vec2(1270,1440) });
	mTokenButtonPositions.insert({ state_6, vec2(1270,1440) });
	mTokenButtonPositions.insert({ state_7, vec2(1270,1440) });
	mTokenButtonPositions.insert({ state_8, vec2(1270,1440) });
	mTokenButtonPositions.insert({ state_9, vec2(1270,1440) });
	mTokenButtonPositions.insert({ state_10, vec2(1270,1440) });

	mTokenScale = vec2(50);

	{
		mTokenWall = getScene()->newEntity("TokenWall");
		auto wall = mTokenWall.get_fast<Entity>();
		wall->createComponent<RigidBodyComponent2d>(RigidBodyComponent2d::Format().rectShape().type(b2_staticBody).friction(.1).restitution(.2).density(1));
		wall->setScale(vec3(10,ci::app::getWindowHeight(), 1));
		wall->setPosition(vec3( ci::app::getWindowWidth()-10, app::getWindowCenter().y, 0));
	}
	mVisualState = getScene()->newEntity("VisualState");
	auto visual = mVisualState.get_fast<Entity>();
	auto render = visual->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<TexturePoolMaterial>(0, texture_pool_id, getVisualState("Approach"), ci::vec4(1.f, 1.f, 1.f, 1.f)));
	render.get_fast<RenderComponent>()->setVisible(false);
	visual->setScale(vec3(app::getWindowSize(), 1));
	visual->setPosition(vec3(app::getWindowCenter(), 0));

	{
		mBackButton = getScene()->newEntity("BackButton");
		auto back = mBackButton.get_fast<Entity>();
		back->setScale(vec3(app::getWindowSize(), 1));
		back->setPosition(vec3(app::getWindowCenter(), 0));
		auto ui_handle = back->createComponent<UIComponent>(mesh_id, -1, InputType::CLICK);
		auto ui = ui_handle.get_fast<UIComponent>();
		ui->connectClickHandler([](const InputSample& sample) { 
			CI_LOG_V("BackButton clicked.");
			auto eventManager = Engine::get()->scene()->getEventManager();
			eventManager->queueEvent(ClickOutEvent::create());
		});
	}

	{
		//1300, 500
		//160, 250
		mEngageProducts[0] = getScene()->newEntity("product1");
		auto product_1 = mEngageProducts[0].get_fast<Entity>();

		product_1->setScale(vec3(160, 250, 1));
		product_1->setPosition( vec3(vec2(1300,500) + vec2( product_1->getWorldScale()*.5f ), 0) );
		auto ui_handle = product_1->createComponent<UIComponent>(mesh_id, 1, InputType::CLICK);

		auto ui = ui_handle.get_fast<UIComponent>();
		ui->setEnabled(false);
		ui->connectClickHandler([&](const InputSample& sample) { 
			auto eventManager = Engine::get()->scene()->getEventManager();
			eventManager->queueEvent(EngageClickEvent::create( getVisualState("PDPMain-Front"), mEngageProducts[0]));
		});

	}
	{
		//1520,1824
		//180, 290
		mEngageProducts[1] = getScene()->newEntity("product2");
		auto product_2 = mEngageProducts[1].get_fast<Entity>();

		product_2->setScale(vec3(180, 290, 1));
		product_2->setPosition(vec3(vec2(1477, 1701) + vec2(product_2->getWorldScale()*.5f), 0));
		auto ui_handle = product_2->createComponent<UIComponent>(mesh_id, 1, InputType::CLICK);

		auto ui = ui_handle.get_fast<UIComponent>();
		ui->setEnabled(false);
		ui->connectClickHandler([&](const InputSample& sample) {
			auto eventManager = Engine::get()->scene()->getEventManager();
			eventManager->queueEvent(EngageClickEvent::create( getVisualState("PDP1"), mEngageProducts[1]));
		});

	}
	{
		mEngageProducts[2] = getScene()->newEntity("product3");
		auto product_3 = mEngageProducts[2].get_fast<Entity>();

		product_3->setScale( vec3(150, 200, 1) );
		product_3->setPosition(vec3(vec2(1398, 2454) + vec2(product_3->getWorldScale()*.5f), 0));
		auto ui_handle = product_3->createComponent<UIComponent>(mesh_id, 1, InputType::CLICK);

		auto ui = ui_handle.get_fast<UIComponent>();
		ui->setEnabled(false);
		ui->connectClickHandler([&](const InputSample& sample) { 
			auto eventManager = Engine::get()->scene()->getEventManager();
			eventManager->queueEvent(EngageClickEvent::create( getVisualState("PDP2"), mEngageProducts[2]));
		});

	}

	mFarAway = getScene()->newEntity("FarAway");
	auto far_away = mFarAway.get_fast<Entity>();
	far_away->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<TexturePoolMaterial>(0, texture_pool_id, getVisualState("FarAway"), ci::vec4(1.f, 1.f, 1.f, 1.f)));
	far_away->setScale(vec3(app::getWindowSize(), 1));
	far_away->setPosition(vec3(app::getWindowCenter(), 10));


	mKeyPress = ci::app::getWindow()->getSignalKeyDown().connect([&, mesh_id](ci::app::KeyEvent event) {
	
		auto& render_system = *getScene()->getSystem<RenderSystem>();
		auto video_material_id = render_system.getMaterialId("FarAwayVideoMaterial");
		auto video_material_id2 = render_system.getMaterialId("FarAwayVideoMaterial2");

		auto video_mat = std::dynamic_pointer_cast<VideoMaterial>(render_system.retrieveMaterial(video_material_id));
		auto video_mat2 = std::dynamic_pointer_cast<VideoMaterial>(render_system.retrieveMaterial(video_material_id2));

		switch (event.getCode())
		{
		case app::KeyEvent::KEY_v:
		{

			static bool pressed = false;
			if (!pressed) {
				mVideo = getScene()->newEntity("Video");
				auto video = mVideo.get_fast<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>(0, video_material_id));
				video->setScale(vec3(app::getWindowSize(), 1));
				video->setPosition(vec3(app::getWindowCenter(), 20));
				video_mat->play();

			}else
			{
				video_mat2->pause();
				video_mat->pause();
				mVideo.destroy();
			}
			pressed = !pressed;
		}break;
		case app::KeyEvent::KEY_1:
		{
			video_mat2->pause();

			if (mVideo.isIntialized()) {
				video_mat->loadVideo("images/demo/video/Katamari_Fruits&Veggies_Far_Animation_V1_[FWD-BWD-Loop].mp4");
				video_mat->play();
				auto video = mVideo.get_fast<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>(0, video_material_id));
			}
		}break;
		case app::KeyEvent::KEY_2:
		{
			video_mat2->pause();

			if (mVideo.isIntialized()){
				video_mat->loadVideo("images/demo/video/Katamari_Dairy_Far_Animation_V1_[FWD-BWD-Loop].mp4");
				video_mat->play();
				auto video = mVideo.get_fast<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>(0, video_material_id));
			}
		}break;
		case app::KeyEvent::KEY_3:
		{

			video_mat2->pause();

			if (mVideo.isIntialized()){
				video_mat->loadVideo("images/demo/video/Katamari_FrozenDessert_Far_Animation_V1_[FWD-BWD-Loop].mp4");
				video_mat->play();
				auto video = mVideo.get_fast<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>(0, video_material_id));
			}
		}break;
		case app::KeyEvent::KEY_4:
		{
			video_mat2->pause();
			if (mVideo.isIntialized()) {
				video_mat->loadVideo("images/demo/video/Katamari_HomeEssentials_Far_Animation_V1_[FWD-BWD-Loop].mp4");
				video_mat->play();
				auto video = mVideo.get_fast<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>(0, video_material_id));
			}
		}break;
		case app::KeyEvent::KEY_5:
		{
			if (mVideo.isIntialized()){
				video_mat2->play();
				auto video = mVideo.get<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>( 0, video_material_id2 ));
			}
		}break;
		case app::KeyEvent::KEY_6:
		{
			video_mat2->pause();
			if (mVideo.isIntialized()) {
				video_mat->loadVideo("images/demo/video/_dairy.mp4");
				video_mat->play();
				auto video = mVideo.get_fast<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>(0, video_material_id));
			}
		}break;
		case app::KeyEvent::KEY_7:
		{
			video_mat2->pause();
			if (mVideo.isIntialized()) {
				video_mat->loadVideo("images/demo/video/_homeEssentials.mp4");
				video_mat->play();
				auto video = mVideo.get_fast<Entity>();
				video->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<VideoMaterial>(0, video_material_id));
			}
		}break;
		case app::KeyEvent::KEY_b:
		{
			if (mVideo.isIntialized()) {
				video_mat->play();
			}
		}break;
		default:
			break;
		}
	
	});


}

void DemoInit::init()
{
	//WMF player has to be created on the main thread...whatever
	auto& render_system = *getScene()->getSystem<RenderSystem>();
	render_system.registerMaterial("FarAwayVideoMaterial", VideoMaterial::create( "images/demo/video/Katamari_Fruits&Veggies_Far_Animation_V1_[FWD-BWD-Loop].mp4" ));
	render_system.registerMaterial("FarAwayVideoMaterial2", VideoMaterial::create("images/demo/video/Isometric Frozen Desserts Animation.mp4"));

}

ISceneLogicStateRef DemoInit::beginLogic()
{
	return DemoFarAway::create(getScene());
}

Handle DemoInit::createToken(const ci::vec2 & position)
{
	auto& render_system = *getScene()->getSystem<RenderSystem>();
	auto& animation_system = *getScene()->getSystem<AnimationSystem>();
	auto token_material_id = render_system.getMaterialId("TokenMaterial");
	auto mesh_id = render_system.getMeshId("unit_cirlce");

	auto handle = getScene()->newEntity("Token_"+std::to_string(mTokens.size()));

	auto token = handle.get_fast<Entity>();
	auto render_handle = token->createComponent<RenderComponent>( RenderTechnique(mesh_id).pass<TextureMaterial>( 0, token_material_id ) );
	token->setScale(vec3(mTokenScale, 1));	
	token->setPosition(vec3(position, 3));
	auto rigid = token->createComponent<RigidBodyComponent2d>( RigidBodyComponent2d::Format().circleShape().type(b2_staticBody).density(1.).fixedRotation(true).friction(.1).restitution(.3) );

	//todo why do i have to do this??
	animation_system.newAnimation(.05, Animation::Format().finishFn([rigid](double percent) { 
		if (auto rb = rigid.get<RigidBodyComponent2d>())
		{
			rb->setBodyType(b2_dynamicBody);
		}
	}));

	auto& physics_system = *getScene()->getSystem<PhysicsSystem2d>();
	physics_system.newSpring( handle, vec2(0), mTokenWall, vec2(0, 375), 1300, 1, 10 );

	mTokens.push_back(handle);

	return handle;
}

void DemoInit::clearTokens()
{
	for ( auto & token : mTokens ) {
		token.destroy();
	}
	mTokens.clear();
}

DemoInit::~DemoInit()
{
	mKeyPress.disconnect();
}

void DemoInit::onCollection(EventDataRef)
{
	auto& animation_system = *getScene()->getSystem<AnimationSystem>();

	animation_system.newAnimation(.25f, Animation::Format().updateFn([&](double percent) {
		for ( auto & token : mTokens ) {
			auto t = token.get_fast<Entity>();
			t->setScale( vec3( mTokenScale*(1.f-(float)percent), 1. ) );
		}
	}).finishFn([&]( double percent ) {
		for (auto & token : mTokens) {
			token.destroy();
		}
		mTokens.clear();
	}));

}
