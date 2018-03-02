#include "WireRect.h"

using namespace ci;

WireRect::WireRect()
{
	mPositions[0] = vec4(-.5, -.5, 0., 1.);
	mPositions[1] = vec4(.5, -.5, 0., 1.);
	mPositions[2] = vec4(.5, -.5, 0., 1.);
	mPositions[3] = vec4(.5, .5, 0., 1.);
	mPositions[4] = vec4(.5, .5, 0., 1.);
	mPositions[5] = vec4(-.5, .5, 0., 1.);
	mPositions[6] = vec4(-.5, .5, 0., 1.);
	mPositions[7] = vec4(-.5, -.5, 0., 1.);
}

WireRect::WireRect(const ci::Rectf & r)
{
	rect(r);
}

WireRect & WireRect::rect(const ci::Rectf & rect)
{
	mPositions[7] = vec4(rect.getUpperLeft(), 0., 1.);
	mPositions[0] = vec4(rect.getUpperLeft(), 0., 1.);

	mPositions[1] = vec4(rect.getUpperRight(), 0., 1.);
	mPositions[2] = vec4(rect.getUpperRight(), 0., 1.);

	mPositions[3] = vec4(rect.getLowerRight(), 0., 1.);
	mPositions[4] = vec4(rect.getLowerRight(), 0., 1.);

	mPositions[5] = vec4(rect.getLowerLeft(), 0., 1.);
	mPositions[6] = vec4(rect.getLowerLeft(), 0., 1.);

	return *this;
}

void WireRect::loadInto(ci::geom::Target * target, const ci::geom::AttribSet & requestedAttribs) const
{
	target->copyAttrib(ci::geom::Attrib::POSITION, 4, 0, (const float*)mPositions.data(), 8);
}
