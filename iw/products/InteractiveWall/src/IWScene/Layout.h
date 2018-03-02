#pragma once

#include "Box2D/Box2D.h"
#include "cinder/Rect.h"
#include "cinder/Vector.h"
#include "LayoutTypes.h"

class Layout {
public:

	struct Product {
		ci::Rectf placement;
	};

	struct Cloud {
		ci::Rectf label;
		ci::Rectf placement;
		std::vector<Product> products;
	};

	struct Subcategory {
		ci::Rectf label;
		ci::Rectf placement;
		std::vector<Cloud> clouds;
		void clear() { clouds.clear(); label = ci::Rectf(); placement = ci::Rectf(); }
	};

	Layout(const ci::vec2& center, const ci::Rectf& bounds);

	Layout::Subcategory generate(const std::vector< std::pair< ci::Rectf/*label size*/, std::vector<ci::Rectf/*product size*/>>>& sub_category_data);

	//void draw();

	~Layout();

private:

	void update();

	bool runUntilSleeping();
	void createProduct(const ci::Rectf& product_size);
	void resetProductRect(int cloud, b2Body* product, int priority, bool circle = false);
	b2Body* createCloud(int index, const ci::Rectf& cloud_bounds);
	void resetCloudBounds(b2Body*body, const ci::Rectf& new_cloud_bounds, bool circle = false);
	void createLabel(const ci::Rectf& label_size, const ci::Rectf& cloud_bounds);
	static ci::Rectf calcBounds(b2Body* body);

	void createBounds();
	static ci::Rectf calcBounds(std::vector<b2Body*>::iterator begin, std::vector<b2Body*>::iterator end, const ci::vec2& center);

	b2World					*mWorld;

	ci::vec2 mCenter;
	ci::Rectf mBounds;

	std::vector< std::vector<ci::Rectf> > mProductSizeByCloudMap;
	std::vector<ci::Rectf> mCloudSizeMap;

	std::vector<b2Body*>	mProducts;
	std::vector<b2Body*>	mLabels;
	std::vector<b2Body*>	mClouds;
	std::vector< ci::ivec2 > mCloudProductOffsets;

	float mHeightOffset{ 0 };
	int mNumClouds{ 0 };

};