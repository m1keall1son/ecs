#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "jsoncpp/json.h"
#include "cinder/gl/SdfText.h"
#include "cinder/Utilities.h"
#include "cinder/Rand.h"
#include "cinder/CameraUi.h"
#include <fstream>
#include "WireRect.h"
#include "cinder/gl/Ssbo.h"

const std::vector<std::string> sText = {
	"The quick brown fox jumps over a lazy dog"
	//"The quick brown fox jumps over a lazy dog",
	//"The quick brown fox jumps over a lazy dog",
	//"The quick brown fox jumps over a lazy dog",
	//"The quick brown fox jumps over a lazy dog",
	//"The quick brown fox jumps over a lazy dog",
	//"The quick brown fox jumps over a lazy dog"
};

using namespace ci;
using namespace ci::app;
using namespace std;

struct TextRenderItem
{
	vec4 color;
	vec4 bounds;
	ivec4 font_data;
};

struct Lookup
{
	uint32_t matrix;
	uint32_t glyph;
};

class TextRendererApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::vector<gl::SdfText::Font> mFonts;
	std::vector<gl::SdfTextRef> mSDText;

	gl::Texture3dRef		mSDTextures;

	gl::VboRef				mGlyphLookups;
	gl::BatchRef			mGlyphMesh;
	gl::GlslProgRef			mSDFShader;

	gl::BatchRef			mDebris;
	gl::GlslProgRef			mDebrisShader;

	gl::BatchRef			mBoundsBatch;
	gl::GlslProgRef			mBoundsShader;

	gl::BatchRef			mGlyphBoundsBatch;
	gl::GlslProgRef			mGlyphBoundsShader;


	//holds model matrix of each text
	gl::UboRef				mTextMatricesUbo;
	//holds all glpyh intsance data for looking up in the SDF
	gl::UboRef				mGlyphDataUbo;
	//holds data common to the text colors and extras
	gl::UboRef				mTextItemsUbo;

	CameraPersp				mCamera;
	CameraOrtho				mOrthoCamera;
	CameraUi				mUI;

	std::vector<gl::SdfText::GlyphData>	mGlyphData;
	std::vector<TextRenderItem>  mTextRenderItems;
	std::vector<mat4>			 mTextMatrices;

	std::vector<Lookup>			 mLookups;

};

Json::Value parseJson(const std::string &configName)
{
	Json::Value value;
	DataSourceRef asset;
	try {
		asset = loadAsset(configName);
	}
	catch (const ci::app::AssetLoadExc &e) {
		CI_LOG_E("Asset Problem: " << configName << ", " << e.what());
		return value;
	}
	CI_LOG_I(asset->getFilePath());
	auto jsonString = loadString(asset);
	Json::Features features;
	features.allowComments_ = true;
	features.strictRoot_ = false;
	Json::Reader reader(features);
	try {
		reader.parse(jsonString, value);
	}
	catch (const std::exception &e) {
		CI_LOG_E("Parsing " << configName << ", " << e.what());
	}
	catch (...) {
		CI_LOG_E("Error");
	}
	return value;
}

