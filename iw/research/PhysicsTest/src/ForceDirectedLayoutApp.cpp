#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ciDebugDraw.h"
#include "cinder/Rand.h"
#include "cinder/Log.h"

#include <Box2D/Box2D.h>

using namespace ci;
using namespace ci::app;
using namespace std;

const float BOX_SIZE = 40;

#include "CirclePacker.h"

enum CollisionType {
	PRODUCT = 0x0001,
	CLOUD = 0x0002,
	BOUNDRY = 0x0004
	/*
	FRIENDLY_AIRCRAFT = 0x0008,
	ENEMY_AIRCRAFT = 0x0010,*/
};

class Spring {

public:

	Spring(b2Body* body_A, const b2Vec2& local_a, b2Body* body_B, const b2Vec2& local_b, float k, float friction, float length) :mBodyA(body_A), mBodyB(body_B), mLocalA(local_a), mLocalB(local_b), mK(k), mLength(length), mFriction(friction) {}

	void update()
	{
		b2Vec2 pA = mBodyA->GetWorldPoint(mLocalA);
		b2Vec2 pB = mBodyB->GetWorldPoint(mLocalB);
		b2Vec2 diff = pB - pA;
		//Find velocities of attach points
		b2Vec2 vA = mBodyA->GetLinearVelocity() - (b2Cross(mBodyA->GetWorldVector(mLocalA), mBodyA->GetAngularVelocity()));
		b2Vec2 vB = mBodyB->GetLinearVelocity() - (b2Cross(mBodyB->GetWorldVector(mLocalB), mBodyB->GetAngularVelocity()));
		b2Vec2 vdiff = vB - vA;
		float dx = diff.Normalize(); //normalizes diff and puts length into dx
		float vrel = vdiff.x*diff.x + vdiff.y*diff.y;
		float forceMag = -mK*(dx - mLength) - mFriction*vrel;
		diff *= forceMag; // diff *= forceMag
		mBodyB->ApplyForce(diff, mBodyA->GetWorldPoint(mLocalA));
		diff *= -1.f;
		mBodyA->ApplyForce(diff, mBodyB->GetWorldPoint(mLocalB));
	}

	void draw() {
		gl::ScopedColor color(Color(1., 0., 0.));
		auto w1 = mBodyA->GetWorldPoint(mLocalA);
		auto w2 = mBodyB->GetWorldPoint(mLocalB);
		gl::drawLine(vec2(w1.x, w1.y), vec2(w2.x, w2.y));
	}

private:

	float	mK;
	float	mLength;
	float	mFriction;
	b2Vec2  mLocalA, mLocalB;
	b2Body* mBodyA;
	b2Body* mBodyB;

};

class SpringGroup {

public:

	SpringGroup( b2World* world, const b2Vec2& center) : mWorld(world), mCenter(center){
		b2BodyDef anchorDef;
		anchorDef.type = b2_staticBody;
		anchorDef.position.Set(mCenter.x, mCenter.y);
		mAnchor = mWorld->CreateBody(&anchorDef);
	}

	SpringGroup(b2World* world, b2Body* anchor ) : mAnchor(anchor), mWorld(world), mCenter(anchor->GetWorldCenter()) {}

	void setCenter(const b2Vec2& center) {
		mCenter = center;
		mAnchor->SetTransform( mCenter, mAnchor->GetAngle() );
	}

	void add( b2Body* body ) {

		auto diff = (mCenter - body->GetWorldCenter()).Length();
		mSprings.emplace_back(body, body->GetLocalCenter(), mAnchor, mAnchor->GetLocalCenter(), 5000.f, 25.f, diff );

		if (mBodies.empty()) {
			mBodies.push_back(body);
		}else{

			for ( auto & other : mBodies ) {
				auto diff_back = (other->GetWorldCenter() - body->GetWorldCenter()).Length();
				mSprings.emplace_back(body, body->GetLocalCenter(), other, other->GetLocalCenter(), 5000.f, 25.f, diff_back);
			}

			mBodies.push_back(body);
		}
	}

	void update() {
		for ( auto & spring : mSprings ) {
			spring.update();
		}
		float num = 0;
		for ( auto & body : mBodies ) {
			auto circle = static_cast<b2CircleShape*>(body->GetFixtureList()->GetShape());
			//if(circle->m_radius < 100)
			mCircleBatch.add( body->GetWorldCenter(), circle->m_radius, b2Color( 1., num / (float)mBodies.size(), num / (float)mBodies.size()) );
			num++;
		}

	}

	void draw()
	{
		mCircleBatch.draw();
	}

	void drawSprings() {
		for (auto & spring : mSprings) {
			spring.draw();
		}
	}

	~SpringGroup() {
		mSprings.clear();
		mBodies.clear();
	}

private:

	b2World* mWorld;
	b2Body * mAnchor;
	b2Vec2 mCenter;
	std::vector<Spring> mSprings;
	std::vector<b2Body*> mBodies;
	ci::gl::b2DubugDraw::CircleFillBatch mCircleBatch;

};

class CloudGroup {

public:

	void add(const std::shared_ptr<SpringGroup>& sg) { mClouds.push_back(sg); }

