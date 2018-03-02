#pragma once

#include "ecs/Component.h"
#include "TaxonomyEnums.h"
#include "jsoncpp/json.h"
#include <vector>
#include <string>
#include <chrono>

class TaxonomySystem;

class CloudComponent : public Component {
public:
	COMPONENT_CREATE_H(CloudComponent)

	using system_type = TaxonomySystem;

	CloudComponent() = default;
	~CloudComponent() = default;

	CloudComponent(const CloudComponent&) = delete;
	CloudComponent& operator=(const CloudComponent&) = delete;

	CloudComponent(CloudComponent&& rhs);
	CloudComponent& operator=(CloudComponent&& rhs);

	void init(const Json::Value& model);
	void destroy();
	void update();

private:

	ModelType mType;
	std::string mUUID;
	std::string mName;
	std::string mDescription;

	std::string mPromotion;
	std::string mIconPath;
	std::string mEtag;

	std::vector<Handle> mProducts;

	friend TaxonomySystem;

};