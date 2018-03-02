#pragma once
#include <list>

#include "cinder/Vector.h"
#include "cinder/Sphere.h"
#include "cinder/Rect.h"
#include "cinder/Exception.h"

class CirclePacker
{
public:

	CirclePacker( const  ci::vec2& center, const ci::Rectf& bounds, float separation = 1.f) : mPackingCenter(center), mBounds(bounds), mMinSeparation(separation) {
	
		/*if (!bounds.contains(center))
			throw ci::Exception("Center must be within bounds.");*/

	}

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

					if (inner_it->getCenter().x - inner_it->getRadius() < mBounds.x1 ) {
						inner_it->setCenter( inner_it->getCenter() + ci::vec3( glm::distance( inner_it->getCenter().x - inner_it->getRadius(), mBounds.x1 ), 0, 0) );
					}
					
					if( inner_it->getCenter().x + inner_it->getRadius() > mBounds.x2 )
					{
						inner_it->setCenter( inner_it->getCenter() - ci::vec3(glm::distance(inner_it->getCenter().x + inner_it->getRadius(), mBounds.x2), 0, 0) );
					}


					if (inner_it->getCenter().y - inner_it->getRadius() < mBounds.y1 ) {
						inner_it->setCenter(inner_it->getCenter() + ci::vec3(0, glm::distance(inner_it->getCenter().y - inner_it->getRadius(), mBounds.y1), 0));
					}

					if (inner_it->getCenter().y + inner_it->getRadius() > mBounds.y2 )
					{
						inner_it->setCenter(inner_it->getCenter() - ci::vec3(0, glm::distance(inner_it->getCenter().y + inner_it->getRadius(), mBounds.y2), 0));
					}
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
	void clear() { mCircles.clear(); }

protected:

	std::list<ci::Sphere> mCircles;
	ci::vec2 mPackingCenter;
	float mMinSeparation;
	ci::Rectf mBounds;

};