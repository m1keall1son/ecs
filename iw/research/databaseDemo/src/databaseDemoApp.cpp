#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "DjangoDatabase.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace midnight::http;

class submitted_databaseDemoApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::shared_ptr<DjangoDatabase> mAPI;
};

void submitted_databaseDemoApp::setup() {
	mAPI = DjangoDatabase::make("http://127.0.0.1:8000/");

	for (int i = 0; i < 100; i++) {
		mAPI->getCategory();
		mAPI->getSubCategories();
		mAPI->getClouds();
		mAPI->getProducts();
	}
}

void submitted_databaseDemoApp::mouseDown( MouseEvent event )
{
}

void submitted_databaseDemoApp::update() {
	std::printf("-------------------- \n Frame Number %d \n -------------------- \n", cinder::app::getElapsedFrames());
}

void submitted_databaseDemoApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP(submitted_databaseDemoApp, RendererGl, [=](cinder::app::App::Settings* settings) {settings->setConsoleWindowEnabled(); })