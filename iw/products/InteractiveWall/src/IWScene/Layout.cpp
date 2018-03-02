#include "Layout.h"

#include "CollisionTypes.h"
#include "cinder/app/App.h"
#include "cinder/Log.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

Layout::Layout(const ci::vec2& center, const ci::Rectf& bounds) :
	mCenter(center), mBounds(bounds)
{
	//mDraw.SetFlags(gl::b2DubugDraw::e_jointBit | gl::b2DubugDraw::e_shapeBit);

	//create b2 world

	b2Vec2 gravity(0.0f, 0.0f);
	mWorld = new b2World(gravity);
	//mWorld->SetDebugDraw(&mDraw);


}

Layout::Subcategory Layout::generate(const std::vector< std::pair< ci::Rectf/*label size*/, std::vector<ci::Rectf/*product size*/>>>& sub_category_data)
{
	Layout::Subcategory subcategory;

	mNumClouds = sub_category_data.size();

	//populate clouds and calc bounds
	ci::Rectf smallest_cloud = mBounds;

	mCloudSizeMap.resize(sub_category_data.size());
	mCloudProductOffsets.resize(sub_category_data.size());
	mProductSizeByCloudMap.resize(sub_category_data.size());

	int num = 0;
	int product_offset = 0;
	for (auto& cloud : sub_category_data) {

		cinder::RectT<float> label_size = cloud.first;

		mProductSizeByCloudMap[num] = cloud.second;

		for (auto& product : cloud.second) {
			createProduct(product);
		}

		runUntilSleeping();


		////replace with rects
		int j = 0;
		for (auto& product : cloud.second) {
			resetProductRect(num, mProducts[product_offset + j], j, false);
			++j;
		}

		runUntilSleeping();

		//calc bounds
		auto begin = mProducts.begin();
		auto end = mProducts.begin();
		std::advance(begin, product_offset);
		std::advance(end, product_offset + cloud.second.size());

		cinder::RectT<float> bounds = calcBounds(begin, end, vec2(0));

		//create label
		createLabel(label_size, bounds);

		runUntilSleeping();

		//calc new bounds
		cinder::RectT<float> new_bounds = calcBounds(begin, end, vec2(0));
		b2Body* label = mLabels.back();
		label_size.offsetCenterTo(vec2(label->GetWorldCenter().x, label->GetWorldCenter().y));
		new_bounds.include(label_size);

		mCloudSizeMap[num] = new_bounds;

		b2Body* cloud_body = createCloud(num, new_bounds);

		mCloudProductOffsets[num] = ivec2(product_offset, product_offset + cloud.second.size());
		//resetCloudBounds(cloud_body, , false);

		if (new_bounds.getWidth() < smallest_cloud.getWidth() || new_bounds.getHeight() < smallest_cloud.getHeight())
		{
			smallest_cloud = new_bounds;
		}

		//recenter products
		b2Vec2 cloud_offset = b2Vec2(new_bounds.getCenter().x, new_bounds.getCenter().y);
		label->SetTransform(label->GetWorldCenter() - cloud_offset, label->GetAngle());
		label->SetActive(false);

		while (begin != end) {
			b2Body* product = *begin++;
			product->SetTransform(product->GetWorldCenter() - cloud_offset, product->GetAngle());
			product->SetActive(false);
		}

		product_offset += cloud.second.size();
		++num;
	}


	//resize clouds to smallest possible size

	for (auto& cloud : mClouds) {
		resetCloudBounds(cloud, smallest_cloud, true);
	}

	//add perimeter

	createBounds();

	runUntilSleeping();

	int label = 0;

	for (auto& cloud : mClouds) {
		cloud->SetActive(true);
		cloud->SetAwake(true);
	}

	//sort closest to center
	glm::vec2 center = mCenter;
	std::stable_sort(mClouds.begin(), mClouds.end(), [center](b2Body* first, b2Body* second) {

		float d1 = glm::length2(ci::vec2(first->GetWorldCenter().x, first->GetWorldCenter().y) - center);
		float d2 = glm::length2(ci::vec2(second->GetWorldCenter().x, second->GetWorldCenter().y) - center);
		if (d1 > d2) {
			return true;
		}
		else if (d1 < d2) {
			return false;
		}
		else {
			return false;
		}
	});

	//resize clouds in priority order and move products/labels to those new locations

	for (auto& cloud : mClouds) {

		cinder::Rectf in_order_size = mCloudSizeMap[label];
		resetCloudBounds(cloud, in_order_size, false);
		label++;
	}

	runUntilSleeping();

	label = 0;
	for (auto& cloud : mClouds) {

		cloud->SetActive(false);

		glm::ivec2 range = mCloudProductOffsets[label];
		auto begin = mProducts.begin();
		auto end = mProducts.begin();
		std::advance(begin, range.x);
		std::advance(end, range.y);
		while (begin != end) {
			auto product = *begin++;

			CI_LOG_V("cloud center [" << cloud->GetWorldCenter().x << ", " << cloud->GetWorldCenter().y << "]");

			product->SetTransform(product->GetPosition() + cloud->GetWorldCenter(), product->GetAngle());
			product->SetActive(true);
			product->SetAwake(true);
		}
		b2Body* label_body = mLabels[label];
		label_body->SetTransform(label_body->GetPosition() + cloud->GetWorldCenter(), label_body->GetAngle());
		label_body->SetActive(true);
		label_body->SetAwake(true);

		//sort products within cloud closest to center
		std::stable_sort(begin, end, [center](b2Body* first, b2Body* second) {
			float d1 = glm::length2(ci::vec2(first->GetWorldCenter().x, first->GetWorldCenter().y) - center);
			float d2 = glm::length2(ci::vec2(second->GetWorldCenter().x, second->GetWorldCenter().y) - center);
			if (d1 > d2) {
				return true;
			}
			else if (d1 < d2) {
				return false;
			}
			else {
				return false;
			}
		}
		);

		begin = mProducts.begin();
		end = mProducts.begin();
		std::advance(begin, range.x);
		std::advance(end, range.y);

		//resize products to correct sizes
		int p = 0;
		while (begin != end) {
			b2Body* product = *begin++;
			resetProductRect(label, product, p++, false);

		}
		label++;
	}

	runUntilSleeping();

	////iterate and grab all positions

	label = 0;
	for (auto& cloud : mClouds) {
		b2Body* label_body = mLabels[label];
		Layout::Cloud c;

		glm::ivec2 range = mCloudProductOffsets[label];
		auto begin = mProducts.begin();
		auto end = mProducts.begin();
		std::advance(begin, range.x);
		std::advance(end, range.y);
		c.placement = calcBounds(begin, end, vec2(cloud->GetWorldCenter().x, cloud->GetWorldCenter().y ) );
		c.label = calcBounds(label_body);
		c.placement.include(c.label);
		
		begin = mProducts.begin();
		end = mProducts.begin();
		std::advance(begin, range.x);
		std::advance(end, range.y);
		while (begin != end) {
			b2Body* product = *begin++;
			Layout::Product prod;
			prod.placement = calcBounds(product);
			c.products.emplace_back(std::move(prod));
		}
		subcategory.clouds.push_back(c);
		label++;
	}

	return std::move(subcategory);
}