	void update() {
		for ( auto & sg : mClouds ) {
			sg->update();
		}
	}

	void draw() {
		for (auto & sg : mClouds) {
			sg->draw();
		}
	}

	void drawSprings()
	{
		for (auto & sg : mClouds) {
			sg->drawSprings();
		}
	}

private:

	std::vector<std::shared_ptr<SpringGroup>> mClouds;

};

class Token {

public:

	Token( b2World* world, const b2Vec2& position, const b2Vec2& target ) : mTarget(target) {

		b2BodyDef anchor;
		anchor.type = b2_staticBody;
		anchor.position = position;

		mAnchor = world->CreateBody(&anchor);

		b2BodyDef token;
		token.type = b2_dynamicBody;
		token.position = position;

		mTokenBody = world->CreateBody(&token);

		b2CircleShape circle;
		circle.m_radius = 10;

		b2FixtureDef fix;
		fix.shape = &circle;
		fix.density = 1.0f;
		fix.friction = 0.3f;
		fix.restitution = 0.5f; // bounce
		fix.filter.categoryBits = PRODUCT;
		fix.filter.maskBits =  PRODUCT | BOUNDRY;

		mTokenBody->CreateFixture(&fix);

		mSpring = std::make_shared<Spring>( mTokenBody, mTokenBody->GetLocalCenter(), mAnchor, mAnchor->GetLocalCenter(), 5000.f, 25.f, .01f );

	}

	void update()
	{
		auto delta = mTarget - mAnchor->GetWorldCenter();
		delta *= 0.05f;
		mAnchor->SetTransform(mAnchor->GetWorldCenter() + delta, mAnchor->GetAngle() );
		mSpring->update();
	}

	void drawSpring()
	{
		mSpring->draw();
	}

private:

	b2Body* mAnchor;
	std::shared_ptr<Spring> mSpring;
	b2Body* mTokenBody;
	b2Vec2 mTarget;

};

class ForceDirectedLayoutApp : public App {
  public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;

	std::shared_ptr<SpringGroup> createCloud( b2World* world, b2Body* anchor, int num_items);
		
	CloudGroup createCloudGroup(b2World* world, int num_clouds);

	b2World				*mWorld;
	gl::b2DubugDraw		mDraw;
	CloudGroup			mCloudGroup;

	std::vector<std::shared_ptr<SpringGroup>> mSpringGroups;

	std::vector<std::shared_ptr<Token>> mTokens;

	bool mShowSprings{false};

};

void ForceDirectedLayoutApp::setup()
{
	//gl::enableVerticalSync(false);

	mDraw.SetFlags( gl::b2DubugDraw::e_jointBit | gl::b2DubugDraw::e_shapeBit );
	gl::enableAlphaBlending();

	b2Vec2 gravity( 0.0f, 0.0f );
	mWorld = new b2World( gravity );
	mWorld->SetDebugDraw(&mDraw);

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2( getWindowCenter().x, getWindowHeight()-10 );

	b2Body * ground = mWorld->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( getWindowWidth(), 20 );

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
	r_wall_def.position = b2Vec2( getWindowWidth() - 10, getWindowCenter().y);

	b2Body * right_wall = mWorld->CreateBody(&r_wall_def);
	right_wall->CreateFixture(&lw_f);


	mCloudGroup = createCloudGroup( mWorld, 4 );

}


void ForceDirectedLayoutApp::mouseDown( MouseEvent event )
{

	//mSpringGroup->setCenter( b2Vec2( event.getPos().x, event.getPos().y ) );

	//b2BodyDef bodyDef;
	//bodyDef.type = b2_dynamicBody;
	//bodyDef.position.Set(event.getPos().x, event.getPos().y);

	//b2Body *body = mWorld->CreateBody(&bodyDef);

	//b2CircleShape dynamicBox;

	//auto rand = randFloat();
	//if (rand < .33) {
	//	dynamicBox.m_radius = 40;
	//}
	//else if (rand > .33 && rand < .66) {
	//	dynamicBox.m_radius = 20;
	//}
	//else {
	//	dynamicBox.m_radius = 30;
	//}

	//b2FixtureDef fixtureDef;
	//fixtureDef.shape = &dynamicBox;
	//fixtureDef.density = 1.0f;
	//fixtureDef.friction = 0.3f;
	//fixtureDef.restitution = 0.5f; // bounce
	//fixtureDef.filter.categoryBits = PRODUCT;
	//fixtureDef.filter.maskBits =  PRODUCT ;


	//body->CreateFixture(&fixtureDef);
	//body->SetLinearVelocity( b2Vec2(10000,0) );
	//

	mTokens.emplace_back( std::make_shared<Token>( mWorld, b2Vec2( event.getPos().x, event.getPos().y ), b2Vec2( getWindowWidth()-40, getWindowCenter().y ) ) );

}

void ForceDirectedLayoutApp::keyDown(KeyEvent event)
{
	switch (event.getCode()) {
	case KeyEvent::KEY_ESCAPE:
	{
		quit();
	}break;
	case KeyEvent::KEY_s:
	{
		mShowSprings = !mShowSprings;
	}		break;
	default: break;
	}
}

