#include "TouchScene.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

float Touchable::sId = 0.f;

void TouchScene::touchesBegan(ci::app::TouchEvent event)
{
	for ( auto & touch : event.getTouches() ) {

		bool found = false;

		for ( auto &  touchable : mPositions ) {
			if (touchable.test(touch.getPos())) {
				mTouches.insert({ touch.getId(), &touchable });
				touchable.setActive(true);
				found = true;
				break;
			}
		}

		if (!found) {
			mPositions.push_back(Touchable(touch.getPos(), randFloat(25, 50)));
			mTouches.insert({ touch.getId(), &mPositions.back() });
		}
	}
}

void TouchScene::touchesMoved(ci::app::TouchEvent event)
{
	for (auto & touch : event.getTouches()) {

		auto found = mTouches.find(touch.getId());
		if (found != mTouches.end()) {

			if (found->second) {
				found->second->setCenter(touch.getPos());
			}
			else {
				CI_LOG_E("No touchable for id: " << touch.getId() );
			}

		}
		else
		{
			CI_LOG_E("Unkown touch!");
		}

	}
}

void TouchScene::touchesEnded(ci::app::TouchEvent event)
{
	for (auto & touch : event.getTouches()) {
		
		auto found = mTouches.find(touch.getId());
		if (found != mTouches.end()) {
			found->second->setActive(false);
		}
		mTouches.erase(found);

	}
}

void TouchScene::mouseDown(ci::app::MouseEvent event)
{
	bool found = false;

	for (auto & touchable : mPositions) {

		if (touchable.test(event.getPos())) {
			mTouches.insert({ std::numeric_limits<uint32_t>::max(), &touchable });
			touchable.setActive(true);
			found = true;
			break;
		}

	}

	if (!found) {
		mPositions.push_back(Touchable(event.getPos(), randFloat(25, 50)));
		mTouches.insert({ std::numeric_limits<uint32_t>::max(), &mPositions.back() });
	}
}

void TouchScene::mouseDrag(ci::app::MouseEvent event)
{
	auto found = mTouches.find(std::numeric_limits<uint32_t>::max());
	if (found != mTouches.end()) {

		if (found->second) {
			found->second->setCenter(event.getPos());
		}
		else {
			CI_LOG_E("No touchable for mouse" );
		}

	}
	else
	{
		CI_LOG_E("Unkown touch!");
	}

}

void TouchScene::mouseUp(ci::app::MouseEvent event)
{
	auto found = mTouches.find(std::numeric_limits<uint32_t>::max());
	if (found != mTouches.end()) {
		found->second->setActive(false);
	}
	mTouches.erase(found);
}

void TouchScene::setup()
{

	mRoughness = .5f;
	mMetallic = 0.00f;
	mSpecular = 1.0f;
	mLightRadius = 400.0f;
	mLightColor = Color::hex(0x43eaf9);
	mBaseColor = Color::hex(0x43eaf9);
	mGamma = 2.2f;
	mExposure = 10.0f;
	mLightPosition = vec3( getWindowCenter().x, -10, 500 );

	mCamera.setOrtho( 0, getWindowWidth(), getWindowHeight(), 0, -1000, 1000 );

	auto mesh = gl::VboMesh::create( geom::Icosahedron() >> geom::Rotate( glm::toQuat( ci::rotate( 1.f, vec3(1,1,1) ) ) )  );

	mData = gl::Vbo::create(GL_ARRAY_BUFFER, sizeof(vec3) * 1000, nullptr, GL_DYNAMIC_DRAW);

	geom::BufferLayout layout;
	layout.append(geom::Attrib::CUSTOM_0, 3, 0, 0, 1);
	mesh->appendVbo(layout, mData);

	gl::GlslProgRef glsl;

	/* std::string vert = CI_GLSL(150,
		in vec4 ciPosition;
		in vec3 ciNormal;
		in vec2 ciTexCoord0;
		in vec2 pos;

		uniform mat4 ciModelViewProjection;

		out vec2 vTexCoord;
		out vec3 vNormal;

		void main() {

			vTexCoord = ciTexCoord0;
			vNormal = ciNormal;
			gl_Position = ciModelViewProjection * vec4(ciPosition.xy + pos, ciPosition.z, 1.);

		}

	);

	 std::string frag = CI_GLSL(150,

		out vec4 FragColor;
		in vec2 vTexCoord;
		in vec3 vNormal;

		void main() {
			FragColor = vec4( vTexCoord, 1.,  1. );
		}

	);*/


	try {
		glsl = gl::GlslProg::create( gl::GlslProg::Format().vertex(loadAsset("PBR.vert")).fragment(loadAsset("PBR.frag")).attrib(geom::CUSTOM_0, "pos") );
	}
	catch ( const ci::gl::GlslProgCompileExc& e ) {
		CI_LOG_E(e.what());
	}

	glsl->uniform("uLightPosition", mLightPosition);
	glsl->uniform("uLightColor", mLightColor);
	glsl->uniform("uLightRadius", mLightRadius);
	glsl->uniform("uBaseColor", mBaseColor);
	glsl->uniform("uSpecular", mSpecular);

	// sends the tone-mapping uniforms
	glsl->uniform("uExposure", mExposure);
	glsl->uniform("uGamma", mGamma);
	glsl->uniform("uRoughness", mRoughness);
	glsl->uniform("uMetallic", mMetallic);
	auto eye = mCamera.getEyePoint();
	glsl->uniform("uEyePos", vec3(getWindowCenter(), 100.));

	mCube = gl::Batch::create( mesh, glsl );


}

void TouchScene::update()
{

	if (mPositions.empty())return;
	
	
	for ( auto & position : mPositions ) {
		position.update();
	}
	auto mapped = (vec3*)mData->mapBufferRange(0, sizeof(ci::vec3)*mPositions.size(), GL_MAP_WRITE_BIT );
	for ( auto & pos : mPositions ) {
		*mapped++ = vec3(pos.getCenter(), pos.getSize());
	}
	mData->unmap();
	CI_CHECK_GL();
}

void TouchScene::pause()
{
	mPositions.clear();
}

void TouchScene::play()
{
}

void TouchScene::draw()
{
	gl::clear(Color::hex(0x32b5c1));

	gl::ScopedMatrices push;
	gl::setMatrices(mCamera);
	gl::ScopedViewport view(vec2(0), getWindowSize());

	gl::ScopedDepth depth(true);

	mCube->drawInstanced(mPositions.size());

}

void Touchable::update()
{
	if(!mIsActive)
		mArea.offsetCenterTo( mArea.getCenter() + vec2(cos(getElapsedSeconds()*.75 + mId), sin(getElapsedSeconds()*.75 + mId)) );
}