//void Layout::draw()
//{
//	//gl::clear();
//	gl::enableAlphaBlending();
//	mWorld->DrawDebugData();
//	mDraw.draw();
//	gl::disableAlphaBlending();
//}

void Layout::update()
{
	for (int i = 0; i < 5; ++i)
		mWorld->Step(1 / 30.0f, 10, 10);
}

Layout::~Layout()
{
	for (auto& product : mProducts) {
		mWorld->DestroyBody(product);
	}
	for (auto& label : mLabels) {
		mWorld->DestroyBody(label);
	}
	for (auto& cloud : mClouds) {
		mWorld->DestroyBody(cloud);
	}
	delete mWorld;
}

bool Layout::runUntilSleeping()
{
	int iterations = 0;
	int max_iterations = 1000;
	bool awake = true;
	while (awake) {
		update();
		bool anyone_awake = false;
		for (auto& product : mProducts) {
			if (!product->IsActive())
				continue;
			if (product->IsAwake())
				anyone_awake = true;
		}
		for (auto& label : mLabels) {
			if (!label->IsActive())
				continue;
			if (label->IsAwake())
				anyone_awake = true;
		}
		for (auto& cloud : mClouds) {
			if (!cloud->IsActive())
				continue;
			if (cloud->IsAwake())
				anyone_awake = true;
		}
		if (!anyone_awake)
			awake = false;

		if (iterations++ > max_iterations)
			return false;
		//throw ci::Exception("Too many iterations!");
	}

	return true;
}

