#include "FlockingLayout.h"
#include "cinder/Rand.h"

using namespace ci;

FlockingLayout::Particle::Particle(ci::vec2 _loc, ci::vec2 _target, int &_id)
{
	mLocation = _loc;
	mCurrentTarget = _target;
	mId = _id;
	mMaxforce = .03f;
	mMaxspeed = 1.5f;
	mVelocity = ci::vec2(0);
	mAcceleration = ci::vec2(0);
	mMass = randFloat(1., 3.);
	float r = randFloat(.1, 1.);
}

void FlockingLayout::Particle::update() {
	mVelocity += mAcceleration;
	if (length2(mVelocity) > mMaxspeed*mMaxspeed) {
		mVelocity = glm::normalize(mVelocity)*mMaxspeed;
	}
	mLocation += mVelocity;
	mAcceleration *= 0;
}

void FlockingLayout::Particle::addForce(const ci::vec2 &force) {
	mAcceleration += force / mMass;
}

void FlockingLayout::Particle::applyBehaviors( std::vector<Particle*> &particles) {
	ci::vec2 separateForce = separate(particles);
	ci::vec2 seekForce = seek(mCurrentTarget);
	addForce(separateForce);
	addForce(seekForce);
}

ci::vec2 FlockingLayout::Particle::seek( const ci::vec2& target ) {
	ci::vec2 desired = target - mLocation;
	desired = glm::normalize(desired);
	desired *= mMaxspeed;
	ci::vec2 steer = desired - mVelocity;
	if (length2(steer) > mMaxforce*mMaxforce ) {
		steer = glm::normalize(steer)*mMaxforce;
	}
	return steer;
}

ci::vec2 FlockingLayout::Particle::separate( std::vector<Particle*>& particles) {
	float desiredseparation = 5.;
	ci::vec2 sum;
	int count = 0;
	for (int i = 0; i<particles.size(); i++) {
		if (mId != i) {
			float d = glm::distance(mLocation, particles[i]->mLocation);
			if ((d > 0) && (d < desiredseparation)) {
				ci::vec2 diff = mLocation - particles[i]->mLocation;
				diff = glm::normalize(diff);
				diff /= d;
				sum += diff;
				count++;
			}
		}
	}

	if (count > 0) {
		sum /= count;
		sum = glm::normalize(sum);
		sum *= mMaxspeed;
		sum -= mVelocity;
		if (length2(sum) > mMaxforce*mMaxforce) {
			sum = glm::normalize(sum)*mMaxforce;
		}
	}
	return sum;
}


void FlockingLayout::Particle::updateTarget(const ci::vec2 &newTarget) {
	mCurrentTarget = newTarget;
}

FlockingLayout::FlockingLayout(const ci::vec2 & center)
{

}
