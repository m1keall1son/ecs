#pragma once

#include "cinder/Rand.h"
#include "cinder/Rect.h"
#include <list>

class LayoutGen {
public:

	struct Product {
		ci::Rectf placement;
	};

	struct Cloud {
		ci::Rectf label;
		ci::Rectf placement;
		std::list<Product> products;
	};

	struct Subcategory {
		ci::Rectf label;
		ci::Rectf placement;
		std::vector<Cloud> clouds;
		void clear() { clouds.clear(); label = ci::Rectf(); placement = ci::Rectf(); }
	};

	static ci::Rectf HIGHTLIGHT_BOUNDS;
	static ci::Rectf FEATURED_BOUNDS;
	static ci::Rectf STANDARD_BOUNDS;

	LayoutGen(const ci::vec2& layout_center, const ci::Rectf& layout_bounds, float min_cloud_separation, float min_product_separation, float min_product_length_step = .02f, float max_product_length_step = 10.f);
	Subcategory generate();
	void addCloud(const Cloud& cloud) { mUnplacedProducts.push_back(cloud); }

private:

	void placeProducts(Cloud&& cloud, Subcategory& layout);

	std::list< Cloud > mUnplacedProducts;
	std::list< ci::Rectf* > mUnplacedClouds;

	ci::vec2 mLayoutCenter;

	ci::Rectf mLayoutBounds;
	float mMinCloudLengthStep;
	float mMaxCloudLengthStep;
	float mMinCloudSeparation;

	float mMinProductLengthStep;
	float mMaxProductLengthStep;
	float mMinProductSeparation;

};