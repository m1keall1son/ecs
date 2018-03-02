#pragma once

#include <string>
#include <atomic>
#include "HttpClient.h"

class DatabaseAPI {
public:
	static std::shared_ptr<DatabaseAPI> make();
	~DatabaseAPI();
	void init();
	Json::Value getCategory();
	Json::Value getSubCategory(int uuid);
	Json::Value getCloud(int uuid);
	Json::Value getProduct(std::string sku);
	Json::Value getAllergen(std::string name);
	void getParameters(std::string name);
	std::atomic<bool>& isSyncingFinished();
	void syncCategory();
	void syncSubCategory(int uuid);
	void syncCloud(int uuid);
	void syncProduct(std::string sku);
	void syncAllergen(std::string name);
	void syncParameters(std::string name);
	std::string getRemoteServer();
	std::string getLocalServer();
protected:
	DatabaseAPI();
	static const std::string mLocalServer;
	static const std::string mRemoteServer;
	int mCategoryNumber;
	std::shared_ptr<midnight::http::HttpClient> mHttpClient;
	std::atomic<bool> mIsCategoryLoadingFinished;
};

