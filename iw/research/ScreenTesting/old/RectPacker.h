#pragma once
#include <list>

#include "cinder/Vector.h"
#include "cinder/Sphere.h"
#include "cinder/Rect.h"
#include "cinder/Exception.h"

class RectPacker
{
public:

	RectPacker(const  ci::vec2& center, const ci::Rectf& bounds, float separation = 1.f) : mPackingCenter(center), mBounds(bounds), mMinSeparation(separation) {}

	void pack(bool sort = true);

	std::list<ci::Rectf*>& getRects() { return mRects; }
	void addRect( ci::Rectf* rect ) { mRects.push_back(rect); }
	const float getMinSeparation() const { return mMinSeparation; }
	void setMinSeparation(float min_separation) { mMinSeparation = min_separation; }
	void clear() { mRects.clear(); }

protected:

	std::list<ci::Rectf*> mRects;
	ci::vec2 mPackingCenter;
	float mMinSeparation;
	ci::Rectf mBounds;

};