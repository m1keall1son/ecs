#include "LayoutGenerator.h"
#include "CirclePacker.h"
#include "RectPacker.h"
#include "cinder/app/App.h"
#include "cinder/Sphere.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

LayoutGenerator::SubcategoryLayout LayoutGenerator::generateLayout( const SubcategoryStructure & sub_category_structure)
{
	SubcategoryLayout layout;

	////auto packer = CirclePacker(mLayoutCenter, mBounds, 1.f);
	//auto packer = RectPacker(mLayoutCenter, mBounds, 1.f);

	//auto num = sub_category_structure.size();

	///*auto smallest_radius = std::numeric_limits<float>::max();

	//for (int i = 0; i < num; i++) {

	//	layout.emplace_back( generateLayout( sub_category_structure[i] ) );

	//	auto radius = layout.back().first.second;

	//	if (radius < smallest_radius) smallest_radius = radius;

	//}*/

	//auto smallest_x = std::numeric_limits<float>::max();
	//auto smallest_y = std::numeric_limits<float>::max();

	//for (int i = 0; i < num; i++) {

	//	layout.emplace_back(generateLayout(sub_category_structure[i]));

	//	auto radius = layout.back().first.second;

	//	if (radius.x < smallest_x) smallest_x = radius.x;
	//	if (radius.y < smallest_y) smallest_y = radius.y;

	//}

	//for (int i = 0; i < num; i++) {
	////	auto circle = ci::Sphere( vec3(mLayoutCenter, 0.) + vec3(randFloat(-1.f, 1.f), randFloat(-1.f, 1.f), 0), smallest_radius);
	////	packer.addCircle(circle);

	//	auto pos = vec2(randFloat(-1.f, 1.f), randFloat(-1.f, 1.f));
	//	auto rect = ci::Rectf( vec2(mLayoutCenter - vec2(smallest_x, smallest_y)*.5f) + pos, vec2(mLayoutCenter + vec2(smallest_x, smallest_y)*.5f) + pos );
	//	//packer.addRect(rect);

	//}

	////iterate packing until no overlaps
	//for (int i = 0; i < 5; i++)
	//	packer.pack();

	////sort by distance to priority epicenter

	///*auto center = mPriorityEpicenter;
	//packer.getCircles().sort([center](const ci::Sphere& p1, const ci::Sphere& p2) {
	//	float d1 = glm::length2(ci::vec2(p1.getCenter()) - center);
	//	float d2 = glm::length2(ci::vec2(p2.getCenter()) - center);
	//	if (d1 < d2)
	//		return true;
	//	else if (d1 > d2)
	//		return false;
	//	else return false;
	//});*/

	//auto center = mPriorityEpicenter;
	//packer.getRects().sort([center](const ci::Rectf& p1, const ci::Rectf& p2) {
	//	float d1 = glm::length2(ci::vec2(p1.getCenter()) - center);
	//	float d2 = glm::length2(ci::vec2(p2.getCenter()) - center);
	//	if (d1 < d2)
	//		return true;
	//	else if (d1 > d2)
	//		return false;
	//	else return false;
	//});

	////resize to correct sizes
	////int i = 0;
	////for (auto & circle : packer.getCircles()) {
	////	auto & cloud = layout[i++];
	////	circle.setRadius( cloud.first.second );
	////}

	//int i = 0;
	//for (auto & rect : packer.getRects()) {
	//	auto & cloud = layout[i++];
	//	rect.set( rect.getCenter().x - cloud.first.second.x*.5f, rect.getCenter().y - cloud.first.second.y*.5f, rect.getCenter().x + cloud.first.second.x*.5f, rect.getCenter().y + cloud.first.second.y*.5f);
	//}

	////iterate packing angin without sorting until no overlaps
	//for (int i = 0; i < 5; i++)
	//	packer.pack(false);

	////place products
	//i = 0;
	///*for ( auto & cloud : packer.getCirlces() ) {
	//	layout[i].first.first = vec2(cloud.getCenter());
	//	for ( auto & product : layout[i].second ) {
	//		product.first += vec2(cloud.getCenter());
	//	}
	//	prioritizeLayout( layout[i], sub_category_structure[i] );
	//	++i;
	//}*/

	//for (auto & cloud : packer.getRects()) {
	//	layout[i].first.first = vec2(cloud.getCenter());
	//	for (auto & product : layout[i].second) {
	//		product.first += vec2(cloud.getCenter());
	//	}
	//	prioritizeLayout(layout[i], sub_category_structure[i]);
	//	++i;
	//}

	////Pack all the products independently
	//packer.clear();

	///*for (auto & cloud : layout ) {
	//	for (auto & product : cloud.second) {
	//		auto radius = product.second;
	//		auto position = product.first;
	//		packer.addCircle( Sphere( vec3( position, 0), radius ) );
	//	}
	//}*/

	//for (auto & cloud : layout) {
	//	for (auto & product : cloud.second) {
	//		auto size = product.second;
	//		auto position = product.first;
	//		packer.addRect( Rectf( position - size*.5f, position + size*.5f ) );
	//	}
	//}

	//for (int j = 0; j < 5; j++)
	//	packer.pack(false);

	//i = 0;
	////for (auto & cloud : layout) {
	////	for (auto & product : cloud.second) {
	////		auto & position = product.first;
	////		auto iter = packer.getCirlces().begin();
	////		std::advance(iter, i++);
	////		position = vec2(iter->getCenter());
	////	}
	////}

	//for (auto & cloud : layout) {
	//	for (auto & product : cloud.second) {
	//		auto & position = product.first;
	//		auto iter = packer.getRects().begin();
	//		std::advance(iter, i++);
	//		position = vec2(iter->getCenter());
	//	}
	//}

	return std::move(layout);
}

