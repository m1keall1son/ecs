#pragma once

#include "Box2D/Box2D.h"
#include "cinder/Rect.h"
#include "cinder/Vector.h"
#include "ciDebugDraw.h"
#include "LayoutTypes.h"

class b2Layout {
public:

	b2Layout( const ci::vec2& center, const ci::Rectf& bounds);

	Layout::Subcategory generate( const std::vector< std::pair< ci::Rectf/*label size*/, std::vector<ci::Rectf/*product size*/>>>& sub_category_data );

	void draw();
	void update();

private:

	void runUntilSleeping();
	void createProduct( const ci::Rectf& product_size );
	void resetProductRect( int cloud, b2Body* product, int priority);
	b2Body* createCloud( const ci::Rectf& cloud_bounds );
	void resetCloudBounds( b2Body*body, const ci::Rectf& new_cloud_bounds, bool circle = false);
	void createLabel( const ci::Rectf& label_size, const ci::Rectf& cloud_bounds );
	static ci::Rectf calcBounds(b2Body* body);

	void createBounds();
	static ci::Rectf calcBounds( std::vector<b2Body*>::iterator begin, std::vector<b2Body*>::iterator end, const ci::vec2& center );

	b2World					*mWorld;

	ci::vec2 mCenter;
	ci::Rectf mBounds;

	std::vector< std::vector<ci::Rectf> > mProductSizeByCloudMap;
	std::vector<ci::Rectf> mCloudSizeMap;

	std::vector<b2Body*>	mProducts;
	std::vector<b2Body*>	mLabels;
	std::vector<b2Body*>	mClouds;
	std::vector< ci::ivec2 > mCloudProductOffsets;

	ci::gl::b2DubugDraw		mDraw;

};