void Layout::createProduct(const ci::Rectf & product_size)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.;
	bodyDef.angularDamping = 1.;

	//splat around (0,0)
	bodyDef.position.Set(randFloat(-20, 20), randFloat(-20, 20));

	b2Body* body = mWorld->CreateBody(&bodyDef);

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

void Layout::resetProductRect(int cloud, b2Body * product, int priority, bool circle)
{
	product->DestroyFixture(product->GetFixtureList());


	Rectf rect = mProductSizeByCloudMap[cloud][priority];

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
	fixtureDef.filter.categoryBits = PRODUCT;
	fixtureDef.filter.maskBits = BOUNDRY | PRODUCT;

	b2PolygonShape dynamicBox;
	b2CircleShape circle_shape;

	if (circle) {
		circle_shape.m_radius = glm::max(rect.getHeight(), rect.getWidth());
		fixtureDef.shape = &circle_shape;
	}
	else
	{
		dynamicBox.SetAsBox(rect.getWidth(), rect.getHeight());
		fixtureDef.shape = &dynamicBox;
		product->SetFixedRotation(true);
	}

	product->CreateFixture(&fixtureDef);
}

b2Body* Layout::createCloud(int index, const ci::Rectf & cloud_bounds)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.;
	bodyDef.angularDamping = 1.;

	//set up a uniform grid 

	float x = 0;
	float y = 0;

	int width = 2;
	double height = ceil(mNumClouds / 2);

	x = (index % width) ;
	y = floor( (index ) / (int)width );

	ci::vec2 step = ci::vec2((mBounds.getWidth()) / (float)width, (mBounds.getHeight()*.5f) / (float)height);

	x = (x * step.x) + mCenter.x - step.x*.5f;
	y = (y * step.y) + mCenter.y - step.y*.5f;

	bodyDef.position.Set(x , y);

	b2Body *body = mWorld->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;

	b2CircleShape circle;
	circle.m_radius = glm::min(cloud_bounds.getWidth()*.5f, cloud_bounds.getHeight()*.5f);
	fixtureDef.shape = &circle;

	//b2PolygonShape rect;
	//rect.SetAsBox( cloud_bounds.getWidth(), cloud_bounds.getHeight() );
	//fixtureDef.shape = &rect;

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce
	fixtureDef.filter.categoryBits = CLOUD;
	fixtureDef.filter.maskBits = CLOUD | BOUNDRY;

	body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);
	body->SetActive(false);

	mClouds.push_back(body);

	mHeightOffset += randFloat(400, 500);

	return body;
}

