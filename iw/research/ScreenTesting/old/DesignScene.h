#pragma once

#include "SceneBase.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

class DesignScene : public SceneBase {

public:

	static SceneRef create() { return SceneRef(new DesignScene); }

	virtual const char * getName() { return "Design Assets"; };

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

	ci::gl::BatchRef mFullScreenQuad;
	std::vector<ci::gl::Texture2dRef> mTextures;
	ci::gl::TextureRef mCurrentTexture;
	ci::params::InterfaceGlRef mParams;
};