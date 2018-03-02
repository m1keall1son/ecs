#pragma once

#include "SceneBase.h"
#include "cinder/params/Params.h"

#include "ciDebugDraw.h"

#include "LayoutGenerator.h"


class LayoutScene : public SceneBase {

public:

	static SceneRef create() { return SceneRef(new LayoutScene); }

	virtual const char * getName() { return "Layout Generator"; };

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

	ci::gl::b2DubugDraw::CircleFillBatch mBatch;
	LayoutGenerator::SubcategoryLayout mLayout;
	bool mShowClouds{ false };
	bool mShowPriority{ false };
	bool mShowEpicenter{ true };
	bool mShowBounds{ false };
	ci::vec2 mEpicenter;
	ci::Rectf mBounds;
	ci::params::InterfaceGlRef mParams;

	int mNumClouds, mNumSubCategories, mMinProducts, mMaxProducts;

};