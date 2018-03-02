#include "LayoutScene.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

void LayoutScene::touchesBegan(ci::app::TouchEvent event)
{
}

void LayoutScene::touchesMoved(ci::app::TouchEvent event)
{
}

void LayoutScene::touchesEnded(ci::app::TouchEvent event)
{
}

void LayoutScene::mouseDown(ci::app::MouseEvent event)
{
}

void LayoutScene::mouseDrag(ci::app::MouseEvent event)
{
}

void LayoutScene::mouseUp(ci::app::MouseEvent event)
{
}

void LayoutScene::setup()
{
	mParams = params::InterfaceGl::create( getName(), vec2( 200, 200 ) );
	mParams->setPosition( vec2(50,300) );

	mNumClouds = 7;
	mMinProducts = 5;
	mMaxProducts = 16;

	mBounds = Rectf(vec2(getWindowSize())*.1f, vec2(getWindowSize())*.9f);

	auto generate_layout = [&] {

		LayoutGenerator generator(getWindowCenter(), mEpicenter, mBounds, 3.f);
	
		LayoutGenerator::SubcategoryStructure subcat_structure;

		for (int j = 0; j < mNumClouds; j++) {

			LayoutGenerator::CloudStructure cloud_structure;
				
			auto products = randInt( mMinProducts, mMaxProducts);
			for (int p = 0; p < products; p++) {

				LayoutGenerator::Size product;
				auto r = randFloat();
				if (r < .1) {
					product = LayoutGenerator::FEATURED;
				}
				else if ( r > .1 && r < .3)
				{
					product = LayoutGenerator::HIGHLIGHT;
				}
				else
				{
					product = LayoutGenerator::STANDARD;
				}

				cloud_structure.push_back( product );

			}

			subcat_structure.push_back( cloud_structure );

		}

		mLayout = generator.generateLayout( subcat_structure );

	};

	mParams->addParam("clouds", &mNumClouds);
	mParams->addParam("min products", &mMinProducts);
	mParams->addParam("max products", &mMaxProducts);
	mParams->addButton("Generate Layout", generate_layout);
	mParams->addSeparator();
	mParams->addParam("show priority", &mShowPriority );
	mParams->addParam("show epicenter", &mShowEpicenter );
	mParams->addParam("show clouds", &mShowClouds );
	mParams->addParam("show bounds", &mShowBounds);

	mEpicenter = vec2(getWindowCenter().x, getWindowCenter().y - 200);

	mParams->addParam("epicenter - x", &mEpicenter.x);
	mParams->addParam("epicenter - y", &mEpicenter.y);
	mParams->addParam("bounds-min - x", &mBounds.x1 );
	mParams->addParam("bounds-min - y", &mBounds.y1);
	mParams->addParam("bounds-max - x", &mBounds.x2);
	mParams->addParam("bounds-max - y", &mBounds.y2);

	generate_layout();

	mParams->hide();

}

void LayoutScene::update()
{
	/*float c = 0;
	for (auto & cloud : mLayout) {
		mBatch.add(cloud.first.first, cloud.first.second, ColorA(1., 1., c / (float)mLayout.size(), mShowClouds ? .5f : 0.f));
		++c;
		float c2 = 0;
		for (auto & product : cloud.second) {
			mBatch.add(product.first, product.second, mShowPriority ? ColorA(1., c2 / (float)cloud.second.size(), c2 / (float)cloud.second.size(), 1.) : ColorA::white());
			++c2;
		}
	}*/
}

void LayoutScene::pause()
{
	mParams->hide();
}

void LayoutScene::play()
{
	mParams->show();
}

void LayoutScene::draw()
{
	gl::clear(Color(0, 0, 0));
	//mBatch.draw();

	if (mShowClouds) {
		float c = 0;
		for ( auto & placement : mLayout ) {
			gl::ScopedColor color(ColorA(1., 1., c++ / (float)mLayout.size(), .5f ));
			gl::drawSolidRect( Rectf(placement.first.first - placement.first.second * .5f, placement.first.first + placement.first.second * .5f ));
		}
	}

	for (auto & cloud : mLayout) {
		float c2 = 0;
		for (auto & product : cloud.second) {
			gl::ScopedColor color( mShowPriority ? ColorA(1., c2 / (float)cloud.second.size(), c2 / (float)cloud.second.size(), 1.) : Color::white() );
			gl::drawSolidRect(Rectf(product.first - product.second * .5f, product.first + product.second * .5f));
			c2++;
		}
	}


	if (mShowEpicenter)
	{
		gl::ScopedColor color(1., 0., 0.);
		gl::drawSolidCircle(mEpicenter, 5);
	}
	if (mShowBounds) {
		gl::drawStrokedRect(mBounds);
	}
}
