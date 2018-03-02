#pragma once

#include "SceneBase.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

class Touchable {
public:
	static float sId;

	Touchable(const ci::vec2& pos, float size) : mArea(ci::vec2(pos-size), ci::vec2(pos+size)), mId(sId++), mSize(size) { }

	bool test(const ci::vec2& pos) {
		return mArea.contains(pos);
	}

	ci::vec2 getCenter() { return mArea.getCenter(); }
	void setCenter(const ci::vec2& pos) { mArea = ci::Rectf(ci::vec2(pos -mSize), ci::vec2(pos + mSize)); }
	ci::Rectf getRect() { return mArea; }
	float getSize() { return mSize; }
	void setActive(bool active) { mIsActive = active; }
	bool isActive() { return mIsActive; }

	void update();

private:

	float mSize;
	float mId;
	bool mIsActive{true};
	ci::Rectf mArea;

};

class TouchScene : public SceneBase {

public:

	static SceneRef create() { return SceneRef( new TouchScene ); }

	virtual const char * getName() { return "Touch Test"; };

	void touchesBegan( ci::app::TouchEvent event )override;
	void touchesMoved( ci::app::TouchEvent event )override;
	void touchesEnded( ci::app::TouchEvent event )override;

	void mouseDown(ci::app::MouseEvent event)override;
	void mouseDrag(ci::app::MouseEvent event) override;
	void mouseUp(ci::app::MouseEvent event) override;

	void setup()override;
	void update()override;
	void pause()override;
	void play()override;
	void draw()override;

private:

	ci::gl::VboRef mData;
	ci::gl::BatchRef mCube;
	std::vector<Touchable> mPositions;
	ci::CameraOrtho mCamera;

	std::map<uint32_t, Touchable*> mTouches;

	float			mRoughness, mMetallic, mSpecular;
	ci::Color		mBaseColor, mLightColor;
	float			mLightRadius, mGamma, mExposure;
	ci::vec3		mLightPosition;

};