#include "VideoMaterial.h"
#include "cinder/app/App.h"
#include "ecs/rendering/RenderSystem.h"
#include "ecs/engine/Engine.h"

void VideoMaterialBind::play()
{
	mVideoPlayer.play();
}

void VideoMaterialBind::pause()
{
	mVideoPlayer.stop();
}

void VideoMaterialBind::init()
{
	if (!ci::fs::exists(ci::app::getAssetPath(mVideoPath)))
		throw ci::Exception("path to movie does not exist: " + mVideoPath);

	mVideoPlayer.loadMovie( ci::app::getAssetPath(mVideoPath) );
	mVideoPlayer.setLoop(true);
	
	auto glsl = ci::gl::GlslProg::Format().vertex( ci::app::loadAsset("FarAwayVideoMaterial/video.vert") ).fragment(ci::app::loadAsset("FarAwayVideoMaterial/video.frag"));
	loadShader(glsl);
	mGlsl->uniform("uVideo", 0);
	mGlsl->uniform("uVideoSize", ci::vec2( mVideoPlayer.getWidth(), mVideoPlayer.getHeight() ));

}

void VideoMaterialBind::bind()
{
	mGlsl->uniform("uAlpha", mAlpha);
	mVideoPlayer.update();
	ci::gl::enableDepthRead();
	ci::gl::enableAlphaBlending();
	mVideoPlayer.bindTexture(0);
}

void VideoMaterialBind::unbind()
{
	mVideoPlayer.unbindTexture();
	ci::gl::disableAlphaBlending();
	ci::gl::disableDepthRead();
}

void VideoMaterialBind::loadVideo(const std::string & path)
{
	mVideoPath = path;
	mVideoPlayer.stop();
	mVideoPlayer.loadMovie(ci::app::getAssetPath(mVideoPath));
	mVideoPlayer.setLoop(true);
}