void ForceDirectedLayoutApp::update()
{
	mCloudGroup.update();
	for (auto & token : mTokens) {
		token->update();
	}

	for( int i = 0; i < 5; ++i )
		mWorld->Step( 1 / 30.0f, 10, 10 );

	if(getElapsedFrames() % 60 == 0)
		CI_LOG_V("fps: " << getAverageFps() );
}

void ForceDirectedLayoutApp::draw()
{
	gl::clear();

	//mWorld->DrawDebugData();
	//mDraw.draw();

	mCloudGroup.draw();

	if (mShowSprings) {
		mCloudGroup.drawSprings();
		for (auto & token : mTokens) {
			token->drawSpring();
		}
	}

	gl::drawString( std::to_string(getAverageFps()), vec2(50), Color(1.0,0.,0.), Font( "Helvetica", 30 ) );

}

std::shared_ptr<SpringGroup> ForceDirectedLayoutApp::createCloud(b2World * world, b2Body* anchor, int num_items)
{

	auto spring_group = std::make_shared<SpringGroup>(world, anchor);

	auto packer = CirclePacker(anchor->GetWorldCenter(), 1.f);

	std::list<b2Body*> bodies;

	for (int i = 0; i < num_items; i++) {

		auto t = lmap((float)i, 0.f, 12.f, 0.f, (float)M_PI*2.f );
		auto d = lmap((float)i, 0.f, 12.f, 1.f, 6.f);

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.linearDamping = 2.;
		bodyDef.angularDamping = 1.;
		
		//spiral out from center w/ top priority.
		bodyDef.position.Set(anchor->GetWorldCenter().x + cos(t)*d, anchor->GetWorldCenter().y + sin(t)*d);

		b2Body *body = mWorld->CreateBody(&bodyDef);

		b2CircleShape dynamicBox;
/*
		if (i == 0) dynamicBox.m_radius = 80;
		else dynamicBox.m_radius = 40;*/

		dynamicBox.m_radius = 40;

		/*auto rand = randFloat();
		if (rand < .8) {
			dynamicBox.m_radius = 40;
		}
		else if (rand > .8 && rand < .9) {
			dynamicBox.m_radius = 60;
		}
		else {
			dynamicBox.m_radius = 80;
		}
*/
		/*static float radius = 50;
		dynamicBox.m_radius = radius;*/
		//radius -= 3;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.5f; // bounce
		fixtureDef.filter.categoryBits = PRODUCT;
		fixtureDef.filter.maskBits = BOUNDRY | PRODUCT;

		body->CreateFixture(&fixtureDef);

		bodies.push_back(body);
		packer.addCircle(body);

	}

	for (int i = 0; i<10; i++)
		packer.OnFrameMove(1);

	/*auto c = b2Vec2( getWindowCenter().x, getWindowCenter().y - 250);
	bodies.sort([c](b2Body* p1, b2Body* p2) {
		float d1 = (p1->GetWorldCenter() - b2Vec2(c.x, c.y)).LengthSquared();
		float d2 = (p2->GetWorldCenter() - b2Vec2(c.x, c.y)).LengthSquared();
		if (d1 < d2)
			return true;
		else if (d1 > d2)
			return false;
		else return false;
	});*/

	for (auto & body : bodies) {
		spring_group->add(body);
	}

	return spring_group;
}

CloudGroup ForceDirectedLayoutApp::createCloudGroup(b2World * world, int num_clouds)
{

	CloudGroup group;

	auto spring_group = std::make_shared<SpringGroup>(world, b2Vec2(getWindowCenter().x, getWindowCenter().y));

	auto center = b2Vec2( getWindowCenter().x, getWindowCenter().y );

	for (int i = 0; i < num_clouds; i++ ) {

		auto t = ci::lmap( (float)i, 0.f, (float)num_clouds, 0.f, (float)M_PI*2.f );

		b2BodyDef anchorDef;
		anchorDef.type = b2_dynamicBody;
		anchorDef.position.Set(cos(t)*getWindowWidth()/4 + getWindowCenter().x, sin(t)*getWindowHeight()/4 + getWindowCenter().y );

		b2CircleShape cloudContainer;
		cloudContainer.m_radius = 400;

		b2FixtureDef anchorFixture;
		anchorFixture.shape = &cloudContainer;
		anchorFixture.density = 1.0f;
		anchorFixture.friction = 0.3f;
		anchorFixture.restitution = 0.5f; // bounce
		anchorFixture.filter.categoryBits = CLOUD;
		anchorFixture.filter.maskBits = BOUNDRY | CLOUD;

		auto anchor = mWorld->CreateBody(&anchorDef);

		anchor->CreateFixture(&anchorFixture);

		group.add( createCloud( world, anchor, randInt(8, 15) ) );

		spring_group->add( anchor );

	}

	group.add(spring_group);

	return std::move(group);
}


CINDER_APP(ForceDirectedLayoutApp, RendererGl(RendererGl::Options().msaa(4)), [](app::App::Settings*settings) { settings->setFullScreen(); settings->setFrameRate(60); })