void TextRendererApp::setup()
{
	auto config = parseJson( "fontConfig.json" );

	auto size = config["texture size"].asUInt();
	auto re_saved_config = Json::Value(config);

	std::vector<Surface> sources;

	int i = 0; 
	for ( auto & font : config["fonts"] ) {
		
		int j = 0;
		for(auto & cached : font["cached"] )
		{

			mFonts.push_back(gl::SdfText::Font(loadAsset(font["font"].asString()), cached["size"].asUInt()));

			auto file = cached["path"].asString();
			auto path_to_cache = getAssetPath("") / "sdf" / (mFonts.back().getName() + "-" + std::to_string(int(mFonts.back().getSize())) + ".sdft");

			if (file.empty())
			{
				re_saved_config["fonts"][i]["cached"][j]["path"] = "sdf/" + (mFonts.back().getName() + "-" + std::to_string(int(mFonts.back().getSize())) + ".sdft");
				mSDText.push_back(gl::SdfText::loadTo3dTexture(path_to_cache, mFonts.back(), sources));
			}else{

				auto existing_cache = getAssetPath("") / file;
				if (fs::exists(existing_cache)) {
					mSDText.push_back(gl::SdfText::loadTo3dTexture(ci::DataSourcePath::create(existing_cache), mFonts.back().getSize(), sources));
				}
				else {
					re_saved_config["fonts"][i]["cached"][j]["path"] = "sdf/" + (mFonts.back().getName() + "-" + std::to_string(int(mFonts.back().getSize())) + ".sdft");
					mSDText.push_back(gl::SdfText::loadTo3dTexture(path_to_cache, mFonts.back(), sources));
				}

			}
			++j;
		}
		++i;
	}

	mSDTextures = gl::Texture3d::create( size, size, sources.size(), gl::Texture3d::Format().target(GL_TEXTURE_2D_ARRAY).internalFormat( GL_RGB8 ).label("Fonts texture") );

	CI_CHECK_GL();

	uint32_t layer = 0;
	for(auto & src : sources)
	{
		mSDTextures->update(src, layer++);
		CI_CHECK_GL();
	}

	//write back out exact file names
	auto writer = Json::StyledWriter();
	ofstream saved_config;
	saved_config.open( getAssetPath("") / "fontConfig.json" );
	saved_config << writer.write(re_saved_config);
	saved_config.close();

	try {
		mDebrisShader = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("instanced_debris.vert")).fragment(loadAsset("instanced_debris.frag")).attrib(geom::CUSTOM_2, "modelMatrix"));
	}
	catch (const gl::GlslProgCompileExc& e)
	{
		CI_LOG_E(e.what());
	}

	std::vector<mat4> modelMatrices;

	for (int i = 0; i < 100; i++ ) {
		
		auto pos = vec3(randFloat(-getWindowWidth()*.05, getWindowWidth()*.05), randFloat(-getWindowHeight()*.05, getWindowHeight()*.05), randFloat(20.,-50));
		auto scale = randFloat(1.,5.);
		mat4 transform = mat4();
		transform *= ci::translate(pos);
		transform *= ci::scale(vec3(scale));
		modelMatrices.push_back(transform);
	}

	auto models = gl::Vbo::create( GL_ARRAY_BUFFER, sizeof(mat4)*modelMatrices.size(), modelMatrices.data(), GL_STATIC_DRAW );

	auto mesh = gl::VboMesh::create(geom::Sphere());

	geom::BufferLayout layout;
	layout.append(geom::CUSTOM_2, 4, sizeof(mat4), 0, 1);
	layout.append(geom::CUSTOM_3, 4, sizeof(mat4), sizeof(vec4), 1);
	layout.append(geom::CUSTOM_4, 4, sizeof(mat4), sizeof(vec4)*2, 1);
	layout.append(geom::CUSTOM_5, 4, sizeof(mat4), sizeof(vec4)*3, 1);
	mesh->appendVbo( layout, models );

	mDebris = gl::Batch::create( mesh, mDebrisShader );

	try {
		mSDFShader = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("sdf_text.vert")).fragment(loadAsset("sdf_text.frag")).attrib(geom::CUSTOM_0, "ModelMatrixLookup").attrib(geom::CUSTOM_1,"GlyphLookup"));
		mSDFShader->uniformBlock("uTextRenderItems", 0);
		mSDFShader->uniformBlock("uTextMatrices", 1);
		mSDFShader->uniformBlock("uGlyphData", 2);
		mSDFShader->uniform( "uFonts", 0 );
	}
	catch (const gl::GlslProgCompileExc& e)
	{
		CI_LOG_E(e.what());
	}

	mCamera.setPerspective( 45, getWindowAspectRatio(), .1, 10000 );
	mCamera.lookAt( vec3( 0, 0, 50 ), vec3( 0 ) );

	mUI.setCamera( &mCamera );
	mUI.connect( getWindow() );

	mGlyphData.clear();
	
	i = 0;
	uint32_t num_glyphs = 0;
	for( auto & text : sText )
	{
		TextRenderItem item;
		item.color = vec4(randFloat(.5, 1.), randFloat(.5, 1.), randFloat(.5, 1.), randFloat() > .5 ? .75 : 1.);
		item.font_data = ivec4( randInt(mFonts.size()), i, 0, 0 );
		//wrap
		item.bounds = vec4(-1, -mFonts[item.font_data.x].getHeight()/2., 2, mFonts[item.font_data.x].getHeight());

		auto & sdf = mSDText[item.font_data.x];

		auto previous = num_glyphs;

		sdf->appendText( text, i, mGlyphData, vec2(0), gl::SdfText::DrawOptions().alignment(gl::SdfText::Alignment::LEFT).clipVertical(false).clipHorizontal(false) );

		num_glyphs = mGlyphData.size();

		for ( uint32_t j = 0; j < num_glyphs - previous; j++ )
		{
			Lookup lookup;
			lookup.matrix = i;
			lookup.glyph = previous + j;
			mLookups.push_back(lookup);
		}

		auto transform = mat4();
		auto pos = vec3(randFloat(-100,100), randFloat(-100, 100), randFloat(20., -50)) ;
		//transform *= ci::translate( pos );
	//	transform *= ci::rotate(randFloat()*float(M_PI)*2.f, vec3(1));
		//transform *= ci::scale( vec3(randFloat(1.,2.)) );
		mTextMatrices.push_back(transform);

		mTextRenderItems.push_back(item);
		++i;
	}

	auto glyph_mesh = gl::VboMesh::create(geom::Rect(Rectf(vec2(-.5), vec2(.5))));

	geom::BufferLayout glyph_lookups_layout;
	glyph_lookups_layout.append(geom::CUSTOM_0, geom::INTEGER, 1, sizeof(Lookup), offsetof(Lookup, matrix), 1);
	glyph_lookups_layout.append(geom::CUSTOM_1, geom::INTEGER, 1, sizeof(Lookup), offsetof(Lookup, glyph), 1);

	mGlyphLookups = gl::Vbo::create(GL_ARRAY_BUFFER, sizeof(Lookup) * mLookups.size(), mLookups.data(), GL_DYNAMIC_DRAW);

	glyph_mesh->appendVbo(glyph_lookups_layout, mGlyphLookups);

	mGlyphMesh = gl::Batch::create(glyph_mesh, mSDFShader);

	mTextItemsUbo = gl::Ubo::create( sizeof(TextRenderItem)*mTextRenderItems.size(), mTextRenderItems.data(), GL_STATIC_DRAW );
	mTextItemsUbo->bindBufferBase(0);

	mTextMatricesUbo = gl::Ubo::create(sizeof(mat4)*mTextMatrices.size(), mTextMatrices.data(), GL_STATIC_COPY);
	mTextMatricesUbo->bindBufferBase(1);

	mGlyphDataUbo = gl::Ubo::create( sizeof(gl::SdfText::GlyphData)*mGlyphData.size(), mGlyphData.data(), GL_STATIC_DRAW);
	mGlyphDataUbo->bindBufferBase(2);

	try {
		mBoundsShader = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("bounds.vert")).fragment(loadAsset("bounds.frag")));
	}
	catch (const gl::GlslProgCompileExc& e)
	{
		CI_LOG_E(e.what());
	}

	mBoundsBatch = gl::Batch::create( WireRect(Rectf(vec2(0), vec2(1))), mBoundsShader );

	try {
		mGlyphBoundsShader = gl::GlslProg::create( gl::GlslProg::Format().vertex(loadAsset("glyph_bounds.vert")).fragment(loadAsset("glyph_bounds.frag")).attrib(geom::CUSTOM_0, "ModelMatrixLookup").attrib(geom::CUSTOM_1, "GlyphLookup") );
	}
	catch (const gl::GlslProgCompileExc& e)
	{
		CI_LOG_E(e.what());
	}

	auto instanced_wire_mesh = gl::VboMesh::create(WireRect());
	instanced_wire_mesh->appendVbo( glyph_lookups_layout, mGlyphLookups );
	mGlyphBoundsBatch = gl::Batch::create( instanced_wire_mesh, mGlyphBoundsShader);

}

