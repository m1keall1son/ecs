#pragma once
#include <list>

#include "cinder/Vector.h"
#include "cinder/Sphere.h"

class CirclePacker
{
public:

	CirclePacker(const ci::vec2& packing_center, float separation = 1.f) : mPackingCenter( packing_center ), mMinSeparation(separation) {}

	void pack(bool sort = true)
	{
		//bool done = true;

		// Sort circles based on the distance to center
		if(sort) {
			auto center = mPackingCenter;
			mCircles.sort([center](const ci::Sphere& p1, const ci::Sphere& p2) {
				float d1 = glm::length2(ci::vec2(p1.getCenter()) - center);
				float d2 = glm::length2(ci::vec2(p2.getCenter()) - center);
				if (d1 < d2)
					return true;
				else if (d1 > d2)
					return false;
				else return false;
			});
		}

		float minSeparationSq = mMinSeparation * mMinSeparation;

		auto it = mCircles.begin();
		auto end = mCircles.end();
		while (it != end) {

			auto inner_it = mCircles.begin();

			while (inner_it != end) {

				if (it == inner_it) { inner_it++; continue; }

				auto AB = ci::vec2(inner_it->getCenter() - it->getCenter());
				float r = it->getRadius() + inner_it->getRadius();
				float d = glm::length2(AB) - minSeparationSq;
				float minSepSq = glm::min(d, minSeparationSq);
				d -= minSepSq;

				if (d < (r * r) - 0.01)
				{
					AB = glm::normalize(AB);
					AB *= (float)((r - sqrt(d)) * 0.5f);
					inner_it->setCenter(inner_it->getCenter() + ci::vec3(AB, 0.));
					it->setCenter( it->getCenter() - ci::vec3( AB, 0. ) );
					//done = false;
				}
				
				inner_it++;
			}
			it++;
		}

		//return done;
	}


	std::list<ci::Sphere>& getCirlces() { return mCircles; }
	void addCircle( const ci::Sphere& circle ) { mCircles.push_back(circle); }
	const float getMinSeparation() const { return mMinSeparation; }
	void setMinSeparation(float min_separation){ mMinSeparation = min_separation; }

protected:

	std::list<ci::Sphere> mCircles;

	ci::vec2 mPackingCenter;
	float mMinSeparation;


};