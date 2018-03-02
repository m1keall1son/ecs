#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ciDebugDraw.h"

#include "LayoutGenerator.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LayoutGeneratorApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;

	gl::b2DubugDraw::CircleFillBatch mBatch;
	LayoutGenerator::SubcategoryLayout mLayout;
	bool mShowClouds{false};
	bool mShowPriority{ false };
	bool mShowEpicenter{ true };
	vec2 mEpicenter;
};

void LayoutGeneratorApp::setup()
{
	mEpicenter = vec2(getWindowCenter().x, getWindowCenter().y - 200);

	LayoutGenerator generator( getWindowCenter(), mEpicenter, getWindowHeight()*.25f, getWindowHeight() * .75f );

	mLayout = generator.generateLayout( { 
		{ LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::HIGHLIGHT, LayoutGenerator::FEATURED, LayoutGenerator::HIGHLIGHT },
		{ LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD },
		{ LayoutGenerator::FEATURED, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::FEATURED, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD },
		{ LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::HIGHLIGHT, LayoutGenerator::FEATURED, LayoutGenerator::HIGHLIGHT, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD },
		{ LayoutGenerator::STANDARD, LayoutGenerator::HIGHLIGHT, LayoutGenerator::STANDARD },
		{ LayoutGenerator::HIGHLIGHT, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::HIGHLIGHT, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD, LayoutGenerator::STANDARD }
	} );

}

void LayoutGeneratorApp::mouseDown( MouseEvent event )
{
}

void LayoutGeneratorApp::keyDown(KeyEvent event)
{
	switch (event.getCode()) {
	case KeyEvent::KEY_ESCAPE: quit(); break;
	case KeyEvent::KEY_c: mShowClouds = !mShowClouds; break;
	case KeyEvent::KEY_p: mShowPriority = !mShowPriority; break;
	default: break;
	}
}

void LayoutGeneratorApp::update()
{
	float c = 0;
	for (auto & cloud : mLayout) {
		mBatch.add(cloud.first.first, cloud.first.second, ColorA(1., 1., c / (float)mLayout.size(), mShowClouds ? .5f : 0.f ));
		++c;
		float c2 = 0;
		for (auto & product : cloud.second) {
			mBatch.add(product.first, product.second, mShowPriority ? ColorA(1., c2 / (float)cloud.second.size(), c2 / (float)cloud.second.size(), 1.) : ColorA::white() );
			++c2;
		}
	}
}

void LayoutGeneratorApp::draw()
{
	gl::clear(Color(0,0,0));
	mBatch.draw();
	if (mShowEpicenter)
	{
		gl::ScopedColor color(1., 0., 0.);
		gl::drawSolidCircle(mEpicenter, 5);
	}
}

CINDER_APP(LayoutGeneratorApp, RendererGl(RendererGl::Options().msaa(4)), [](App::Settings*settings) { settings->setWindowSize( 1920, 1080 ); })
