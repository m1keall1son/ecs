#include "PhysicsScene.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

void PhysicsScene::touchesBegan(ci::app::TouchEvent event)
{
	for ( auto & t : event.getTouches() ) {
		mTokens.emplace_back(std::make_shared<Token>(mWorld, b2Vec2(t.getPos().x, t.getPos().y), b2Vec2(getWindowWidth() - 40, getWindowCenter().y)));
	}
}

void PhysicsScene::touchesMoved(ci::app::TouchEvent event)
{
}

void PhysicsScene::touchesEnded(ci::app::TouchEvent event)
{
}

void PhysicsScene::mouseDown(ci::app::MouseEvent event)
{
	mTokens.emplace_back(std::make_shared<Token>(mWorld, b2Vec2(event.getPos().x, event.getPos().y), b2Vec2(getWindowWidth() - 40, getWindowCenter().y)));
}

void PhysicsScene::mouseDrag(ci::app::MouseEvent event)
{
}

void PhysicsScene::mouseUp(ci::app::MouseEvent event)
{
}

void PhysicsScene::setup()
{
	mParams = params::InterfaceGl::create(getName(), vec2(200, 200));
	mParams->setPosition(vec2(50, 300));

	mNumClouds = 5;
	mMinProducts = 1;
	mMaxProducts = 16;

	auto generate_layout = [&] {

		mTokens.clear();

		LayoutGenerator generator(getWindowCenter(), mEpicenter, Rectf(vec2(getWindowSize())*.1f, vec2(getWindowSize())*.9f), 30.f  );

		LayoutGenerator::SubcategoryStructure subcat_structure;

		for (int j = 0; j < mNumClouds; j++) {

			LayoutGenerator::CloudStructure cloud_structure;

			auto products = randInt(mMinProducts, mMaxProducts);
			for (int p = 0; p < products; p++) {

				LayoutGenerator::Size product;
				auto r = randFloat();
				if (r < .1) {
					product = LayoutGenerator::FEATURED;
				}
				else if (r > .1 && r < .3)
				{
					product = LayoutGenerator::HIGHLIGHT;
				}
				else
				{
					product = LayoutGenerator::STANDARD;
				}

				cloud_structure.push_back(product);

			}

			subcat_structure.push_back(cloud_structure);

		}

		mLayout = generator.generateLayout(subcat_structure);

		mSubcategory = createSubcategory( mLayout );
		mSubcategory.perturb();


	};

	mParams->addParam("clouds", &mNumClouds);
	mParams->addParam("min products", &mMinProducts);
	mParams->addParam("max products", &mMaxProducts);
	mParams->addButton("Generate Layout", generate_layout);
	mParams->addSeparator();
	mParams->addParam("show springs", &mShowSprings);

	mEpicenter = vec2(getWindowCenter().x, getWindowCenter().y - 200);

	mParams->addParam("epicenter - x", &mEpicenter.x);
	mParams->addParam("epicenter - y", &mEpicenter.y);

	mParams->hide();

	//create b2 debug draw

	mDraw.SetFlags(gl::b2DubugDraw::e_jointBit | gl::b2DubugDraw::e_shapeBit);
	gl::enableAlphaBlending();

	//create b2 world

	b2Vec2 gravity(0.0f, 0.0f);
	mWorld = new b2World(gravity);
	mWorld->SetDebugDraw(&mDraw);

	//create bounds

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(getWindowCenter().x, getWindowHeight() - 10);

	b2Body * ground = mWorld->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(getWindowWidth(), 20);

	b2PolygonShape dynamicBoxTall;
	dynamicBoxTall.SetAsBox(20, getWindowHeight());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
	fixtureDef.filter.categoryBits = BOUNDRY;
	fixtureDef.filter.maskBits = BOUNDRY | CLOUD | PRODUCT;

	ground->CreateFixture(&fixtureDef);

	b2BodyDef ceiling_def;
	ceiling_def.type = b2_staticBody;
	ceiling_def.position = b2Vec2(getWindowCenter().x, 10);

	b2Body * ceiling = mWorld->CreateBody(&ceiling_def);
	ceiling->CreateFixture(&fixtureDef);

	b2BodyDef wall_def;
	wall_def.type = b2_staticBody;
	wall_def.position = b2Vec2(10, getWindowCenter().y);

	b2Body * left_wall = mWorld->CreateBody(&wall_def);

	b2FixtureDef lw_f;
	lw_f.shape = &dynamicBoxTall;
	lw_f.density = 1.0f;
	lw_f.friction = 0.3f;
	lw_f.restitution = 0.5f; // bounce
	lw_f.filter.categoryBits = BOUNDRY;
	lw_f.filter.maskBits = BOUNDRY | CLOUD | PRODUCT;

	left_wall->CreateFixture(&lw_f);

	b2BodyDef r_wall_def;
	r_wall_def.type = b2_staticBody;
	r_wall_def.position = b2Vec2(getWindowWidth() - 10, getWindowCenter().y);

	b2Body * right_wall = mWorld->CreateBody(&r_wall_def);
	right_wall->CreateFixture(&lw_f);

	//generate layout

	generate_layout();

}

