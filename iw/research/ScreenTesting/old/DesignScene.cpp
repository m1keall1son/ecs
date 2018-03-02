#include "DesignScene.h"
#include "cinder/ImageIo.h"

using namespace ci;
using namespace ci::app;

void DesignScene::touchesBegan(ci::app::TouchEvent event)
{
}

void DesignScene::touchesMoved(ci::app::TouchEvent event)
{
}

void DesignScene::touchesEnded(ci::app::TouchEvent event)
{
}

void DesignScene::mouseDown(ci::app::MouseEvent event)
{
}

void DesignScene::mouseDrag(ci::app::MouseEvent event)
{
}

void DesignScene::mouseUp(ci::app::MouseEvent event)
{
}

void DesignScene::setup()
{
	mParams = params::InterfaceGl::create("Desing Assets", vec2(200, 200));
	mParams->setPosition(vec2( 50, 400 ));

	mFullScreenQuad = gl::Batch::create( geom::Rect(Rectf( vec2(0), vec2(getWindowSize()) )), gl::getStockShader(gl::ShaderDef().texture()) );

	mParams->addText("65\" 4k\"");
	auto product_size_tex_65 = gl::Texture::create( loadImage( loadAsset("images/Planar 65 4K/Product Szie.png") ) );
	mParams->addButton("65\" Product Size", [&, product_size_tex_65] { mCurrentTexture = product_size_tex_65; });
	auto italic_size_tex_65 = gl::Texture::create(loadImage(loadAsset("images/Planar 65 4K/Typography Italic Size.png")));
	mParams->addButton("65\" Typography Italic Size", [&, italic_size_tex_65] { mCurrentTexture = italic_size_tex_65; });
	auto typography_size_65 = gl::Texture::create(loadImage(loadAsset("images/Planar 65 4K/Typography Size.png")));
	mParams->addButton("65\" Typography Size", [&, typography_size_65] { mCurrentTexture = typography_size_65; });

	mParams->addText("75\" 4k\"");
	auto product_size_tex_75 = gl::Texture::create(loadImage(loadAsset("images/Planar 75 4K/Product Size.png")));
	mParams->addButton("75\" Product Size", [&, product_size_tex_75] { mCurrentTexture = product_size_tex_75; });
	auto italic_size_tex_75 = gl::Texture::create(loadImage(loadAsset("images/Planar 75 4K/Typography Italic Size.png")));
	mParams->addButton("75\" Typography Italic Size", [&, italic_size_tex_75] { mCurrentTexture = italic_size_tex_75; });
	auto typography_size_75 = gl::Texture::create(loadImage(loadAsset("images/Planar 75 4K/Typography Size.png")));
	mParams->addButton("75\" Typography Size", [&, typography_size_75] { mCurrentTexture = typography_size_75; });

	mCurrentTexture = product_size_tex_65;

	mParams->hide();

}

void DesignScene::update()
{
}

void DesignScene::pause()
{
	mParams->hide();
}

void DesignScene::play()
{
	mParams->show();
}

void DesignScene::draw()
{
	gl::ScopedTextureBind tex( mCurrentTexture, 0 );
	mFullScreenQuad->draw();
	mParams->draw();
}
