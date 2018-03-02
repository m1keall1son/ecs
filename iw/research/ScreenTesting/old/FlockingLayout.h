#pragma once

#include "cinder/Vector.h"
#include <vector>

class FlockingLayout {

	class Particle {
	public:
		Particle(ci::vec2 _loc, ci::vec2 _target, int &_id);
		void update();
		void applyBehaviors( std::vector<Particle*> &particles);
		void updateTarget(const ci::vec2 &newTarget);
		bool hasArrived();
		void addForce(const ci::vec2 &force);

		int mId;
		ci::vec2 mCurrentTarget;

	private:

		ci::vec2 seek(const ci::vec2 &target);
		ci::vec2 separate( std::vector<Particle*> &particles);
		ci::vec2 cohesion( std::vector<Particle*> &particles);
		ci::vec2 mLocation;
		ci::vec2 mStartLoc;
		ci::vec2 mVelocity;
		ci::vec2 mAcceleration;
		float mMaxforce = 5;    // Maximum steering force
		float mMaxspeed = .5;    // Maximum speed
		float mMass;


	};

public:

	FlockingLayout(const ci::vec2& center);

	void add(  );

private:



};