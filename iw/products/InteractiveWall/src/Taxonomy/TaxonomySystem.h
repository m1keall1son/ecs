#pragma once

#include "ecs/engine/ISystem.h"
#include "ecs/utils/ObjectPool.hpp"
#include "jsoncpp/json.h"
#include "ProductComponent.h"
#include "SubcategoryComponent.h"
#include "CloudComponent.h"

class TaxonomySystem : public ISystem
{
public:

	SYSTEM_CREATE_H(TaxonomySystem)

	void load(std::atomic<float>* progress, float progress_step)override;
	void loadConfig(const Json::Value& config) override;
	void saveConfig() override {}
	void initPools(uint32_t initial_size)override;
	void init() override;
	void postInit()override;
	void update()override;
	void draw() override;
	void shutdown() override;
	void enableDebug(bool debug = true)override;

private:

	Handle newComponent( const Handle& entity_handle, const Json::Value& model );

	TaxonomySystem(Scene* context) : ISystem(context), mDebug(false) {}

	bool mDebug;

	ObjectPool<ProductComponent> mProducts;
	ObjectPool<CloudComponent> mClouds;
	ObjectPool<SubcategoryComponent> mSubcategories;

	friend ProductComponent;
	friend CloudComponent;
	friend SubcategoryComponent;
};