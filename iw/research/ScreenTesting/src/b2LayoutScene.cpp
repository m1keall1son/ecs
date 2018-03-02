#include "b2LayoutScene.h"

#include "cinder/Rand.h"
#include "b2Layout.h"

using namespace ci;
using namespace ci::app;

void b2LayoutScene::touchesBegan(ci::app::TouchEvent event)
{
}

void b2LayoutScene::touchesMoved(ci::app::TouchEvent event)
{
}

void b2LayoutScene::touchesEnded(ci::app::TouchEvent event)
{
}

void b2LayoutScene::mouseDown(ci::app::MouseEvent event)
{
}

void b2LayoutScene::mouseDrag(ci::app::MouseEvent event)
{
}

void b2LayoutScene::mouseUp(ci::app::MouseEvent event)
{
}

void b2LayoutScene::setup()
{
	mParams = params::InterfaceGl::create("b2Layout Control", vec2(200, 200));
	mParams->setPosition(vec2(50, 400));

	mBounds = Rectf( vec2( getWindowSize() )*.1f, vec2(getWindowSize())*.9f );
	mCenter = getWindowCenter();

	auto generator = b2Layout( mCenter, mBounds );

	populateInitialData();

	mSubcategoryLayout = generator.generate( mInitialData );

}

void b2LayoutScene::update()
{
}

void b2LayoutScene::pause()
{
	mParams->hide();
}

void b2LayoutScene::play()
{
	mParams->show();
}

void b2LayoutScene::draw()
{
	gl::clear();

	//mWorld->DrawDebugData();
	//mDraw.draw();

	if(mShowBounds)gl::drawStrokedRect(mBounds);

	float col = 0.f;
	for (auto & cloud : mSubcategoryLayout.clouds) {

		gl::ScopedColor color(ColorA(1., 1., 0., .25));
		if(mShowBounds)gl::drawStrokedRect(cloud.placement);
		gl::ScopedColor color_label(ColorA(1. - col / (float)mSubcategoryLayout.clouds.size(), col / (float)mSubcategoryLayout.clouds.size(), col / (float)mSubcategoryLayout.clouds.size(), 1.));
		gl::drawSolidRect(cloud.label);

		for (auto & product : cloud.products) {
			gl::ScopedColor color_bounds(ColorA(1., 1., 1., .25));
			if(mShowBounds)gl::drawStrokedRect(product.placement);
			gl::ScopedColor color_circle(ColorA( 1. - col / (float)mSubcategoryLayout.clouds.size(), col / (float)mSubcategoryLayout.clouds.size(), col / (float)mSubcategoryLayout.clouds.size(), 1.));
			gl::drawSolidCircle(product.placement.getCenter() - vec2(0, product.placement.getSize().y*.5f - product.placement.getSize().x*.5f), product.placement.getSize().x*.5f);
			gl::ScopedColor color_text(ColorA(col / (float)mSubcategoryLayout.clouds.size(), col / (float)mSubcategoryLayout.clouds.size(), .7, 1.));
			auto text_area_height = product.placement.getSize().y - product.placement.getSize().x;
			auto text_area_pos = vec2(product.placement.getCenter().x, (product.placement.getCenter().y - product.placement.getSize().y*.5f) + product.placement.getSize().x + text_area_height*.5f);
			gl::drawSolidRect(Rectf(text_area_pos - vec2(product.placement.getSize().x, text_area_height)*.5f, text_area_pos + vec2(product.placement.getSize().x, text_area_height)*.5f));
		}
		col++;
	}
	
	mParams->draw();
}

void b2LayoutScene::populateInitialData()
{
	//from icecream subcat
	{
		auto label = Rectf(vec2(0), vec2(randFloat(200, 300), randFloat(60,100)));
		std::vector<ci::Rectf> products;
		auto num_products = 16;
		for (int i = 0; i < num_products; i++) {

			auto rand = randFloat();
			Rectf rect;
			if (rand < .7) {
				rect = Rectf(vec2(0), mStandard);
			}
			else if (rand > .7 && rand < .9) {
				rect = Rectf(vec2(0), mFeatured);
			}
			else {
				rect = Rectf(vec2(0), mHighlight);
			}
			products.emplace_back(std::move(rect));
		}
		mInitialData.emplace_back( label, std::move(products) );
	}

	{
		auto label = Rectf(vec2(0), vec2(randFloat(200, 300), randFloat(60, 100)));
		std::vector<ci::Rectf> products;
		auto num_products = 6;
		for (int i = 0; i < num_products; i++) {

			auto rand = randFloat();
			Rectf rect;
			if (rand < .7) {
				rect = Rectf(vec2(0), mStandard);
			}
			else if (rand > .7 && rand < .9) {
				rect = Rectf(vec2(0), mFeatured);
			}
			else {
				rect = Rectf(vec2(0), mHighlight);
			}
			products.emplace_back(std::move(rect));
		}
		mInitialData.emplace_back(label, std::move(products));
	}

	{
		auto label = Rectf(vec2(0), vec2(randFloat(200, 300), randFloat(60, 100)));
		std::vector<ci::Rectf> products;
		auto num_products = 26;
		for (int i = 0; i < num_products; i++) {

			auto rand = randFloat();
			Rectf rect;
			if (rand < .7) {
				rect = Rectf(vec2(0), mStandard);
			}
			else if (rand > .7 && rand < .9) {
				rect = Rectf(vec2(0), mFeatured);
			}
			else {
				rect = Rectf(vec2(0), mHighlight);
			}
			products.emplace_back(std::move(rect));
		}
		mInitialData.emplace_back(label, std::move(products));
	}

	{
		auto label = Rectf(vec2(0), vec2(randFloat(200, 300), randFloat(60, 100)));
		std::vector<ci::Rectf> products;
		auto num_products = 19;
		for (int i = 0; i < num_products; i++) {

			auto rand = randFloat();
			Rectf rect;
			if (rand < .7) {
				rect = Rectf(vec2(0), mStandard);
			}
			else if (rand > .7 && rand < .9) {
				rect = Rectf(vec2(0), mFeatured);
			}
			else {
				rect = Rectf(vec2(0), mHighlight);
			}
			products.emplace_back(std::move(rect));
		}
		mInitialData.emplace_back(label, std::move(products));
	}
}