void PhysicsScene::update()
{
	mSubcategory.update();
	for (auto & token : mTokens) {
		token->update();
	}

	for (int i = 0; i < 5; ++i)
		mWorld->Step(1 / 30.0f, 10, 10);

	for ( auto & token : mTokens ) {
		mTokenBatch.add(token->getCenter(), token->getRadius(), b2Color(37./255., 94./255., 249./255.) );
	}

}

void PhysicsScene::pause()
{
	mTokens.clear();
	mParams->hide();
}

void PhysicsScene::play()
{
	mParams->show();
}

void PhysicsScene::draw()
{
	gl::clear( Color::hex(0xf94343) );

	mSubcategory.draw();
	mTokenBatch.draw();

	if (mShowSprings) {
		mSubcategory.drawSprings();
		for (auto & token : mTokens) {
			token->drawSpring();
		}
	}

	//gl::drawString(std::to_string(ci::app::getAverageFps()), vec2(50), Color(1.0, 0., 0.), Font("Helvetica", 30));
}

std::shared_ptr<SpringGroup> PhysicsScene::createCloud(const LayoutGenerator::CloudLayout & cloud)
{

	b2BodyDef anchorDef;
	anchorDef.type = b2_dynamicBody;
	anchorDef.position.Set( cloud.first.first.x, cloud.first.first.y );

	b2CircleShape cloudContainer;
	///////////////cloudContainer.m_radius = cloud.first.second;

	b2FixtureDef anchorFixture;
	anchorFixture.shape = &cloudContainer;
	anchorFixture.density = 1.0f;
	anchorFixture.friction = 0.3f;
	anchorFixture.restitution = 0.5f; // bounce
	anchorFixture.filter.categoryBits = CLOUD;
	anchorFixture.filter.maskBits = BOUNDRY | CLOUD;

	auto anchor = mWorld->CreateBody(&anchorDef);

	anchor->CreateFixture(&anchorFixture);

	auto spring_group = std::make_shared<SpringGroup>(mWorld, anchor);

	std::list<b2Body*> bodies;

	for (int i = 0; i < cloud.second.size(); i++) {

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.linearDamping = 2.;
		bodyDef.angularDamping = 1.;

		//spiral out from center w/ top priority.
		bodyDef.position.Set( cloud.second[i].first.x, cloud.second[i].first.y );

		b2Body *body = mWorld->CreateBody(&bodyDef);

		b2CircleShape dynamicBox;
	
		//////////////dynamicBox.m_radius = cloud.second[i].second;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.5f; // bounce
		fixtureDef.filter.categoryBits = PRODUCT;
		fixtureDef.filter.maskBits = BOUNDRY | PRODUCT;

		body->CreateFixture(&fixtureDef);
		spring_group->add(body);

	}

	return spring_group;
}

Subcategory PhysicsScene::createSubcategory(const LayoutGenerator::SubcategoryLayout & sub_category)
{
	Subcategory group;

	auto spring_group = std::make_shared<SpringGroup>(mWorld, b2Vec2(getWindowCenter().x, getWindowCenter().y), false);

	auto center = b2Vec2(getWindowCenter().x, getWindowCenter().y);

	for (int i = 0; i < sub_category.size(); i++) {
		auto cloud = createCloud(sub_category[i]);
		group.add(cloud);
		spring_group->add(cloud->getAnchor());
	}

	group.add(spring_group);

	return std::move(group);
}
