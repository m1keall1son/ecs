#include "SpiralFit.h"
#include "cinder/Rand.h"
#include "cinder/Exception.h"
#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "RectPacker.h"

using namespace ci;

SpiralFit::SpiralFit(const ci::vec2 & center, float min_separation, float min_length_step, float max_length_step) : mFitCenter(center), mMinSeparation(min_separation), mCurrentAngle(randFloat()*M_PI), mCurrentLength(randFloat()), mMaxLengthStep(max_length_step), mMinLengthStep(min_length_step)
{
}

void SpiralFit::fit()
{
	mIterations = 0;
	bool cont = true;
	bool done = false;
	ci::Rectf* unplaced;
	while (!done) {
		if (mPlaced.empty()) {
			//place first one
			auto unplaced = mUnplaced.front();
			mUnplaced.pop_front();
			unplaced->offsetCenterTo(mFitCenter + vec2(cos(mCurrentAngle), sin(mCurrentAngle))*mCurrentLength);
			place(unplaced);

			if (mUnplaced.empty())
				break;

		}
		else {
			if (cont) {
				if (mUnplaced.empty())
					throw ci::Exception("should not be here");
				unplaced = mUnplaced.front();
				mUnplaced.pop_front();
			}
			bool placed = false;
			while (stepAngle() && !placed) {
				cont = true;
				placed = tryPlace(unplaced);
			}
			if (!placed) {
				mCurrentLength += randFloat(mMinLengthStep, mMaxLengthStep);
				cont = false;
			}else{
				if (mUnplaced.empty()) {
					done = true;
				}
			}
		}
		mIterations++;
		if (mIterations > mMaxIterations)
			throw ci::Exception("Could not fit rects within bounds");
	}
}

float SpiralFit::fitOne(ci::Rectf * one)
{
	auto side = randFloat() > .5f ? -1.f : 1.f;

	auto bounds = calcFitArea();

	auto top_left = vec2(bounds.getUpperLeft().x, bounds.getUpperLeft().y + one->getSize().y*.5f);
	one->offsetCenterTo(top_left);

	float inc = .25f;
	while (!tryFitOne(one))
	{
		one->offset( side * vec2(inc, 0) );
	}


	CI_LOG_V( std::to_string(side) );

	return side;
}

ci::Rectf SpiralFit::calcFitArea()
{
	auto bounds = Rectf( mFitCenter - vec2(1), mFitCenter + vec2(1) );
	for ( auto & placed : mPlaced ) {
		bounds.include(*placed);
	}
	return bounds;
}


bool SpiralFit::tryPlace(Rectf* rect)
{
	rect->offsetCenterTo(mFitCenter + vec2(cos(mCurrentAngle), sin(mCurrentAngle))*mCurrentLength);

	//make sure it doesn't intersect others
	auto it = mPlaced.begin();
	auto end = mPlaced.end();
	while (it != end ) {
		auto placed = **it++;
		placed.set( placed.x1 - mMinSeparation*.5f, placed.y1 - mMinSeparation*.5f, placed.x2 + mMinSeparation*.5f, placed.y2 + mMinSeparation*.5f);
		if ((placed.contains(rect->getUpperLeft()) && placed.contains(rect->getUpperRight()) && placed.contains(rect->getLowerLeft()) && placed.contains(rect->getLowerRight())) || placed.intersects(*rect)) {
			return false;
		}
	}
	place(rect);
	return true;
}

bool SpiralFit::tryFitOne(ci::Rectf * rect)
{
	//make sure it doesn't intersect others
	auto it = mPlaced.begin();
	auto end = mPlaced.end();
	while (it != end) {
		auto placed = **it++;
		placed.set(placed.x1 - mMinSeparation*.5f, placed.y1 - mMinSeparation*.5f, placed.x2 + mMinSeparation*.5f, placed.y2 + mMinSeparation*.5f);
		if ((placed.contains(rect->getUpperLeft()) && placed.contains(rect->getUpperRight()) && placed.contains(rect->getLowerLeft()) && placed.contains(rect->getLowerRight())) || placed.intersects(*rect)) {
			return false;
		}
	}
	place(rect);
	return true;
}

void SpiralFit::place(Rectf* rect)
{
	mPlaced.push_back(rect);
}

bool SpiralFit::stepAngle()
{
	mCurrentAngle += randFloat(.01f, 3.f);
	if (mCurrentAngle > M_PI*2) {
		mCurrentAngle = randFloat(2*M_PI);
		return false;
	}
	return true;
}

