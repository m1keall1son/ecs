#include "ProductComponent.h"
#include "ecs/engine/Engine.h"

COMPONENT_CREATE_CPP(ProductComponent)

ProductComponent::ProductComponent(ProductComponent && rhs) :
	Component(std::move(rhs)),
	mType(std::move(rhs.mType)),
	mName(std::move(rhs.mName)),
	mBrand(std::move(rhs.mBrand)),
	mDescription(std::move(rhs.mDescription)),
	mPromotion(std::move(rhs.mPromotion)),
	mBarcode(std::move(rhs.mBarcode)),
	mSku(std::move(rhs.mSku)),
	mListPrice(std::move(rhs.mListPrice)),
	mInStock(std::move(rhs.mInStock)),
	mDisplaySize(std::move(rhs.mDisplaySize)),
	mSaleStart(std::move(rhs.mSaleStart)),
	mSaleEnd(std::move(rhs.mSaleEnd)),
	mSalePrice(std::move(rhs.mSalePrice)),
	mSaleDescription(std::move(rhs.mSaleDescription)),
	mImagePath(std::move(rhs.mImagePath)),
	mThumbnailPath(std::move(rhs.mThumbnailPath)),
	mEtag(std::move(rhs.mEtag)) {
}

ProductComponent & ProductComponent::operator=(ProductComponent && rhs)
{
	Component::operator=(std::move(rhs));
	mName = std::move(rhs.mName);
	mType = std::move(rhs.mType);
	mName = std::move(rhs.mName);
	mBrand = std::move(rhs.mBrand);
	mDescription = std::move(rhs.mDescription);
	mPromotion = std::move(rhs.mPromotion);
	mBarcode = std::move(rhs.mBarcode);
	mSku = std::move(rhs.mSku);
	mListPrice = std::move(rhs.mListPrice);
	mInStock = std::move(rhs.mInStock);
	mDisplaySize = std::move(rhs.mDisplaySize);
	mSaleStart = std::move(rhs.mSaleStart);
	mSaleEnd = std::move(rhs.mSaleEnd);
	mSalePrice = std::move(rhs.mSalePrice);
	mSaleDescription = std::move(rhs.mSaleDescription);
	mImagePath = std::move(rhs.mImagePath);
	mThumbnailPath = std::move(rhs.mThumbnailPath);
	mEtag = std::move(rhs.mEtag);

	return *this;
}

void ProductComponent::init(const Json::Value& model) {
	try {
		mName = model["name"].asString();
		mType = ModelType(model["type"].asInt());
		mBrand = model["brand"].asString();
		mDescription = model["description"].asString();
		mPromotion = model["promotion"].asString();
		mBarcode = model["barcode"].asString();
		mSku = model["sku"].asString();
		mListPrice = model["price"].asString();
		mInStock = model["in-stock"].asBool();

		mDisplaySize = ProductDisplaySize(model["display_size"].asInt());

		mSalePrice = model["sale_price"].asString();
		mSaleDescription = model["sale_description"].asString();
		mSaleStart = std::chrono::time_point<std::chrono::system_clock>();
		mSaleEnd = std::chrono::time_point<std::chrono::system_clock>();

		mImagePath = model["image"].asString();
		mThumbnailPath = model["thumbnail"].asString();

		mEtag = model["etag"].asString();
	}

	catch (const std::exception& e) {
		CI_LOG_E(e.what());
	}

	reconstructEntity();
}

void ProductComponent::destroy() {
	mName.clear();
	mType = ModelType(0);
	mBrand.clear();
	mDescription.clear();
	mPromotion.clear();
	mBarcode.clear();
	mSku.clear();
	mListPrice = 0.0;
	mInStock = false;
	mDisplaySize = ProductDisplaySize::STANDARD;
	mSaleStart = std::chrono::time_point<std::chrono::system_clock>();
	mSaleEnd = std::chrono::time_point<std::chrono::system_clock>();
	mSalePrice = 0.0;
	mSaleDescription.clear();
	mImagePath.clear();
	mThumbnailPath.clear();
	mEtag.clear();

	Component::destroy();
}

void ProductComponent::update() {
}

void ProductComponent::reconstructEntity() {
	//use all data to construct the proper compound entity.
}
