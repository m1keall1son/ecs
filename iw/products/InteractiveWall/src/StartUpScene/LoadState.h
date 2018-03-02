#pragma once

#include "ecs/engine/ISceneLogicState.h"
#include "ecs/utils/Handle.h"
#include "ecs/events/EventData.h"

class LoadState : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context) { return std::unique_ptr<LoadState>(new LoadState(context)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	float mLoadProgress{ 0.f };

	LoadState(Scene* context) : ISceneLogicState(context) {}

};