void Layout::resetCloudBounds(b2Body * body, const ci::Rectf & new_cloud_bounds, bool circle)
{
	body->DestroyFixture(body->GetFixtureList());

	if (circle) {

		b2CircleShape dynamicBox;
		dynamicBox.m_radius = glm::max(new_cloud_bounds.getWidth()*.5f, new_cloud_bounds.getHeight()*.5f);

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
		dynamicBox.SetAsBox(new_cloud_bounds.getWidth()*.5f, new_cloud_bounds.getHeight()*.5f);

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

void Layout::createLabel(const ci::Rectf& label_size, const ci::Rectf & cloud_bounds)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.;
	bodyDef.angularDamping = 1.;

	//place at one of the top corners
	float rand = randFloat();

	//if (rand > .5) {
	bodyDef.position.Set(cloud_bounds.getUpperLeft().x + label_size.getWidth()*.5f, cloud_bounds.getUpperLeft().y + label_size.getHeight()*.5f);
	/*}
	else{
	bodyDef.position.Set(cloud_bounds.getUpperRight().x - label_size.getWidth()*.5f, cloud_bounds.getUpperRight().y + label_size.getHeight()*.5f);
	}*/

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

ci::Rectf Layout::calcBounds(b2Body * body)
{
	Rectf bounds = Rectf(vec2(body->GetWorldCenter().x, body->GetWorldCenter().y) - .5f, vec2(body->GetWorldCenter().x, body->GetWorldCenter().y) + .5f);
	if (auto poly_shape = dynamic_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape())) {
		for (int i = 0; i < poly_shape->GetVertexCount(); i++) {
			vec2 point = vec2(poly_shape->GetVertex(i).x, poly_shape->GetVertex(i).y);
			point += vec2(body->GetWorldCenter().x, body->GetWorldCenter().y);
			bounds.include(point);
		}
	}
	else if (auto circle_shape = dynamic_cast<b2CircleShape*>(body->GetFixtureList()->GetShape())) {
		float rad = circle_shape->m_radius;
		vec2 body_center = vec2(body->GetWorldCenter().x, body->GetWorldCenter().y);
		vec2 t = body_center + vec2(0, 1)*rad;
		vec2 b = body_center + vec2(0, -1)*rad;
		vec2 l = body_center + vec2(-1, 0)*rad;
		vec2 r = body_center + vec2(1, 0)*rad;
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

void Layout::createBounds()
{
	//create bounds

	b2PolygonShape t_width;
	t_width.SetAsBox(getWindowWidth(), mBounds.getUpperLeft().y);

	b2PolygonShape b_width;
	b_width.SetAsBox(getWindowWidth(), glm::abs(mBounds.getLowerRight().y - getWindowHeight()));

	b2PolygonShape l_height;
	l_height.SetAsBox(mBounds.getUpperLeft().x, getWindowHeight());

	b2PolygonShape r_height;
	r_height.SetAsBox(glm::abs(mBounds.getLowerRight().x - getWindowWidth()), getWindowHeight());

	b2FixtureDef t_widthDef;
	t_widthDef.shape = &t_width;
	t_widthDef.density = 1.0f;
	t_widthDef.friction = 0.3f;
	t_widthDef.restitution = 0.5f; // bounce
	t_widthDef.filter.categoryBits = BOUNDRY;
	t_widthDef.filter.maskBits = BOUNDRY | PRODUCT;

	b2FixtureDef b_widthDef;
	b_widthDef.shape = &b_width;
	b_widthDef.density = 1.0f;
	b_widthDef.friction = 0.3f;
	b_widthDef.restitution = 0.5f; // bounce
	b_widthDef.filter.categoryBits = BOUNDRY;
	b_widthDef.filter.maskBits = BOUNDRY | PRODUCT;

	b2FixtureDef l_heightDef;
	l_heightDef.shape = &l_height;
	l_heightDef.density = 1.0f;
	l_heightDef.friction = 0.3f;
	l_heightDef.restitution = 0.5f; // bounce
	l_heightDef.filter.categoryBits = BOUNDRY;
	l_heightDef.filter.maskBits = BOUNDRY | PRODUCT;

	b2FixtureDef r_hieghtDef;
	r_hieghtDef.shape = &r_height;
	r_hieghtDef.density = 1.0f;
	r_hieghtDef.friction = 0.3f;
	r_hieghtDef.restitution = 0.5f; // bounce
	r_hieghtDef.filter.categoryBits = BOUNDRY;
	r_hieghtDef.filter.maskBits = BOUNDRY | PRODUCT;

	b2BodyDef floorDef;
	floorDef.type = b2_staticBody;
	floorDef.position = b2Vec2(0, getWindowHeight());

	b2Body * ground = mWorld->CreateBody(&floorDef);
	ground->CreateFixture(&b_widthDef);

	b2BodyDef ceiling_def;
	ceiling_def.type = b2_staticBody;
	ceiling_def.position = b2Vec2(0, 0);

	b2Body * ceiling = mWorld->CreateBody(&ceiling_def);
	ceiling->CreateFixture(&t_widthDef);

	b2BodyDef left_wall_def;
	left_wall_def.type = b2_staticBody;
	left_wall_def.position = b2Vec2(0, 0);

	b2Body * left_wall = mWorld->CreateBody(&left_wall_def);
	left_wall->CreateFixture(&l_heightDef);

	b2BodyDef right_wall_def;
	right_wall_def.type = b2_staticBody;
	right_wall_def.position = b2Vec2(getWindowWidth(), 0);

	b2Body * right_wall = mWorld->CreateBody(&right_wall_def);
	right_wall->CreateFixture(&r_hieghtDef);
}


ci::Rectf Layout::calcBounds(std::vector<b2Body*>::iterator begin, std::vector<b2Body*>::iterator end, const ci::vec2& center)
{
	Rectf bounds = Rectf(vec2(center) - .5f, vec2(center) + .5f);
	while (begin != end) {
		b2Body* body = *begin++;
		bounds.include(calcBounds(body));
	}
	return std::move(bounds);
}
