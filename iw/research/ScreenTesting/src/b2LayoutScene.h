#pragma once

#include "SceneBase.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Box2D/Box2D.h"
#include "ciDebugDraw.h"
#include "cinder/params/Params.h"
#include "CollisionTypes.h"
#include "LayoutTypes.h"

class b2LayoutScene : public SceneBase {

public:

	static SceneRef create() { return SceneRef(new b2LayoutScene); }

	virtual const char * getName() { return "b2LayoutScene"; };

	void touchesBegan(ci::app::TouchEvent event)override;
	void touchesMoved(ci::app::TouchEvent event)override;
	void touchesEnded(ci::app::TouchEvent event)override;

	void mouseDown(ci::app::MouseEvent event)override;
	void mouseDrag(ci::app::MouseEvent event) override;
	void mouseUp(ci::app::MouseEvent event) override;

	void setup()override;
	void update()override;
	void pause()override;
	void play()override;
	void draw()override;

private:

	void populateInitialData();

	ci::params::InterfaceGlRef mParams;
	ci::Rectf mFitArea;

	int mNumClouds{ 4 };
	int mMinProducts{ 1 };
	int mMaxProducts{ 12 };
	float mMinProductSeparation{ 10.f };
	float mMinCloudSeparation{ 100.f };

	bool mShowBounds{ false };
	bool mDrawDebug{ false };

	ci::vec2 mCenter;
	ci::Rectf mBounds;

	ci::vec2 mStandard = ci::vec2(60, 80);
	ci::vec2 mFeatured = ci::vec2(75, 100);
	ci::vec2 mHighlight = ci::vec2(90, 120);

	std::vector< std::pair< ci::Rectf/*label size*/, std::vector<ci::Rectf/*product size*/>>> mInitialData;

	Layout::Subcategory mSubcategoryLayout;

};