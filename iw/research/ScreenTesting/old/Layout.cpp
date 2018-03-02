
#include "Layout.h"
#include "cinder/Log.h"
#include "RectPacker.h"
#include "Attractor.h"
#include "SpiralFit.h"

using namespace ci;

ci::Rectf LayoutGen::FEATURED_BOUNDS = ci::Rectf(vec2(0), vec2(120, 160));
ci::Rectf LayoutGen::HIGHTLIGHT_BOUNDS = ci::Rectf(vec2(0), vec2(100, 130));
ci::Rectf LayoutGen::STANDARD_BOUNDS = ci::Rectf(vec2(0), vec2(80, 100));

LayoutGen::LayoutGen(const ci::vec2 & layout_center, const ci::Rectf & layout_bounds, float min_cloud_separation, float min_product_separation, float min_product_length_step, float max_product_length_step)
	: mLayoutCenter(layout_center), mLayoutBounds(layout_bounds), mMinCloudSeparation(min_cloud_separation), mMinProductSeparation(min_product_separation), mMinProductLengthStep(min_product_length_step), mMaxProductLengthStep(max_product_length_step)
{
}

LayoutGen::Subcategory LayoutGen::generate()
{
	Subcategory layout;
	layout.clouds.reserve(mUnplacedProducts.size());

	while (!mUnplacedProducts.empty()) {
		//place first one
		auto unplaced = mUnplacedProducts.front();
		mUnplacedProducts.pop_front();
		//place first cloud

		CI_LOG_V("num products: " << unplaced.products.size());

 		placeProducts(std::move(unplaced), layout);
	}

	auto fitter = SpiralFit(mLayoutCenter, mMinCloudSeparation);

	while (!mUnplacedClouds.empty()) {
		auto unplaced = mUnplacedClouds.front();
		mUnplacedClouds.pop_front();
		fitter.addRect(unplaced);
	}

	fitter.fit();
	//
	auto & placements = fitter.getPlacements();

	auto packer = RectPacker(mLayoutCenter, mLayoutBounds, mMinCloudSeparation);
	auto attractor = Attractor(mLayoutCenter, mMinCloudSeparation);

	for (auto & placement : placements) {
		packer.addRect(placement);
		attractor.addRect(placement);
	}

	for (int j = 0; j < 20; j++)
		packer.pack();

	attractor.attract();

	////offset positions of products to cloud position
	int i = 0;
	for (auto & cloud : layout.clouds) {
		cloud.label.offset(cloud.placement.getCenter());
		for (auto & product : cloud.products) {
			product.placement.offset(cloud.placement.getCenter());
		}
	}

	return std::move(layout);
}


void LayoutGen::placeProducts(LayoutGen::Cloud&& cloud, LayoutGen::Subcategory& layout)
{
	auto fitter = SpiralFit(vec2(0), mMinProductSeparation);


	int i = 0;
	bool added = false;
	for (auto & product : cloud.products) {

		/*if (cloud.products.size() == 1 && !added) {
			fitter.addRect(&cloud.label);
			fitter.addRect(&product.placement);
			added = true;
		}
		else if (cloud.products.size() == 2 && !added) {
			fitter.addRect(&product.placement);
			fitter.addRect(&cloud.label);
			added = true;
		}
		else if (cloud.products.size() == 3 && !added) {
			fitter.addRect(&cloud.label);
			fitter.addRect(&product.placement);
			added = true;
		}
		else {
			fitter.addRect(&product.placement);
			auto t = ci::lmap((float)(i++), 0.f, (float)cloud.products.size(), 0.f, 1.f);
			if (t > .7 && !added) {
				fitter.addRect(&cloud.label);
				added = true;
			}
		}*/

		fitter.addRect(&product.placement);

	}

	fitter.fit();

	layout.clouds.emplace_back(std::move(cloud));

	auto & cloud_layout = layout.clouds.back();
	cloud_layout.placement = fitter.calcFitArea();

	mUnplacedClouds.push_back(&cloud_layout.placement);

	cloud_layout.label.offset(-cloud_layout.placement.getCenter());
	for (auto & product : cloud_layout.products) {
		product.placement.offset(-cloud_layout.placement.getCenter());
	}

	auto side = fitter.fitOne( &cloud_layout.label );

	CI_LOG_V(std::to_string(side));

	cloud_layout.placement= fitter.calcFitArea();

	vec2 corner;

	if (side < 0.f)
		corner = vec2(cloud_layout.placement.getUpperLeft().x + cloud_layout.label.getSize().x*.5f, cloud_layout.placement.getUpperLeft().y + cloud_layout.label.getSize().y*.5f);
	else 
		corner = vec2(cloud_layout.placement.getUpperRight().x - cloud_layout.label.getSize().x*.5f, cloud_layout.placement.getUpperRight().y + cloud_layout.label.getSize().y*.5f);

	cloud_layout.label.offset( -cloud_layout.placement.getCenter());

	for (auto & product : cloud_layout.products) {
	product.placement.offset(-cloud_layout.placement.getCenter());
	}
	

}
