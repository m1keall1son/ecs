#include "CloudComponent.h"

#include "ecs/engine/Engine.h"

COMPONENT_CREATE_CPP(CloudComponent)

CloudComponent::CloudComponent(CloudComponent && rhs) :
	Component(std::move(rhs)),
	mType(std::move(rhs.mType)),
	mUUID(std::move(rhs.mUUID)),
	mName(std::move(rhs.mName)),
	mDescription(std::move(rhs.mDescription)),
	mPromotion(std::move(rhs.mPromotion)),
	mIconPath(std::move(rhs.mIconPath)),
	mEtag(std::move(rhs.mEtag)),
	mProducts(std::move(rhs.mProducts)) {
}

CloudComponent & CloudComponent::operator=(CloudComponent && rhs) {
	Component::operator=(std::move(rhs));
	mType = std::move(rhs.mType);
	mUUID = std::move(rhs.mUUID);
	mName = std::move(rhs.mName);
	mDescription = std::move(rhs.mDescription);
	mPromotion = std::move(rhs.mPromotion);
	mIconPath = std::move(rhs.mIconPath);
	mEtag = std::move(rhs.mEtag);
	mProducts = std::move(rhs.mProducts);
	return *this;
}

void CloudComponent::init(const Json::Value& model) {
	mType = ModelType(model["type"].asInt());
	mUUID = model["uuid"].asString();
	mName = model["name"].asString();
	mDescription = model["description"].asString();
	mPromotion = model["promotion"].asString();
	mIconPath = model["icon"].asString();
	mEtag = model["etag"].asString();

	/*
	for(auto product : model["products"]) {
		std::string sku = product.asString();
		// get product from sku here
		mProducts.push_back();
	}
	*/
}

void CloudComponent::destroy() {
	mUUID.clear();
	mName.clear();
	mDescription.clear();
	mPromotion.clear();
	mIconPath.clear();
	mProducts.clear();
	mEtag.clear();
	Component::destroy();
}

void CloudComponent::update()
{
}
