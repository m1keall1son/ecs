#include "LayoutGenerator.h"
#include "CirclePacker.h"
#include "cinder/app/App.h"
#include "cinder/Sphere.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

LayoutGenerator::SubcategoryLayout LayoutGenerator::generateLayout( const SubcategoryStructure & sub_category_structure)
{
	SubcategoryLayout layout;

	auto packer = CirclePacker(mLayoutCenter, 1.f);

	auto num = sub_category_structure.size();

	auto smallest_radius = std::numeric_limits<float>::max();

	for (int i = 0; i < num; i++) {

		layout.emplace_back( generateLayout( sub_category_structure[i] ) );

		auto radius = layout.back().first.second;

		if (radius < smallest_radius) smallest_radius = radius;

	}

	for (int i = 0; i < num; i++) {
		auto circle = ci::Sphere( vec3(mLayoutCenter, 0.) + vec3(randFloat(-1.f, 1.f), randFloat(-1.f, 1.f), 0), smallest_radius);
		packer.addCircle(circle);
	}

	//iterate packing until no overlaps
	//while (!packer.pack());
	for (int i = 0; i < 10; i++)
		packer.pack();

	//sort by distance to priority epicenter
	auto center = mPriorityEpicenter;
	packer.getCirlces().sort([center](const ci::Sphere& p1, const ci::Sphere& p2) {
		float d1 = glm::length2(ci::vec2(p1.getCenter()) - center);
		float d2 = glm::length2(ci::vec2(p2.getCenter()) - center);
		if (d1 < d2)
			return true;
		else if (d1 > d2)
			return false;
		else return false;
	});

	//resize to correct sizes
	int i = 0;
	for (auto & circle : packer.getCirlces()) {
		auto & cloud = layout[i++];
		circle.setRadius( cloud.first.second );
	}

	//iterate packing angin without sorting until no overlaps
	//while (!packer.pack(false));
	for (int i = 0; i < 10; i++)
		packer.pack(false);

	//place products
	i = 0;
	for ( auto & cloud : packer.getCirlces() ) {
		layout[i].first.first = vec2(cloud.getCenter());
		for ( auto & product : layout[i].second ) {
			product.first += vec2(cloud.getCenter());
		}
		prioritizeLayout( layout[i], sub_category_structure[i] );
		++i;
	}


	return std::move(layout);
}

LayoutGenerator::CloudLayout LayoutGenerator::generateLayout( const CloudStructure & cloud_structure )
{
	CloudLayout layout;

	auto packer = CirclePacker( vec2(0), 30.f );

	auto num = cloud_structure.size();

	for (int i = 0; i < num; i++) {

		auto circle = ci::Sphere( vec3( randFloat(-1.f, 1.f), randFloat(-1.f, 1.f), 0 ), mStandardSize );
		packer.addCircle(circle);

	}

	//iterate packing until no overlaps
	//while (!packer.pack());
	for (int i = 0; i < 10; i++)
		packer.pack();

	auto circles_copy = packer.getCirlces();

	
	//resize to correct sizes for bounds test
	int i = 0;
	for (auto & circle : packer.getCirlces()) {

		auto & product = cloud_structure[i++];

		switch (product)
		{
		case HIGHLIGHT: { circle.setRadius(mHighlightSize); } break;
		case FEATURED: {   circle.setRadius(mFeaturedSize); } break;
		default: /*Nothing*/ break;
		}

	}

	//iterate packing angin without sorting until no overlaps
	//while (!packer.pack(false));
	for (int i = 0; i < 10; i++)
		packer.pack(false);

	//get centers to calc bounds
	auto points = std::vector<vec3>();
	for (auto & circle : packer.getCirlces()) {
		points.push_back( vec3(circle.getCenter().x + circle.getRadius(), circle.getCenter().y , 0. ) );
		points.push_back( vec3(circle.getCenter().x - circle.getRadius(), circle.getCenter().y, 0.) );
		points.push_back(vec3(circle.getCenter().x, circle.getCenter().y - circle.getRadius(), 0.));
		points.push_back(vec3(circle.getCenter().x, circle.getCenter().y + circle.getRadius(), 0.));
	}

	//calc cloud radius
	auto bounds = ci::Sphere::calculateBoundingSphere(points);

	//use pre resized products in layout
	std::vector<Placement> placements;
	for (auto & circle : circles_copy) {
		placements.emplace_back(std::make_pair(vec2(circle.getCenter()), circle.getRadius()));
	}

	//populate layout
	layout.first = std::make_pair(vec2(bounds.getCenter()), bounds.getRadius());
	layout.second = std::move(placements);

	return std::move(layout);

}

void LayoutGenerator::prioritizeLayout( CloudLayout & cloud_layout, const CloudStructure & cloud_structure)
{

	auto packer = CirclePacker( cloud_layout.first.first, 30.f);

	for ( auto & product : cloud_layout.second ) {
		packer.addCircle( ci::Sphere( vec3(product.first, 0.), product.second ) );
	}

	//sort by distance to priority epicenter
	auto center = mPriorityEpicenter;
	packer.getCirlces().sort([center](const ci::Sphere& p1, const ci::Sphere& p2) {
		float d1 = glm::length2(ci::vec2(p1.getCenter()) - center);
		float d2 = glm::length2(ci::vec2(p2.getCenter()) - center);
		if (d1 < d2)
			return true;
		else if (d1 > d2)
			return false;
		else return false;
	});

	//resize to correct sizes
	int i = 0;
	for (auto & circle : packer.getCirlces()) {

		auto & product = cloud_structure[i++];

		switch (product)
		{
		case HIGHLIGHT: { circle.setRadius(mHighlightSize); } break;
		case FEATURED: {   circle.setRadius(mFeaturedSize); } break;
		default: /*Nothing*/ break;
		}

	}

	//iterate packing angin without sorting until no overlaps
	//while (!packer.pack(false));
	for (int i = 0; i < 10; i++)
		packer.pack(false);

	//resize and reposition the layout items 
	i = 0;
	for (auto & circle : packer.getCirlces() ) {
		auto & product = cloud_layout.second[i++];
		product.first = vec2(circle.getCenter());
		product.second = circle.getRadius();
	}

}
