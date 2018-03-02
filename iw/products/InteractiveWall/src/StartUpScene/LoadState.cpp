#include "LoadState.h"
#include "StartUpInit.h"
#include "cinder/app/App.h"
#include "ECS.h"

using namespace ci;
using namespace ci::app;

void LoadState::update()
{
	auto init = getScene()->getInitializer<StartUpInit>();

	static bool loaded = false;

	//if(init->mAPI->isSyncingFinished()){
	if(true) {
		if (!loaded) {
			Engine::get()->loadScene("IWScene");
			std::printf("Scene finished loading!\n");
			loaded = true;
		}
	}

	//TODO this should be an image sqeuence of a spinning wheel or something instead of a progress bar. since we are not doing a good job of breaking down the progress, lol
	auto & render_system = *Engine::get()->scene()->getSystem<RenderSystem>();

	if (auto progress_bar = init->mLoadingProgressBar.get<Entity>()) {
		progress_bar->setScale(vec3(500.f*mLoadProgress, 10, 0));
		if (auto rc = progress_bar->getComponent<RenderComponent>()) {
			auto color_handle = rc->getMaterialInstanceHandle( render_system.getMaterialId("ColorMaterial") );
			if ( auto color = color_handle.get_fast<ColorMaterialInstance>() ) {
				color->setColor( vec4( lmap( cos( ci::app::getElapsedSeconds()*2. ), -1., 1., .6, 1.)  ) );
			}
		}
	}
}

void LoadState::enter()
{
}

void LoadState::exit()
{
}

ISceneLogicStateRef LoadState::nextLogicState()
{
	//end state
	return nullptr;
}
