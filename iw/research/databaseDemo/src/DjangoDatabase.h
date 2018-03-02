#pragma once

#include "HTTPClient.h"

class DjangoDatabase {
public:
	enum TAXONOMY {
		CATEGORY,
		SUBCATEGORY,
		CLOUD,
		PRODUCT,
		ALLERGEN,
		PARAMETER
	};

	static std::shared_ptr<DjangoDatabase> make(std::string root_url);
	~DjangoDatabase();
	void setCategory(const Json::Value& category);
	void getCategory();
	void setSubCategory(const Json::Value& subcategory);
	void getSubCategories();
	void getSubCategory(const std::string& uuid);
	void setCloud(const Json::Value& cloud);
	void getClouds();
	void getCloud(const std::string& uuid);
	void setProduct(const Json::Value& product);
	void getProducts();
	void getProduct(const std::string& sku);
	void setAllergen(const Json::Value& allergen);
	void getAllergens();
	void getAllergen(const std::string& name);
	void setParameter(const Json::Value& parameter);
	void getParameters();
	void getParameter(const std::string& key);
protected:
	DjangoDatabase(std::string url);
	std::shared_ptr<midnight::http::HttpClient> mClient;
	std::string mRootUrl;
	Json::FastWriter mJsonWriter;
};