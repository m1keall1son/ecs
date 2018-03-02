#pragma once

#include "ecs/engine/ISceneInitializer.h"
#include "ecs/utils/Handle.h"
#include "../Taxonomy/DatabaseAPI.h"

class StartUpInit : public ISceneInitializer
{
public:

	static ISceneInitializerRef create(Scene* context) { return std::shared_ptr<StartUpInit>(new StartUpInit(context)); }

	//Async scene loading
	void load(std::atomic<float>* progress, float progress_step)override;

	void init()override {}

	//creates the first scene logic
	ISceneLogicStateRef beginLogic()override;

private:

	StartUpInit(Scene* context) :ISceneInitializer(context) {}

	Handle mLoadingProgressBar;
	Handle mLoadingText;
	
	std::shared_ptr<DatabaseAPI> mAPI;

	friend class LoadState;

};
