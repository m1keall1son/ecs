#pragma once
#include "cinder/app/App.h"
#include "cinder/Log.h"

using SceneRef = std::shared_ptr<class SceneBase>;

class SceneBase {

public:

	virtual void touchesBegan(ci::app::TouchEvent event) = 0;
	virtual void touchesMoved( ci::app::TouchEvent event) = 0;
	virtual void touchesEnded( ci::app::TouchEvent event) = 0;

	virtual void mouseDown(ci::app::MouseEvent event) = 0;
	virtual void mouseDrag(ci::app::MouseEvent event) = 0;
	virtual void mouseUp(ci::app::MouseEvent event) = 0;

	virtual void setup() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void pause() = 0;
	virtual void play() = 0;
	virtual const char * getName() = 0;

};