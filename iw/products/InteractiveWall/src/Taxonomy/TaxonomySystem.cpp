#include "cinder/Log.h"
#include "TaxonomySystem.h"
#include "ecs/engine/Engine.h"
#include "CloudComponent.h"
#include "ProductComponent.h"
#include "SubcategoryComponent.h"

SYSTEM_CREATE_CPP(TaxonomySystem);


void TaxonomySystem::load(std::atomic<float>* progress, float progress_step)
{
	
}

void TaxonomySystem::loadConfig(const Json::Value & config) {
	int product_pool_size = config["product_pool_size"].asUInt();
	mProducts.initPool(product_pool_size);

	int cloud_pool_size = config["cloud_pool_size"].asUInt();
	mClouds.initPool(cloud_pool_size);

	int subcategory_pool_size = config["subcategory_pool_size"].asUInt();
	mSubcategories.initPool(subcategory_pool_size);
}

void TaxonomySystem::initPools(uint32_t initial_size) {
}

void TaxonomySystem::init() {
}

void TaxonomySystem::postInit() {
}

void TaxonomySystem::update() {
}

void TaxonomySystem::draw()
{
}

void TaxonomySystem::shutdown() {
	mProducts.clear();
	mClouds.clear();
	mSubcategories.clear();
}

void TaxonomySystem::enableDebug(bool debug) {
}

Handle TaxonomySystem::newComponent(const Handle& entity_handle, const Json::Value& model) {
	Json::Value json = model["type"].asInt();

	if (!json.isNull()) {
		int modelType = json.asInt();
		if (modelType == PRODUCT) {
			auto handle = mProducts.init_new(model);
			handle.get_fast<ProductComponent>()->setEntity(entity_handle);
			return handle;
		}
		else if (modelType == CLOUD) {
			auto handle = mClouds.init_new(model);
			handle.get_fast<CloudComponent>()->setEntity(entity_handle);
			return handle;
		}
		else if (modelType == SUBCATEGORY) {
			auto handle = mSubcategories.init_new(model);
			handle.get_fast<SubcategoryComponent>()->setEntity(entity_handle);
			return handle;
		}
		else {
			CI_LOG_E("Object model 'type' field is not Product, Cloud, or SubCategory, its: " << modelType);
			return Handle();
		}
	}
	else {
		CI_LOG_E("Object model has no 'type' field!");
		return Handle();
	}
}
