#pragma once

#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include <vector>

class Attractor {
public:

	Attractor(const ci::vec2& point, float separation) :mCenter(point), mSeparation(separation)  {}
	void attract();
	void addRect(ci::Rectf* rect) { mRects.push_back(rect); mVelocities.push_back(ci::vec2(0)); }

	bool iterate();


private:


	std::vector<ci::Rectf*> mRects;
	std::vector<ci::vec2>   mVelocities;

	ci::vec2 mCenter;
	float mSeparation;
};