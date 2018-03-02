#pragma once
#include "ecs/engine/ISceneInitializer.h"
#include "ecs/utils/Handle.h"
#include "../Taxonomy/DatabaseAPI.h"
#include <array>
#include "PersonDetector.h"
#include "ecs/events/EventData.h"

class DemoInit : public ISceneInitializer
{
public:

	static ISceneInitializerRef create(Scene* context) { return std::shared_ptr<DemoInit>(new DemoInit(context)); }

	//Async scene loading
	void load(std::atomic<float>* progress, float progress_step)override;

	void init()override;

	//creates the first scene logic
	ISceneLogicStateRef beginLogic()override;

	Handle createToken(const ci::vec2& position);

	Handle getBackButton() { return mBackButton; }
	Handle getVisualState() { return mVisualState; }
	Handle getFarAway() { return mFarAway; }
	std::array<Handle, 3>& getEngageProducts() { return mEngageProducts; }

	int getVisualState( const std::string& name ) { return mVisualStateMap[name]; }
	ci::vec2 getTokenButtonPosition( int state ) { return mTokenButtonPositions[state]; }

	void clearTokens();

	~DemoInit();

private:

	DemoInit(Scene* context) :ISceneInitializer(context) {}

	void onCollection(EventDataRef);

	Handle mTokenWall;
	Handle mBackButton;
	Handle mVisualState;
	Handle mFarAway;
	std::array<Handle, 3> mEngageProducts;

	std::map<std::string, int> mVisualStateMap;

	ci::vec2 mTokenScale;
	std::vector<Handle> mTokens;
	std::map<int, ci::vec2> mTokenButtonPositions;

	Handle mVideo;
	ci::signals::Connection mKeyPress;


};
