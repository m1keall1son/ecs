#include "b2Layout.h"
#include "CollisionTypes.h"
#include "cinder/app/App.h"
#include "cinder/Log.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

b2Layout::b2Layout(const ci::vec2& center, const ci::Rectf& bounds ) : 
	mCenter(center),mBounds(bounds)
{
	mDraw.SetFlags(gl::b2DubugDraw::e_jointBit | gl::b2DubugDraw::e_shapeBit);

	//create b2 world

	b2Vec2 gravity(0.0f, 0.0f);
	mWorld = new b2World(gravity);
	mWorld->SetDebugDraw(&mDraw);


}

Layout::Subcategory b2Layout::generate(const std::vector< std::pair< ci::Rectf/*label size*/, std::vector<ci::Rectf/*product size*/>>>& sub_category_data)
{
	Layout::Subcategory subcategory;

	//populate clouds and calc bounds
	ci::Rectf smallest_cloud = mBounds;
	mCloudSizeMap.resize(sub_category_data.size());
	mCloudProductOffsets.resize(sub_category_data.size());
	int num = 0;
	int product_offset = 0;
	for (auto & cloud : sub_category_data) {

		auto label_size = cloud.first;

		for (auto & product : cloud.second) {
			createProduct(product);
		}

		runUntilSleeping();

		//replace with rects
		int j = 0;
		for (auto & product : mProducts) {
			resetProductRect(num, product, j++);
		}

		runUntilSleeping();

		//calc bounds
		auto begin = mProducts.begin();
		auto end = mProducts.begin();
		std::advance(begin, product_offset);
		std::advance(end, product_offset + cloud.second.size());

		auto bounds = calcBounds(begin, end, vec2(0));
		auto cloud_body = createCloud(bounds);
		mCloudProductOffsets[num] = ivec2(product_offset, product_offset + cloud.second.size());

		//create label
		createLabel(label_size, bounds);

		runUntilSleeping();

		//calc new bounds
		auto new_bounds = calcBounds(begin, end, vec2(0));
		auto label = mLabels.back();
		label_size.offsetCenterTo(vec2(label->GetWorldCenter().x, label->GetWorldCenter().y));
		new_bounds.include(label_size);

		mCloudSizeMap[num] = new_bounds;

		mProductSizeByCloudMap[num] = cloud.second;

		if (new_bounds.getWidth() < smallest_cloud.getWidth() || new_bounds.getHeight() < smallest_cloud.getHeight())
		{
			smallest_cloud = new_bounds;
		}

		//recenter products
		auto cloud_offset = b2Vec2(new_bounds.getCenter().x, new_bounds.getCenter().y);
		label->SetTransform(label->GetWorldCenter() - cloud_offset, label->GetAngle());
		label->SetActive(false);

		while (begin != end) {
			auto product = *begin++;
			product->SetTransform(product->GetWorldCenter() - cloud_offset, product->GetAngle());
			product->SetActive(false);
		}

		product_offset += cloud.second.size();
		num++;
	}

	//resize clouds to smallest possible size

	for (auto & cloud : mClouds) {
		resetCloudBounds(cloud, smallest_cloud, true);
	}

	//add perimeter

	createBounds();

	for (auto & cloud : mClouds) {
		cloud->SetActive(true);
		cloud->SetAwake(true);
	}

	//create basic layout

	runUntilSleeping();

	//sort closest to center
	auto center = mCenter;
	std::stable_sort(mClouds.begin(), mClouds.end(), [center](b2Body* first, b2Body* second) {

		float d1 = glm::length2(ci::vec2(first->GetWorldCenter().x, first->GetWorldCenter().y) - center);
		float d2 = glm::length2(ci::vec2(second->GetWorldCenter().x, second->GetWorldCenter().y) - center);
		if (d1 > d2)
			return true;
		else if (d1 < d2)
			return false;
		else return false;

	});

	//resize clouds in priority order and move products/labels to those new locations
	int label = 0;
	for (auto & cloud : mClouds) {

		cloud->SetActive(false);

		auto in_order_size = mCloudSizeMap[label];
		resetCloudBounds(cloud, in_order_size);

		auto range = mCloudProductOffsets[label];
		auto begin = mProducts.begin();
		auto end = mProducts.end();
		std::advance(begin, range.x);
		std::advance(end, range.y);
		while (begin != end) {
			auto product = *begin++;
			product->SetTransform(product->GetPosition() + cloud->GetWorldCenter(), product->GetAngle());
		}
		auto label_body = mLabels[label];
		label_body->SetTransform(label_body->GetPosition() + cloud->GetWorldCenter(), label_body->GetAngle());

		//sort products within cloud closest to center
		std::stable_sort(begin, end, [center](b2Body* first, b2Body* second) {
			float d1 = glm::length2(ci::vec2(first->GetWorldCenter().x, first->GetWorldCenter().y) - center);
			float d2 = glm::length2(ci::vec2(second->GetWorldCenter().x, second->GetWorldCenter().y) - center);
			if (d1 > d2)
				return true;
			else if (d1 < d2)
				return false;
			else return false;
		});

		begin = mProducts.begin();
		end = mProducts.end();
		std::advance(begin, range.x);
		std::advance(end, range.y);

		//resize products to correct sizes
		int p = 0;
		while (begin != end) {
			auto product = *begin++;
			resetProductRect(label, product, p++);
		}
		label++;
	}

	runUntilSleeping();

	//iterate and grab all positions

	label = 0;
	for (auto & cloud : mClouds) {
		auto label_body = mLabels[label];
		Layout::Cloud c;
		c.placement = calcBounds(cloud);
		c.label = calcBounds(label_body);
		auto range = mCloudProductOffsets[label++];
		auto begin = mProducts.begin();
		auto end = mProducts.end();
		std::advance(begin, range.x);
		std::advance(end, range.y);
		while (begin != end) {
			auto product = *begin++;
			Layout::Product prod;
			prod.placement = calcBounds(product);
			c.products.emplace_back(std::move(prod));
		}
		subcategory.clouds.push_back(c);
	}

	return std::move(subcategory);
}