LayoutGenerator::CloudLayout LayoutGenerator::generateLayout( const CloudStructure & cloud_structure )
{
	CloudLayout layout;

	////auto packer = CirclePacker( vec2(0), Rectf( -getWindowCenter(), getWindowCenter() ), mSeparation );
	//auto packer = RectPacker(vec2(0), Rectf(-getWindowCenter(), getWindowCenter()), mSeparation);

	//auto num = cloud_structure.size();

	//for (int i = 0; i < num; i++) {

	//	//auto circle = ci::Sphere( vec3( randFloat(-1.f, 1.f), randFloat(-1.f, 1.f), 0 ), mStandardSize );
	//	//packer.addCircle(circle);
	//	auto pos = vec2(randFloat(-1.f, 1.f), randFloat(-1.f, 1.f));
	//	auto rect = ci::Rectf( pos - mStandardSize * .5f, pos + mStandardSize * .5f );
	//	packer.addRect(rect);

	//}

	////iterate packing until no overlaps
	////while (!packer.pack());
	//for (int i = 0; i < 5; i++)
	//	packer.pack();

	////auto circles_copy = packer.getCirlces();
	//auto rects_copy = packer.getRects();


	////resize to correct sizes for bounds test
	////int i = 0;
	////for (auto & circle : packer.getCirlces()) {

	////	auto & product = cloud_structure[i++];

	////	switch (product)
	////	{
	////	case HIGHLIGHT: { circle.setRadius(mHighlightSize); } break;
	////	case FEATURED: {   circle.setRadius(mFeaturedSize); } break;
	////	default: /*Nothing*/ break;
	////	}

	////}

	//int i = 0;
	//for (auto & rect : packer.getRects()) {

	//	auto & product = cloud_structure[i++];

	//	switch (product)
	//	{
	//	case HIGHLIGHT: { rect.set( rect.getCenter().x - mHighlightSize.x *.5f, rect.getCenter().y - mHighlightSize.y *.5f, rect.getCenter().x + mHighlightSize.x *.5f, rect.getCenter().y + mHighlightSize.y *.5f); } break;
	//	case FEATURED: {   rect.set(rect.getCenter().x - mFeaturedSize.x *.5f, rect.getCenter().y - mFeaturedSize.y *.5f, rect.getCenter().x + mFeaturedSize.x *.5f, rect.getCenter().y + mFeaturedSize.y *.5f); } break;
	//	default: /*Nothing*/ break;
	//	}

	//}

	////iterate packing angin without sorting until no overlaps
	////while (!packer.pack(false));
	//for (int i = 0; i < 5; i++)
	//	packer.pack(false);

	////get centers to calc bounds
	////auto points = std::vector<vec3>();
	////for (auto & circle : packer.getCirlces()) {
	////	points.push_back( vec3(circle.getCenter().x + circle.getRadius(), circle.getCenter().y , 0. ) );
	////	points.push_back( vec3(circle.getCenter().x - circle.getRadius(), circle.getCenter().y, 0.) );
	////	points.push_back(vec3(circle.getCenter().x, circle.getCenter().y - circle.getRadius(), 0.));
	////	points.push_back(vec3(circle.getCenter().x, circle.getCenter().y + circle.getRadius(), 0.));
	////}

	//////calc cloud radius
	////auto bounds = ci::Sphere::calculateBoundingSphere(points);

	//ci::Rectf bounds = Rectf(vec2(-1), vec2(1));

	//for (auto & rect : packer.getRects()) {
	//	bounds.include(rect);
	//}


	////use pre resized products in layout
	////std::vector<Placement> placements;
	////for (auto & circle : circles_copy) {
	////	placements.emplace_back(std::make_pair(vec2(circle.getCenter()), circle.getRadius()));
	////}

	//std::vector<Placement> placements;
	//for (auto & rect : rects_copy) {
	//	placements.emplace_back(std::make_pair(vec2(rect.getCenter()), rect.getSize()));
	//}

	////populate layout
	////layout.first = std::make_pair(vec2(bounds.getCenter()), bounds.getRadius());
	//layout.first = std::make_pair(vec2(bounds.getCenter()), bounds.getSize());
	//layout.second = std::move(placements);

	return std::move(layout);

}