void TextRendererApp::mouseDown( MouseEvent event )
{
}

void TextRendererApp::update()
{
	CI_CHECK_GL();
	auto view = mCamera.getViewMatrix();
	std::stable_sort(mLookups.begin(), mLookups.end(), [&, view](const Lookup& first, const Lookup& second) -> bool {
		return (view * mTextMatrices[first.matrix] * vec4(vec2(mGlyphData[first.glyph].glyph), 0.f, 1.f)).z < (view * mTextMatrices[second.matrix] * vec4(vec2(mGlyphData[second.glyph].glyph), 0.f, 1.f)).z;
	});

	auto lookup = (Lookup*)mGlyphLookups->mapBufferRange(0, mGlyphLookups->getSize(), GL_MAP_WRITE_BIT );
	memcpy(lookup, mLookups.data(), mLookups.size()*sizeof(Lookup));
	mGlyphLookups->unmap();
	CI_CHECK_GL();

}

void TextRendererApp::draw()
{
	CI_CHECK_GL();

	//target
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::ScopedMatrices push;
	gl::setMatrices(mCamera);

	{   //opaque pass
		gl::ScopedDepthTest test(true);
		gl::ScopedDepthWrite write(true);
		mDebris->drawInstanced(100);
	}

	{   //transparent pass
		gl::ScopedDepthTest test(true);
		gl::ScopedDepthWrite write(false);
		gl::ScopedTextureBind fonts(mSDTextures, 0);
		glBlendEquation(GL_FUNC_ADD);
		gl::ScopedBlend blend(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		mGlyphMesh->drawInstanced(mGlyphData.size());
		mBoundsBatch->drawInstanced( mTextRenderItems.size() );
		mGlyphBoundsBatch->drawInstanced(mGlyphData.size());
	}

	CI_CHECK_GL();

}

CINDER_APP(TextRendererApp, RendererGl( RendererGl::Options().msaa(8) ), [](ci::app::App::Settings* settings) { settings->setFullScreen(); })