void b2Layout::draw()
{
	gl::enableAlphaBlending();

	mDraw.draw();
}

void b2Layout::update()
{
	for (int i = 0; i < 5; ++i)
		mWorld->Step(1 / 30.0f, 10, 10);
}

void b2Layout::runUntilSleeping()
{
	int iterations = 0;
	int max_iterations = 10000;
	bool awake = true;
	while (awake) {
		update();
		bool anyone_awake = false;
		for ( auto & product : mProducts ) {
			if (!product->IsActive())
				continue;
			if (product->IsAwake())
				anyone_awake = true;
		}
		for (auto & label : mLabels) {
			if (!label->IsActive())
				continue;
			if (label->IsAwake())
				anyone_awake = true;
		}
		for (auto & cloud : mClouds) {
			if (!cloud->IsActive())
				continue;
			if (cloud->IsAwake())
				anyone_awake = true;
		}
		if (!anyone_awake)
			awake = false;

		if (iterations++ > max_iterations)
			throw ci::Exception("Too many iterations!");
	}
}

void b2Layout::createProduct(const ci::Rectf & product_size)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.;
	bodyDef.angularDamping = 1.;

	//splat around (0,0)
	bodyDef.position.Set( randFloat(-20, 20), randFloat(-20, 20) );

	b2Body *body = mWorld->CreateBody(&bodyDef);

	b2CircleShape circle;
	circle.m_radius = product_size.getHeight();

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
	fixtureDef.filter.categoryBits = PRODUCT;
	fixtureDef.filter.maskBits = BOUNDRY | PRODUCT;

	body->CreateFixture(&fixtureDef);
	
	mProducts.push_back(body);

}

void b2Layout::resetProductRect(int cloud, b2Body * product, int priority)
{
	product->DestroyFixture(product->GetFixtureList());

	b2PolygonShape dynamicBox;

	Rectf rect = mProductSizeByCloudMap[cloud][priority];
	dynamicBox.SetAsBox(rect.getWidth(), rect.getHeight());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
	fixtureDef.filter.categoryBits = PRODUCT;
	fixtureDef.filter.maskBits = BOUNDRY | PRODUCT;

	product->SetFixedRotation(true);

	product->CreateFixture(&fixtureDef);
}

b2Body* b2Layout::createCloud(const ci::Rectf & cloud_bounds)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.;
	bodyDef.angularDamping = 1.;

	//splat around center
	bodyDef.position.Set( mCenter.x + randFloat(-120, 120), mCenter.x + randFloat(-120, 120) );

	b2Body *body = mWorld->CreateBody(&bodyDef);

	b2CircleShape rect;
	rect.m_radius = glm::min(cloud_bounds.getWidth(), cloud_bounds.getHeight());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rect;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
	fixtureDef.filter.categoryBits = CLOUD;
	fixtureDef.filter.maskBits = CLOUD | BOUNDRY;

	body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);
	body->SetActive(false);
	
	mClouds.push_back(body);

	return body;
}

void b2Layout::resetCloudBounds(b2Body * body, const ci::Rectf & new_cloud_bounds, bool circle)
{
	body->DestroyFixture(body->GetFixtureList());

	if (circle) {

		b2CircleShape dynamicBox;
		dynamicBox.m_radius = glm::min(new_cloud_bounds.getWidth(),new_cloud_bounds.getHeight());

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.5f; // bounce
		fixtureDef.filter.categoryBits = CLOUD;
		fixtureDef.filter.maskBits = CLOUD | BOUNDRY;

		body->SetFixedRotation(true);

		body->CreateFixture(&fixtureDef);
	}
	else {
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(new_cloud_bounds.getWidth(), new_cloud_bounds.getHeight());

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.5f; // bounce
		fixtureDef.filter.categoryBits = CLOUD;
		fixtureDef.filter.maskBits = CLOUD | BOUNDRY;

		body->SetFixedRotation(true);

		body->CreateFixture(&fixtureDef);
	}
}

