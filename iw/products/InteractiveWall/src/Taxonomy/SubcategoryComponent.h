#pragma once

#include "ecs/Component.h"
#include "TaxonomyEnums.h"
#include "jsoncpp/json.h"
#include <vector>
#include <string>
#include <chrono>

class TaxonomySystem;

class SubcategoryComponent : public Component {
public:

	COMPONENT_CREATE_H(SubcategoryComponent);

	using system_type = TaxonomySystem;

	SubcategoryComponent() = default;
	~SubcategoryComponent() = default;

	SubcategoryComponent(const SubcategoryComponent&) = delete;
	SubcategoryComponent& operator=(const SubcategoryComponent&) = delete;

	SubcategoryComponent(SubcategoryComponent&& rhs);
	SubcategoryComponent& operator=(SubcategoryComponent&& rhs);

	void init( const Json::Value& model );
	void destroy();
	std::string getUUID() { return mUUID; }
	void update();

private:
	ModelType mType;
	std::string mUUID;
	std::string mName;
	std::string mDescription;
	int mPriority;
	std::string mPromotion;
	std::string mIconPath;
	std::string mEtag;

	std::vector<Handle> mClouds;

	friend TaxonomySystem;

};