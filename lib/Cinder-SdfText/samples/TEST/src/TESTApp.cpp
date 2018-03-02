#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_SIZES_H
#include "freetype/ftsnames.h"
#include "freetype/ttnameid.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TESTApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	FT_Library mLibrary;

};

void TESTApp::setup()
{
	FT_Error error = FT_Init_FreeType(&mLibrary);
	if (FT_Err_Ok != error) {
		throw FontInvalidNameExc("Failed to initialize FreeType2");
	}

	FT_Face arial_face;
	std::string face_name;

	auto data = Buffer(loadAsset("TargetHelvetica-Medium.ttf"));
	//auto data = Buffer(loadFile("C:\\Windows\\Fonts\\Arial.ttf"));

	error = FT_New_Memory_Face(
		mLibrary,
		reinterpret_cast<FT_Byte*>(data.getData()),
		static_cast<FT_Long>(data.getSize()),
		0,
		&arial_face
	);

	if (FT_Err_Ok != error) {
		throw std::runtime_error("Failed to load font data");
	}

	FT_Select_Charmap( arial_face, FT_ENCODING_UNICODE );

	float font_size_pt = 50.f;
	FT_F26Dot6 font_size = static_cast<FT_F26Dot6>( font_size_pt * 64.0f );

	FT_Set_Char_Size( arial_face, 0, font_size, 96, 96 );
	//FT_Set_Pixel_Sizes(arial_face, 10,10);

	//in points
	auto height = arial_face->size->metrics.height / 64.f;
	auto acent = arial_face->size->metrics.ascender / 64.f;
	auto descent = arial_face->size->metrics.descender / 64.f;

	auto em_size = 1.0 * arial_face->units_per_EM;
	auto ppem_x = arial_face->size->metrics.x_ppem;
	auto ppem_y = arial_face->size->metrics.y_ppem;
	auto scale_x = arial_face->size->metrics.x_scale / 65535.f;
	auto scale_y = arial_face->size->metrics.y_scale / 65535.f;

	auto index = FT_Get_Char_Index(arial_face, 0x42);
	FT_Load_Glyph(arial_face, index, FT_LOAD_DEFAULT);

	// Extract the name if needed

	FT_SfntName sn = {};
	if( FT_Err_Ok == FT_Get_Sfnt_Name( arial_face, TT_NAME_ID_FULL_NAME, &sn ) ) {
		// If full name isn't available use family and style name
		if( sn.string_len > 0  && ( 0 == sn.string[0] ) ) {
			// Fallback to this name
			face_name = "(Unknown)";
			std::string familyName = arial_face->family_name;
			std::string styleName = arial_face->style_name;
			if( ! familyName.empty() ) {
				face_name = familyName;
				if( ! styleName.empty() ) {
					face_name += ( "-" + styleName );
				}
			}
		}
		else {
			face_name = std::string( reinterpret_cast<const char *>( sn.string ), sn.string_len );
		}
	}
	

	font_size_pt = 20.f;
	font_size = static_cast<FT_F26Dot6>(font_size_pt * 64.0f);

	FT_Set_Char_Size(arial_face, 0, font_size, 0, 72);

	FT_Done_Face(arial_face);

	FT_Done_FreeType(mLibrary);

}

void TESTApp::mouseDown( MouseEvent event )
{
}

void TESTApp::update()
{
}

void TESTApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( TESTApp, RendererGl )
