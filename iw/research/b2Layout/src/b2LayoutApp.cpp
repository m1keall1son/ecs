#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "b2LayoutScene.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class b2LayoutApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;

	SceneRef mScene;
};

void b2LayoutApp::setup()
{
	mScene = b2LayoutScene::create();
	mScene->setup();
	mScene->play();
}

void b2LayoutApp::mouseDown( MouseEvent event )
{
}

void b2LayoutApp::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
	case KeyEvent::KEY_ESCAPE:
	{
		quit();
	}break;
	default:
		break;
	}
}

void b2LayoutApp::update()
{
	mScene->update();
}

void b2LayoutApp::draw()
{
	mScene->draw();
}

CINDER_APP(b2LayoutApp, RendererGl(RendererGl::Options().msaa(8)), [](App::Settings*settings) { settings->setFullScreen(); })
