#include "RectPacker.h"
#include "cinder/Log.h"
#include "cinder/Rand.h"

void RectPacker::pack(bool sort)
{
	//bool done = true;

	// Sort circles based on the distance to center
	if (sort) {
		auto center = mPackingCenter;
		mRects.sort([center](ci::Rectf* p1, ci::Rectf* p2) {
			float d1 = glm::length2(ci::vec2(p1->getCenter()) - center);
			float d2 = glm::length2(ci::vec2(p2->getCenter()) - center);
			if (d1 > d2)
				return true;
			else if (d1 < d2)
				return false;
			else return false;
		});
	}

	auto it = mRects.begin();
	auto end = mRects.end();
	while (it != end) {

		auto inner_it = mRects.begin();

		while (inner_it != end) {

			if (it == inner_it) { inner_it++; continue; }

			if ((*inner_it)->x1 < mBounds.x1) {
				(*inner_it)->offsetCenterTo((*inner_it)->getCenter() + ci::vec2(glm::distance((*inner_it)->x1, mBounds.x1), 0));
			}

			if ((*inner_it)->x2 > mBounds.x2)
			{
				(*inner_it)->offsetCenterTo((*inner_it)->getCenter() - ci::vec2(glm::distance((*inner_it)->x2, mBounds.x2), 0));
			}

			if ((*inner_it)->y1 < mBounds.y1) {
				(*inner_it)->offsetCenterTo((*inner_it)->getCenter() + ci::vec2(0, glm::distance((*inner_it)->y1, mBounds.y1)));
			}

			if ((*inner_it)->y2 > mBounds.y2)
			{
				(*inner_it)->offsetCenterTo((*inner_it)->getCenter() - ci::vec2(0, glm::distance((*inner_it)->y2, mBounds.y2)));
			}

			auto diff = (*inner_it)->getCenter() - (*it)->getCenter();

			auto dir = glm::normalize(diff);
			float w = ((*it)->getWidth() / 2.f) + ((*inner_it)->getWidth() / 2.f) + mMinSeparation;
			float h = ((*it)->getHeight() / 2.f) + ((*inner_it)->getHeight() / 2.f) + mMinSeparation;

			auto w_sep = *(*it);
			w_sep.set(w_sep.x1 - mMinSeparation*.5f, w_sep.y1 - mMinSeparation*.5f, w_sep.x2 + mMinSeparation*.5f, w_sep.y2 + mMinSeparation*.5f);
			if (w_sep.intersects(**inner_it))
			{

				dir.x *= w - glm::abs(diff.x);
				dir.x *= .5f;

				(*inner_it)->offsetCenterTo((*inner_it)->getCenter() + ci::vec2(dir.x, 0.));
				(*it)->offsetCenterTo((*it)->getCenter() - ci::vec2(dir.x, 0.));

				dir.y *= h - glm::abs(diff.y);
				dir.y *= .5f;

				(*inner_it)->offsetCenterTo((*inner_it)->getCenter() + ci::vec2(0., dir.y));
				(*it)->offsetCenterTo((*it)->getCenter() - ci::vec2(0., dir.y));

			}

			

			inner_it++;
		}
		it++;
	}

	//return done;
}

