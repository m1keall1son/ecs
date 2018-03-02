#pragma once

#pragma once

#include "SceneBase.h"
#include "cinder/params/Params.h"

#include "ciDebugDraw.h"
#include "cinder/Rand.h"
#include "LayoutGenerator.h"
#include "b2Spring.h"
#include "CollisionTypes.h"


class Subcategory {

public:

	void add(const std::shared_ptr<SpringGroup>& sg) { mClouds.push_back(sg); }

	void update() {
		for (auto & sg : mClouds) {
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

	void perturb() {
		for (auto& c : mClouds)
		{
			c->perturb();
		}
	}

	~Subcategory()
	{
		mClouds.clear();
	}


private:

	std::vector<std::shared_ptr<SpringGroup>> mClouds;

};

class Token {

public:

	Token(b2World* world, const b2Vec2& position, const b2Vec2& target) : mTarget(target) {

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
		fix.filter.maskBits = PRODUCT | BOUNDRY;

		mTokenBody->CreateFixture(&fix);

		mSpring = std::make_shared<Spring>(mTokenBody, mTokenBody->GetLocalCenter(), mAnchor, mAnchor->GetLocalCenter(), 5000.f, 25.f, .01f);
		mWorld = world;
	}

	void update()
	{
		auto delta = mTarget - mAnchor->GetWorldCenter();
		delta *= 0.05f;
		mAnchor->SetTransform(mAnchor->GetWorldCenter() + delta, mAnchor->GetAngle());
		mSpring->update();
	}

	void drawSpring()
	{
		mSpring->draw();
	}

	b2Vec2 getCenter() { return mTokenBody->GetWorldCenter();  }
	float getRadius() { return 10.f; }

	~Token()
	{
		mWorld->DestroyBody(mAnchor);
		mWorld->DestroyBody(mTokenBody);
	}

private:

	b2Body* mAnchor;
	std::shared_ptr<Spring> mSpring;
	b2Body* mTokenBody;
	b2Vec2 mTarget;
	b2World* mWorld;

};

class PhysicsScene : public SceneBase {

public:

	static SceneRef create() { return SceneRef(new PhysicsScene); }

	virtual const char * getName() { return "Physics Test"; };

	void touchesBegan(ci::app::TouchEvent event)override;
	void touchesMoved(ci::app::TouchEvent event)override;
	void touchesEnded(ci::app::TouchEvent event)override;

	void mouseDown(ci::app::MouseEvent event)override;
	void mouseDrag(ci::app::MouseEvent event) override;
	void mouseUp(ci::app::MouseEvent event) override;

	void setup()override;
	void update()override;
	void pause()override;
	void play()override;
	void draw()override;

private:

	std::shared_ptr<SpringGroup> createCloud( const LayoutGenerator::CloudLayout& cloud );
	Subcategory createSubcategory( const LayoutGenerator::SubcategoryLayout & sub_category );

	ci::gl::b2DubugDraw::CircleFillBatch mBatch;
	ci::gl::b2DubugDraw::CircleFillBatch mTokenBatch;

	LayoutGenerator::SubcategoryLayout mLayout;

	bool mShowClouds{ false };
	bool mShowPriority{ false };
	bool mShowEpicenter{ true };
	ci::vec2 mEpicenter;
	ci::params::InterfaceGlRef mParams;

	int mNumClouds, mNumSubCategories, mMinProducts, mMaxProducts;

	b2World				*mWorld;
	ci::gl::b2DubugDraw		mDraw;
	Subcategory			mSubcategory;

	std::vector<std::shared_ptr<SpringGroup>> mSpringGroups;

	std::vector<std::shared_ptr<Token>> mTokens;

	bool mShowSprings{ false };
	bool mShowB2DDebug{ true };

};