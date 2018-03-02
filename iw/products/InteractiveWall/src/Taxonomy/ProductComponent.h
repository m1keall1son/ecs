#pragma once

#include "ecs/Component.h"
#include "TaxonomyEnums.h"
#include "jsoncpp/json.h"
#include <vector>
#include <string>
#include <chrono>

class TaxonomySystem;

class ProductComponent : public Component {
	public:

	using system_type = TaxonomySystem;

	COMPONENT_CREATE_H(ProductComponent)

	ProductComponent() = default;
	~ProductComponent() = default;

	ProductComponent(const ProductComponent&) = delete;
	ProductComponent& operator=(const ProductComponent&) = delete;

	ProductComponent(ProductComponent&& rhs);
	ProductComponent& operator=(ProductComponent&& rhs);

	void init(const Json::Value& model);
	void destroy();

	void update();

	std::string getSku() { return mSku; }
private:

	void reconstructEntity();

	ModelType mType;
	std::string mName;
	std::string mBrand;
	std::string mDescription;
	std::string mPromotion;
	std::string mBarcode;
	std::string	mSku;
	std::string	mListPrice;
	bool		mInStock;
	ProductDisplaySize mDisplaySize;
	std::chrono::time_point<std::chrono::system_clock> mSaleStart;
	std::chrono::time_point<std::chrono::system_clock> mSaleEnd;
	std::string mSalePrice;
	std::string mSaleDescription;
	std::string mImagePath;
	std::string mThumbnailPath;
	std::string mEtag;

	friend TaxonomySystem;

};