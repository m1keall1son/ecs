#pragma once
#include <list>

#include "cinder/Vector.h"
#include "Box2D/Box2D.h"

class CirclePacker
{
public:

	CirclePacker(const ci::vec2& packing_center, float separation = 1.f) : mPackingCenter( b2Vec2(packing_center.x, packing_center.y) ), mMinSeparation(separation) {}
	CirclePacker(const b2Vec2& packing_center, float separation = 1.f) : mPackingCenter(packing_center), mMinSeparation(separation) {}

	void OnFrameMove( uint32_t iterationCounter )
	{
		// Sort circles based on the distance to center, reversed
		auto center = mPackingCenter;

		mCircles.sort([center](b2Body* p1, b2Body* p2){
			float d1 = (p1->GetWorldCenter() - center).LengthSquared();
			float d2 = (p2->GetWorldCenter() - center).LengthSquared();
			if (d1 > d2)
				return true;
			else if (d1 < d2)
				return false;
			else return false;
		});

		float minSeparationSq = mMinSeparation * mMinSeparation;

		auto it = mCircles.begin();
		auto end = mCircles.end();
		while (it != end) {

			auto inner_it = mCircles.begin();

			while (inner_it != end) {

				if (it == inner_it) { inner_it++; continue; }

				auto fixture_outer = (*it)->GetFixtureList();
				auto fixture_inner = (*inner_it)->GetFixtureList();

				if (fixture_outer && fixture_inner) {
					auto circle_outer = static_cast<b2CircleShape*>(fixture_outer->GetShape());
					auto circle_inner = static_cast<b2CircleShape*>(fixture_inner->GetShape());

					b2Vec2 AB = (*inner_it)->GetWorldCenter() - (*it)->GetWorldCenter();
					ci::vec2 ab = ci::vec2(AB.x, AB.y);
					float r = circle_outer->m_radius + circle_inner->m_radius;
					float d = glm::length2(ab) - minSeparationSq;
					float minSepSq = glm::min(d, minSeparationSq);
					d -= minSepSq;

					if (d < (r * r) - 0.01)
					{
						AB.Normalize();
						AB *= (float)((r - sqrt(d)) * 0.5f);
						(*inner_it)->SetTransform((*inner_it)->GetWorldCenter() + AB, (*inner_it)->GetAngle());
						(*it)->SetTransform((*it)->GetWorldCenter() - AB, (*it)->GetAngle());
					}
				}
				inner_it++;
			}
			it++;
		}
	}

	void addCircle(b2Body* body) { mCircles.push_back(body); }
	const float getMinSeparation() const { return mMinSeparation; }
	void setMinSeparation(float min_separation){ mMinSeparation = min_separation; }

protected:

	std::list<b2Body*> mCircles;

	b2Vec2 mPackingCenter;
	float mMinSeparation;


};