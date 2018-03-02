#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "PersonDetector.h"
#include "ECS.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "IWScene/IWInitializer.h"
#include "StartUpScene/StartUpInit.h"
#include "Taxonomy/TaxonomySystem.h"
#include "IWDemo/DemoInit.h"
#include "ciPQLabsTouch.h"
#include "IWScene/IWEvents.h"
#include "IWDemo/DemoEvents.h"

#include "IWDemo/Serial.h"

#define BAUD 115200

class InteractiveWallApp : public Engine {
public:

	static InteractiveWallApp * get() { return static_cast<InteractiveWallApp*>(Engine::get()); }

	//include HTTP and database stuff here w/ appropriate setters/getters so scene states can access them?

	PQLabsTouch mTouchClient;
	PersonDetectorRef mPersonDetector;
	params::InterfaceGlRef mParams;

	bool tryConnectSerial();
	void serialThread( HANDLE port );

	std::thread mSerialThread;
	std::atomic_bool mSerialConnection{false};
	std::atomic_bool mAppRunning{true};
	std::atomic_bool mTap{ false };
	std::atomic<uint8_t> mCurrentState;

	int mTimeoutCounter{ 0 };

	void keyDown(KeyEvent event)override;
	void postSetup()override;
	void preUpdate()override;
	void postDraw()override;
	void cleanup()override;
	void init()override;
};

bool InteractiveWallApp::tryConnectSerial()
{
	std::future<HANDLE> connection = std::async(std::launch::async, [&] () mutable -> HANDLE {

		HANDLE serialPort;
		bool connected = false;
		// connect to the first available serial port
		for (int portNum = 0; portNum < 10; portNum++) {
			serialPort = Serial::openPort(portNum, BAUD, &connected);

			if (connected) {
				CI_LOG_V( "Opened serial port " << portNum );
				mSerialConnection.store(connected);
				Serial::flush(serialPort);
				return serialPort;
			}
		}
		
		if (!connected) {
			throw std::runtime_error("No serial device found");
		}

	});

	try {
		HANDLE serialPort;
		try {
			serialPort = connection.get();
		}
		catch (const std::runtime_error& e) {
			CI_LOG_E(e.what());
			return false;
		}
		mSerialThread = std::move(std::thread(&InteractiveWallApp::serialThread, this, serialPort));
		return true;
	}
	catch (const std::exception& e)
	{
		CI_LOG_EXCEPTION("serial port failed to connect",e);
		return false;
	}

}

void InteractiveWallApp::serialThread(HANDLE serialPort)
{
	Serial::flush(serialPort);

	CI_LOG_V("STARTING SERIAL THREAD");
	while ( mAppRunning.load() && mSerialConnection.load() ) {
		try {
			char byte[1] = { 'A' };
			if (Serial::writeBytes(serialPort, byte, 1) != 1) {
				CI_LOG_E("Byte 'A' not written!");
			}
		}
		catch (const std::exception& e)
		{
			CI_LOG_E(e.what());
			mSerialConnection.store(false);
		}

		try {
			char byte;
			Serial::readByte(serialPort, &byte);
			if (mCurrentState.load() != byte)
			{
				if (byte == '1')
				{
					mTap.store(true);
				}
				mCurrentState.store(byte);
			}
		}
		catch (const std::exception& e)
		{
			CI_LOG_E(e.what());
			mSerialConnection.store(false);
		}

		Sleep(10);

		Serial::flush(serialPort);

	}
	Serial::closePort(serialPort);
}

void InteractiveWallApp::cleanup() {
	mAppRunning.store(false);
	if(mSerialThread.joinable())
		mSerialThread.join();
}

void InteractiveWallApp::keyDown(KeyEvent event)
{
	switch (event.getCode()){
	case KeyEvent::KEY_p:
	{
		if (mParams->isVisible())
			mParams->hide();
		else
			mParams->show();
	}break;
	case KeyEvent::KEY_SPACE:
	{
		auto eventManager = get()->scene()->getEventManager();
		eventManager->queueEvent(CollectionEvent::create());
	}break;
	case KeyEvent::KEY_l:
	{
		loadScene("StartUpScene");
	}break;
	case KeyEvent::KEY_a:
	{
		loadScene("IWDemo");
	}break;
	default: break;
	}
}

void InteractiveWallApp::postSetup()
{
	tryConnectSerial();

	mTouchClient.connect( PQLabsTouch::GESTURES_ALL );

	//TODO this is sort of hackey
	auto handler = [&](const pql::TouchGesture& gesture) {
		CI_LOG_I("CLICK...");
		onClick(vec2(gesture.params[0], gesture.params[1]));
	};

	mTouchClient.connectClickListener(handler);
	mTouchClient.connectTwoFingerClickListener(handler);
	mTouchClient.connectDoubleClickListener(handler);

	mParams = params::InterfaceGl::create( "CONTROL", vec2(200,200) );
	mParams->hide();

	mPersonDetector = PersonDetector::create();
	mPersonDetector->addOnDetectionFn([](bool detected) {
		CI_LOG_V( (detected ? "ZED: Person detected" : "ZED: Person no longer detected") );
		auto scene = get()->scene();
		if (!scene)return;
		 auto eventManager = scene->getEventManager();
		eventManager->queueEvent(PresenceDetectionEvent::create(detected));
	});
	mPersonDetector->populateParams(mParams);

	CI_CHECK_GL();
}

void InteractiveWallApp::preUpdate()
{
	mPersonDetector->update();

	if (!mSerialConnection.load()) {
		if (getElapsedFrames() % 120 == 0)
		{
			tryConnectSerial();
		}
	}

	if (mTap)
	{
		scene()->getEventManager()->queueEvent(CollectionEvent::create());
		mTap.store(false);
	}

}

void InteractiveWallApp::postDraw()
{
	mPersonDetector->draw();
	mParams->draw();
}

void InteractiveWallApp::init()
{
	//Register scene states and new systems
	SceneInitializerFactory& factory = getSceneInitializerFactory();
	factory.emplace("StartUpInit", StartUpInit::create);
	factory.emplace("IWInitializer", IWInitializer::create);
	factory.emplace("DemoInit", DemoInit::create);

	SystemFactory& sysFactory = getSystemFactory();
	sysFactory.emplace("TaxonomySystem", TaxonomySystem::create);

	//load config
	Json::Value config = util::parseJson("config/EngineConfig.json");
	loadConfig(config);
}

CINDER_APP(InteractiveWallApp, RendererGl(RendererGl::Options().version(4, 3).msaa(8)), [](App::Settings* settings) { settings->setFullScreen(); settings->setConsoleWindowEnabled(); })
