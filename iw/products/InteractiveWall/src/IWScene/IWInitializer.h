#pragma once

#include "ecs/engine/ISceneInitializer.h"
#include "ecs/utils/Handle.h"
#include "../Taxonomy/DatabaseAPI.h"
#include "../Taxonomy/TaxonomySystem.h"
#include "PersonDetector.h"
#include "Layout.h"
#include "ecs/utils/Edge.h"

class IWInitializer : public ISceneInitializer
{
public:

	static ISceneInitializerRef create(Scene* context) { return std::shared_ptr<IWInitializer>(new IWInitializer(context)); }

	//Async scene loading
	void load(std::atomic<float>* progress, float progress_step)override;

	void init()override {}

	//creates the first scene logic
	ISceneLogicStateRef beginLogic()override;

	Json::Value& getCategoryData() { return mCategoryJson; }
	Json::Value& getSubCategoryData(int uuid) { return mSubCategoryMap[uuid]; }
	std::map<int, Json::Value>& getSubCategoryMap() { return mSubCategoryMap; }
	Json::Value& getCloudData(int uuid) { return mCloudMap[uuid]; }
	std::map<int, Json::Value>& getCloudMap() { return mCloudMap; }
	Json::Value& getProductData(std::string sku) { return mProductMap[sku]; }
	std::map<std::string, Json::Value>& getProductMap() { return mProductMap; }
	uint32_t getProductTextureId(std::string sku) { return mProductTextureMap[sku]; }
	PersonDetectorRef getPersonDetector() { return mPersonDetector; }

	Layout::Subcategory & getLayout(int subcategory_uuid) { return mLayouts[subcategory_uuid]; }
	std::vector<Handle> & getEngageLayout() { return mEngageLayout; }
	std::vector< std::pair< std::vector<int>, std::vector< std::pair<int, int>>>> & getSpringConnections(int subcategory_uuid) { return mLayoutSpringConnections[subcategory_uuid]; }

	Handle getBackgroundButton() { return mBackgroundButton; }

private:

	IWInitializer(Scene* context) :ISceneInitializer(context) {}

	std::shared_ptr<DatabaseAPI> mAPI;
	std::shared_ptr<midnight::http::HttpClient> mHttpClient;
	
	// maps for looking up JSON by uuid
	Json::Value mCategoryJson;
	std::map<int, Json::Value> mSubCategoryMap;
	std::map<int, Json::Value> mCloudMap;
	std::map<std::string, Json::Value> mProductMap;
	std::map<std::string, uint32_t> mProductTextureMap;
	PersonDetectorRef mPersonDetector;
	std::map<int, Layout::Subcategory> mLayouts;
	std::map<int, std::vector< std::pair< std::vector<int>, std::vector< std::pair<int, int>>>>> mLayoutSpringConnections;

	std::vector<Handle> mEngageLayout;
	Handle mBackgroundButton;
};
