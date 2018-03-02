#pragma once
#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include <list>

class SpiralFit {

public:

	SpiralFit(const ci::vec2& center, float min_separation, float min_length_step = .2f, float max_length_step = 1.f );

	void fit();
	float fitOne(ci::Rectf* one);

	void addRect( ci::Rectf* rect) { mUnplaced.push_back(rect); }
	void addPlacedRect( ci::Rectf* rect ) { mPlaced.push_back(rect); }
	void clear() { mUnplaced.clear(); mPlaced.clear(); }
	ci::Rectf calcFitArea();

	std::list<ci::Rectf*>& getPlacements() { return mPlaced; }

private:

	bool tryPlace(ci::Rectf* rect);
	bool tryFitOne(ci::Rectf* rect);
	void place(ci::Rectf* rect);
	bool stepAngle();

	std::list<ci::Rectf*> mUnplaced;
	std::list<ci::Rectf*> mPlaced;

	float mMinSeparation;
	ci::vec2 mFitCenter;

	float mMinLengthStep;
	float mMaxLengthStep;
	float mCurrentAngle;
	float mCurrentLength;

	int mMaxIterations{ 100000 };
	int mIterations{ 0 };

	friend class Layout;

};