void LayoutGenerator::prioritizeLayout( CloudLayout & cloud_layout, const CloudStructure & cloud_structure)
{

	/*auto packer = CirclePacker( cloud_layout.first.first, mBounds, mSeparation);

	for ( auto & product : cloud_layout.second ) {
		packer.addCircle( ci::Sphere( vec3(product.first, 0.), product.second ) );
	}
*/
	//auto packer = RectPacker(cloud_layout.first.first, mBounds, mSeparation);

	//for (auto & product : cloud_layout.second) {
	//	packer.addRect( ci::Rectf( product.first - product.second*.5f, product.first + product.second*.5f ) );
	//}

	////sort by distance to priority epicenter
	////auto center = mPriorityEpicenter;
	////packer.getCirlces().sort([center](const ci::Sphere& p1, const ci::Sphere& p2) {
	////	float d1 = glm::length2(ci::vec2(p1.getCenter()) - center);
	////	float d2 = glm::length2(ci::vec2(p2.getCenter()) - center);
	////	if (d1 < d2)
	////		return true;
	////	else if (d1 > d2)
	////		return false;
	////	else return false;
	////});

	//auto center = mPriorityEpicenter;
	//packer.getRects().sort([center](const ci::Rectf& p1, const ci::Rectf& p2) {
	//	float d1 = glm::length2(ci::vec2(p1.getCenter()) - center);
	//	float d2 = glm::length2(ci::vec2(p2.getCenter()) - center);
	//	if (d1 < d2)
	//		return true;
	//	else if (d1 > d2)
	//		return false;
	//	else return false;
	//});

	////resize to correct sizes
	////int i = 0;
	////for (auto & circle : packer.getCirlces()) {

	////	auto & product = cloud_structure[i++];

	////	switch (product)
	////	{
	////	case HIGHLIGHT: { circle.setRadius(mHighlightSize); } break;
	////	case FEATURED: {   circle.setRadius(mFeaturedSize); } break;
	////	default: /*Nothing*/ break;
	////	}

	////}

	//int i = 0;
	//for (auto & rect : packer.getRects()) {

	//	auto & product = cloud_structure[i++];

	//	switch (product)
	//	{
	//	case HIGHLIGHT: { rect.set(rect.getCenter().x - mHighlightSize.x *.5f, rect.getCenter().y - mHighlightSize.y *.5f, rect.getCenter().x + mHighlightSize.x *.5f, rect.getCenter().y + mHighlightSize.y *.5f); } break;
	//	case FEATURED: {   rect.set(rect.getCenter().x - mFeaturedSize.x *.5f, rect.getCenter().y - mFeaturedSize.y *.5f, rect.getCenter().x + mFeaturedSize.x *.5f, rect.getCenter().y + mFeaturedSize.y *.5f); } break;
	//	default: /*Nothing*/ break;
	//	}

	//}

	////iterate packing angin without sorting until no overlaps
	////while (!packer.pack(false));
	//for (int i = 0; i < 5; i++)
	//	packer.pack(false);

	////resize and reposition the layout items 
	//i = 0;
	////for (auto & circle : packer.getCirlces() ) {
	////	auto & product = cloud_layout.second[i++];
	////	product.first = vec2(circle.getCenter());
	////	product.second = circle.getRadius();
	////}

	//for (auto & rect : packer.getRects()) {
	//	auto & product = cloud_layout.second[i++];
	//	product.first = vec2(rect.getCenter());
	//	product.second = rect.getSize();
	//}

}