void b2Layout::createLabel( const ci::Rectf& label_size, const ci::Rectf & cloud_bounds )
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.;
	bodyDef.angularDamping = 1.;

	//place at one of the top corners
	auto rand = randFloat();

	if (rand > .5) {
		bodyDef.position.Set(cloud_bounds.getUpperLeft().x + label_size.getWidth()*.5f, cloud_bounds.getUpperLeft().y + label_size.getHeight()*.5f);
	}
	else{
		bodyDef.position.Set(cloud_bounds.getUpperRight().x - label_size.getWidth()*.5f, cloud_bounds.getUpperRight().y + label_size.getHeight()*.5f);
	}

	b2Body *body = mWorld->CreateBody(&bodyDef);

	b2PolygonShape rect;
	rect.SetAsBox(label_size.getWidth(), label_size.getHeight());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rect;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
	fixtureDef.filter.categoryBits = PRODUCT;
	fixtureDef.filter.maskBits = BOUNDRY | PRODUCT;

	body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);

	mLabels.push_back(body);

}

ci::Rectf b2Layout::calcBounds(b2Body * body)
{
	Rectf bounds = Rectf(vec2(body->GetWorldCenter().x, body->GetWorldCenter().y) - .5f, vec2(body->GetWorldCenter().x, body->GetWorldCenter().y) + .5f);
	if (auto poly_shape = dynamic_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape())) {
		for (int i = 0; i < poly_shape->GetVertexCount(); i++) {
			vec2 point = vec2(poly_shape->GetVertex(i).x, poly_shape->GetVertex(i).y);
			point += vec2(body->GetWorldCenter().x, body->GetWorldCenter().y);
			bounds.include(point);
		}
	}
	else if (auto circle_shape = dynamic_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape())) {
		auto rad = circle_shape->m_radius;
		auto body_center = vec2(body->GetWorldCenter().x, body->GetWorldCenter().y);
		auto t = body_center + vec2(0, 1)*rad;
		auto b = body_center + vec2(0, -1)*rad;
		auto l = body_center + vec2(-1, 0)*rad;
		auto r = body_center + vec2(1, 0)*rad;
		bounds.include(t);
		bounds.include(b);
		bounds.include(l);
		bounds.include(r);
	}
	else {
		CI_LOG_E("Unknown shape");
	}
	return std::move(bounds);
}

void b2Layout::createBounds()
{
	//create bounds

	b2EdgeShape width;
	width.Set(b2Vec2(0, 0), b2Vec2(mBounds.getWidth(), 0));

	b2EdgeShape height;
	height.Set(b2Vec2(0, 0), b2Vec2(0, mBounds.getHeight()));

	b2FixtureDef widthDef;
	widthDef.shape = &width;
	widthDef.density = 1.0f;
	widthDef.friction = 0.3f;
	widthDef.restitution = 0.5f; // bounce
	widthDef.filter.categoryBits = BOUNDRY;
	widthDef.filter.maskBits = BOUNDRY | PRODUCT;

	b2FixtureDef heightDef;
	heightDef.shape = &height;
	heightDef.density = 1.0f;
	heightDef.friction = 0.3f;
	heightDef.restitution = 0.5f; // bounce
	heightDef.filter.categoryBits = BOUNDRY;
	heightDef.filter.maskBits = BOUNDRY | PRODUCT;

	b2BodyDef floorDef;
	floorDef.type = b2_staticBody;
	floorDef.position = b2Vec2(mBounds.getCenter().x, mBounds.getLowerLeft().y);

	b2Body * ground = mWorld->CreateBody(&floorDef);
	ground->CreateFixture(&widthDef);

	b2BodyDef ceiling_def;
	ceiling_def.type = b2_staticBody;
	ceiling_def.position = b2Vec2(mBounds.getCenter().x, mBounds.getUpperLeft().y);

	b2Body * ceiling = mWorld->CreateBody(&ceiling_def);
	ceiling->CreateFixture(&widthDef);

	b2BodyDef left_wall_def;
	left_wall_def.type = b2_staticBody;
	left_wall_def.position = b2Vec2(mBounds.getUpperLeft().x, mBounds.getCenter().y);

	b2Body * left_wall = mWorld->CreateBody(&left_wall_def);
	left_wall->CreateFixture(&heightDef);

	b2BodyDef right_wall_def;
	right_wall_def.type = b2_staticBody;
	right_wall_def.position = b2Vec2(mBounds.getUpperRight().x, mBounds.getCenter().y);

	b2Body * right_wall = mWorld->CreateBody(&right_wall_def);
	right_wall->CreateFixture(&heightDef);
}


ci::Rectf b2Layout::calcBounds(std::vector<b2Body*>::iterator begin, std::vector<b2Body*>::iterator end, const ci::vec2& center)
{
	Rectf bounds = Rectf( vec2(center)-.5f, vec2(center)+.5f );
	while ( begin != end ) {
		auto body = *begin++;
		bounds.include( calcBounds(body) );
	}
	return std::move(bounds);
}
