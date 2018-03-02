#include "Attractor.h"
#include "cinder/Log.h"

using namespace ci;

void Attractor::attract()
{
	int iterations = 0;
	while (!iterate())
	{
		if (iterations > 5000) {
			CI_LOG_V("too many iterations");
			break;
		}
		iterations++;
		
	}

}

bool Attractor::iterate()
{
	
	auto outer = mRects.begin();
	auto end = mRects.end();

	int i = 0;
	while (outer != end) {

		vec2 velocity = vec2(0);

		auto inner = mRects.begin();

		auto new_pos = **outer;
		auto new_posx = **outer;
		auto new_posy = **outer;

		auto dir = glm::normalize(mCenter - new_pos.getCenter());
		
		new_pos.offset(dir);
		new_pos.set(new_pos.x1 - mSeparation*.5f, new_pos.y1 - mSeparation*.5f, new_pos.x2 + mSeparation*.5f, new_pos.y2 + mSeparation*.5f);

		new_posx.offset(vec2(dir.x,0));
		new_posx.set(new_pos.x1 - mSeparation*.5f, new_pos.y1 - mSeparation*.5f, new_pos.x2 + mSeparation*.5f, new_pos.y2 + mSeparation*.5f);

		new_posy.offset(vec2(0.,dir.y));
		new_posy.set(new_pos.x1 - mSeparation*.5f, new_pos.y1 - mSeparation*.5f, new_pos.x2 + mSeparation*.5f, new_pos.y2 + mSeparation*.5f);

		bool intersection = false;

		bool tb_intersection = false;
		bool rl_intersection = false;

		while (inner != end) {

			if (*inner == *outer) { ++inner; continue; }

			/*if (new_pos.intersects(**inner)) {

				intersection = true;
*/
				if (new_posx.intersects(**inner)) {
					rl_intersection = true;
				}

				if (new_posy.intersects(**inner)) {
					tb_intersection = true;
				}

			//	}

			++inner;
		}

	/*	if (!intersection)
			velocity = dir;
		else{*/
			if (tb_intersection && !rl_intersection) {
				velocity.x = dir.x;
			}
			if (!tb_intersection && rl_intersection) {
				velocity.y = dir.y;
			}
		//}

		(**outer).offset(velocity);
		mVelocities[i++] = velocity;
		++outer;
	}

	for ( auto & vel : mVelocities ) {
		if (vel.x != 0 || vel.y != 0)
			return false;
	}

	return true;
}
