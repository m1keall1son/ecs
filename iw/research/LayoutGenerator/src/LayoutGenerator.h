#pragma once

#include "cinder/Vector.h"
#include <vector>

class LayoutGenerator {

public:

	enum Size { STANDARD, HIGHLIGHT, FEATURED };

	using Placement = std::pair< ci::vec2 /*center*/, float /*radius*/ >;
	using CloudStructure = std::vector<Size>; //priority order
	using CloudLayout = std::pair< Placement /*cloud anchor + radius*/ , std::vector<Placement> >;
	using SubcategoryStructure = std::vector< CloudStructure >; //priority order
	using SubcategoryLayout = std::vector< CloudLayout >;

	LayoutGenerator(const ci::vec2& layout_center, const ci::vec2& priority_epicenter, float upper_bound, float lower_bound, float standard = 40.f, float highlight = 60.f, float featured = 80.f ) : mLayoutCenter(layout_center), mPriorityEpicenter(priority_epicenter), mUpperBound(upper_bound), mLoweBound(lower_bound), mStandardSize(standard), mHighlightSize(highlight), mFeaturedSize(featured) {}

	SubcategoryLayout generateLayout( const SubcategoryStructure& sub_category_structure );
	CloudLayout generateLayout( const CloudStructure& cloud_structure );
	void prioritizeLayout( CloudLayout& cloud_layout, const CloudStructure& cloud_structure );

private:

	ci::vec2 mLayoutCenter;
	ci::vec2 mPriorityEpicenter;
	float mUpperBound;
	float mLoweBound;
	float mStandardSize;
	float mHighlightSize;
	float mFeaturedSize;
};