#pragma once

#include "cinder/GeomIo.h"

class WireRect : public ci::geom::WireSource
{

public:

	WireRect();
	WireRect(const ci::Rectf& rect);

	WireRect& rect(const ci::Rectf& rect);

	size_t getNumVertices()const override { return mPositions.size(); }

	void loadInto(ci::geom::Target* target, const ci::geom::AttribSet& requestedAttribs)const override;

	WireRect* clone() const override { return new WireRect(*this); }

private:

	std::array<ci::vec4, 8> mPositions;

};