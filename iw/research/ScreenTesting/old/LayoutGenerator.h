#pragma once

#include "cinder/Vector.h"
#include <vector>
#include "cinder/Rect.h"

class LayoutGenerator {

public:

	enum Size { STANDARD, HIGHLIGHT, FEATURED };

	//using Placement = std::pair< ci::vec2 /*center*/, float /*radius*/ >;
	using Placement = std::pair< ci::vec2 /*center*/, ci::vec2 /*radius*/ >;
	using CloudStructure = std::vector<Size>; //priority order
	using CloudLayout = std::pair< Placement /*cloud anchor + radius*/, std::vector<Placement> >;
	using SubcategoryStructure = std::vector< CloudStructure >; //priority order
	using SubcategoryLayout = std::vector< CloudLayout >;

	//LayoutGenerator(const ci::vec2& layout_center, const ci::vec2& priority_epicenter, const ci::Rectf& bounds, float product_separation, float standard = 40.f, float highlight = 60.f, float featured = 80.f ) : mLayoutCenter(layout_center), mPriorityEpicenter(priority_epicenter), mBounds(bounds), mSeparation(product_separation), mStandardSize(standard), mHighlightSize(highlight), mFeaturedSize(featured) {}
	LayoutGenerator(const ci::vec2& layout_center, const ci::vec2& priority_epicenter, const ci::Rectf& bounds, float product_separation, const ci::vec2& standard = ci::vec2(60.f, 90.f), const ci::vec2& highlight = ci::vec2(120.f, 160.f), const ci::vec2& featured = ci::vec2(160.f, 180.f)) : mLayoutCenter(layout_center), mPriorityEpicenter(priority_epicenter), mBounds(bounds), mSeparation(product_separation), mStandardSize(standard), mHighlightSize(highlight), mFeaturedSize(featured) {}

	SubcategoryLayout generateLayout(const SubcategoryStructure& sub_category_structure);
	CloudLayout generateLayout(const CloudStructure& cloud_structure);
	void prioritizeLayout(CloudLayout& cloud_layout, const CloudStructure& cloud_structure);

private:

	ci::vec2 mLayoutCenter;
	ci::vec2 mPriorityEpicenter;
	ci::Rectf mBounds;
	float mSeparation;
	/*float mStandardSize;
	float mHighlightSize;
	float mFeaturedSize;*/
	ci::vec2 mStandardSize;
	ci::vec2 mHighlightSize;
	ci::vec2 mFeaturedSize;

};