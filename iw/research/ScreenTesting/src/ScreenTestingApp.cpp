#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "b2LayoutScene.h"

class ScreenTestingApp : public App {
  public:
	void setup() override;

	void touchesBegan(TouchEvent event)override;
	void touchesMoved(TouchEvent event)override;
	void touchesEnded(TouchEvent event)override;

	void mouseDown( MouseEvent event ) override;
	void mouseDrag(MouseEvent event) override;
	void mouseUp(MouseEvent event) override;

	void keyDown(KeyEvent event)override;
	void update() override;
	void draw() override;

	SceneRef mCurrentScene;
	params::InterfaceGlRef mParams;
	

};

void ScreenTestingApp::setup()
{

	//auto touch_scene = TouchScene::create();
	//touch_scene->setup();
	//auto layout_scene = LayoutScene::create();
	//layout_scene->setup();
	////auto physics = PhysicsScene::create();
	////physics->setup();
	//auto design = DesignScene::create();
	//design->setup();

	auto b2scene = b2LayoutScene::create();
	b2scene->setup();
	mCurrentScene = b2scene;

	mParams = params::InterfaceGl::create( "CONTROL", vec2(200, 200) );
	
	mParams->addText("Tests");
	//mParams->addButton("Touch Test", [&, touch_scene] { if (mCurrentScene)mCurrentScene->pause(); mCurrentScene = touch_scene; mCurrentScene->play(); });
	//mParams->addButton("Layout Generator", [&, layout_scene] { if (mCurrentScene)mCurrentScene->pause(); mCurrentScene = layout_scene; mCurrentScene->play(); });
	////mParams->addButton("Physics Layout", [&, physics] { if (mCurrentScene)mCurrentScene->pause(); mCurrentScene = physics; mCurrentScene->play(); });
	//mParams->addButton("Design Assets", [&, design] { if (mCurrentScene)mCurrentScene->pause(); mCurrentScene = design; mCurrentScene->play(); });

	//mCurrentScene = touch_scene;

	mCurrentScene->play();

	gl::enableAlphaBlending();

}

void ScreenTestingApp::touchesBegan(TouchEvent event)
{
	if(mCurrentScene)mCurrentScene->touchesBegan(event);
}

void ScreenTestingApp::touchesMoved(TouchEvent event)
{
	if (mCurrentScene)mCurrentScene->touchesMoved(event);
}

void ScreenTestingApp::touchesEnded(TouchEvent event)
{
	if (mCurrentScene)mCurrentScene->touchesEnded(event);
}

void ScreenTestingApp::mouseDown( MouseEvent event )
{
	if (mCurrentScene)mCurrentScene->mouseDown(event);
}

void ScreenTestingApp::mouseDrag(MouseEvent event)
{
	if (mCurrentScene)mCurrentScene->mouseDrag(event);
}

void ScreenTestingApp::mouseUp(MouseEvent event)
{
	if (mCurrentScene)mCurrentScene->mouseUp(event);
}

void ScreenTestingApp::keyDown(KeyEvent event)
{
	switch (event.getCode()) {
	case KeyEvent::KEY_ESCAPE:
		quit();
		break;
	case KeyEvent::KEY_p:
		if (mParams->isVisible())mParams->hide();
		else mParams->show();
		break;
	case KeyEvent::KEY_SPACE:
		
	default: break;
	}

}

void ScreenTestingApp::update()
{
	if (mCurrentScene)mCurrentScene->update();

}

void ScreenTestingApp::draw()
{
	gl::clear();
	if (mCurrentScene)mCurrentScene->draw();

	if (mParams)mParams->draw();

}
