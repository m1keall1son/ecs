#pragma once

#include "Box2D/Box2D.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "CollisionTypes.h"
#include "ciDebugDraw.h"
//
//class Spring {
//
//public:
//
//	Spring(b2Body* body_A, const b2Vec2& local_a, b2Body* body_B, const b2Vec2& local_b, float k, float friction, float length) :mBodyA(body_A), mBodyB(body_B), mLocalA(local_a), mLocalB(local_b), mK(k), mLength(length), mFriction(friction) {}
//
//	void update()
//	{
//		b2Vec2 pA = mBodyA->GetWorldPoint(mLocalA);
//		b2Vec2 pB = mBodyB->GetWorldPoint(mLocalB);
//		b2Vec2 diff = pB - pA;
//		//Find velocities of attach points
//		b2Vec2 vA = mBodyA->GetLinearVelocity() - (b2Cross(mBodyA->GetWorldVector(mLocalA), mBodyA->GetAngularVelocity()));
//		b2Vec2 vB = mBodyB->GetLinearVelocity() - (b2Cross(mBodyB->GetWorldVector(mLocalB), mBodyB->GetAngularVelocity()));
//		b2Vec2 vdiff = vB - vA;
//		float dx = diff.Normalize(); //normalizes diff and puts length into dx
//		float vrel = vdiff.x*diff.x + vdiff.y*diff.y;
//		float forceMag = -mK*(dx - mLength) - mFriction*vrel;
//		diff *= forceMag; // diff *= forceMag
//		mBodyB->ApplyForce(diff, mBodyA->GetWorldPoint(mLocalA));
//		diff *= -1.f;
//		mBodyA->ApplyForce(diff, mBodyB->GetWorldPoint(mLocalB));
//	}
//
//	void draw() {
//		ci::gl::ScopedColor color(ci::Color(1., 0., 0.));
//		auto w1 = mBodyA->GetWorldPoint(mLocalA);
//		auto w2 = mBodyB->GetWorldPoint(mLocalB);
//		ci::gl::drawLine(ci::vec2(w1.x, w1.y), ci::vec2(w2.x, w2.y));
//	}
//
//private:
//
//	float	mK;
//	float	mLength;
//	float	mFriction;
//	b2Vec2  mLocalA, mLocalB;
//	b2Body* mBodyA;
//	b2Body* mBodyB;
//
//};
//
//class SpringGroup {
//
//public:
//
//	SpringGroup(b2World* world, const b2Vec2& center, bool owns_bodies = true) : mOwnsBodies(owns_bodies), mWorld(world), mCenter(center) {
//		b2BodyDef anchorDef;
//		anchorDef.type = b2_staticBody;
//		anchorDef.position.Set(mCenter.x, mCenter.y);
//		mAnchor = mWorld->CreateBody(&anchorDef);
//	}
//
//	SpringGroup(b2World* world, b2Body* anchor, bool owns_bodies = true) : mOwnsBodies(owns_bodies), mAnchor(anchor), mWorld(world), mCenter(anchor->GetWorldCenter()) {}
//
//	void setCenter(const b2Vec2& center) {
//		mCenter = center;
//		mAnchor->SetTransform(mCenter, mAnchor->GetAngle());
//	}
//
//	void perturb()
//	{
//		for (auto & b : mBodies) {
//			b->SetTransform(b->GetWorldCenter() + b2Vec2(ci::randFloat(-25, 25), ci::randFloat(-25, 25)), b->GetAngle());
//		}
//	}
//
//	void add(b2Body* body) {
//
//		auto diff = (mCenter - body->GetWorldCenter()).Length();
//		mSprings.emplace_back(body, body->GetLocalCenter(), mAnchor, mAnchor->GetLocalCenter(), 5000.f, 25.f, diff);
//
//		if (mBodies.empty()) {
//			mBodies.push_back(body);
//		}
//		else {
//
//			for (auto & other : mBodies) {
//				auto diff_back = (other->GetWorldCenter() - body->GetWorldCenter()).Length();
//				mSprings.emplace_back(body, body->GetLocalCenter(), other, other->GetLocalCenter(), 5000.f, 25.f, diff_back);
//			}
//
//			mBodies.push_back(body);
//		}
//	}
//
//	void update() {
//		for (auto & spring : mSprings) {
//			spring.update();
//		}
//		float num = 0;
//		for (auto & body : mBodies) {
//			auto circle = static_cast<b2CircleShape*>(body->GetFixtureList()->GetShape());
//			//if(circle->m_radius < 100)
//			if (body->GetFixtureList()->GetFilterData().categoryBits & PRODUCT != 0)
//				mCircleBatch.add(body->GetWorldCenter(), circle->m_radius, b2Color(1., 1., 1.));
//			num++;
//		}
//
//	}
//
//	void draw()
//	{
//		mCircleBatch.draw();
//	}
//
//	void drawSprings() {
//		for (auto & spring : mSprings) {
//			spring.draw();
//		}
//	}
//
//	~SpringGroup() {
//		if (mOwnsBodies) {
//			for (auto & body : mBodies) {
//				mWorld->DestroyBody(body);
//			}
//		}
//		mWorld->DestroyBody(mAnchor);
//		mSprings.clear();
//		mBodies.clear();
//	}
//
//
//	b2Body* getAnchor() { return mAnchor; }
//
//private:
//
//	b2World* mWorld;
//	b2Body * mAnchor;
//	b2Vec2 mCenter;
//	std::vector<Spring> mSprings;
//	std::vector<b2Body*> mBodies;
//	ci::gl::b2DubugDraw::CircleFillBatch mCircleBatch;
//	bool mOwnsBodies{ true };
//};