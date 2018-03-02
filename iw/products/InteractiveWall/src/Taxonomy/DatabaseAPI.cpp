#include "DatabaseAPI.h"
#include "cinder/app/App.h"
#include "ECS.h"

const std::string DatabaseAPI::mLocalServer = "http://127.0.0.1:8000/";
const std::string DatabaseAPI::mRemoteServer = "http://127.0.0.1:8000/";

std::shared_ptr<DatabaseAPI> DatabaseAPI::make() {
	return std::shared_ptr<DatabaseAPI>(new DatabaseAPI());
}


void DatabaseAPI::init() {
	mHttpClient = midnight::http::HttpClient::make();

	Json::Value config = util::parseJson("config/database/database_config.json");
	mCategoryNumber = config["category"].asInt();

	mIsCategoryLoadingFinished = false;
}

Json::Value DatabaseAPI::getCategory() {
	midnight::http::HttpRequest getCategory;
	getCategory.mMethod = midnight::http::HTTP_GET;
	getCategory.mUrl = mRemoteServer + "category/" + std::to_string(mCategoryNumber) + "/";
	getCategory.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);
	};

	midnight::http::HttpResponse response = mHttpClient->makeRequest(getCategory);
	return response.mBody;
}

Json::Value DatabaseAPI::getSubCategory(int uuid) {
	midnight::http::HttpRequest getSubCategory;
	getSubCategory.mMethod = midnight::http::HTTP_GET;
	getSubCategory.mUrl = mRemoteServer + "subcategory/" + std::to_string(uuid) + "/";
	getSubCategory.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);
	};

	midnight::http::HttpResponse response = mHttpClient->makeRequest(getSubCategory);
	return response.mBody;
}

Json::Value DatabaseAPI::getCloud(int uuid) {
	midnight::http::HttpRequest getCloud;
	getCloud.mMethod = midnight::http::HTTP_GET;
	getCloud.mUrl = mRemoteServer + "cloud/" + std::to_string(uuid) + "/";
	getCloud.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);
	};

	midnight::http::HttpResponse response = mHttpClient->makeRequest(getCloud);
	return response.mBody;
}

Json::Value DatabaseAPI::getProduct(std::string sku) {
	midnight::http::HttpRequest getProduct;
	getProduct.mMethod = midnight::http::HTTP_GET;
	getProduct.mUrl = mRemoteServer + "product/" + sku + "/";
	getProduct.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);
	};

	midnight::http::HttpResponse response = mHttpClient->makeRequest(getProduct);
	return response.mBody;
}

Json::Value DatabaseAPI::getAllergen(std::string name) {
	midnight::http::HttpRequest getAllergen;
	getAllergen.mMethod = midnight::http::HTTP_GET;
	getAllergen.mUrl = mRemoteServer + "allergen/" + name + "/";
	getAllergen.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {
		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);
	};

	midnight::http::HttpResponse response = mHttpClient->makeRequest(getAllergen);
	return response.mBody;
}

void DatabaseAPI::getParameters(std::string name) {

}

std::atomic<bool>& DatabaseAPI::isSyncingFinished() {
	return mIsCategoryLoadingFinished;
}


void DatabaseAPI::syncCategory() {
	midnight::http::HttpRequest getCategory;
	getCategory.mMethod = midnight::http::HTTP_GET;
	getCategory.mUrl = mRemoteServer + "category/" + std::to_string(mCategoryNumber) + "/";
	getCategory.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);

		// get json object, put it into a PUT request and send it to the local database.
		midnight::http::HttpRequest pushCategory;
		pushCategory.mMethod = midnight::http::HTTP_PUT;
		pushCategory.mUrl = mLocalServer + "category/" + std::to_string(mCategoryNumber) + "/";
		pushCategory.mRequestBody = mHttpClient->jsonToString(response->mBody);
		pushCategory.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {
			std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		};

		client->addRequest(pushCategory);

		// parse category information and begin cascade of reading subobjects
		for (auto subcategory : response->mBody["inventory"]) {
			syncSubCategory(atoi(subcategory.asCString()));
		}
		mIsCategoryLoadingFinished = true;
	};

	mHttpClient->addRequest(getCategory);
}

void DatabaseAPI::syncSubCategory(int uuid) {
	midnight::http::HttpRequest getSubCategory;
	getSubCategory.mMethod = midnight::http::HTTP_GET;
	getSubCategory.mUrl = mRemoteServer + "subcategory/" + std::to_string(uuid) + "/";
	getSubCategory.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);

		// get json object, put it into a PUT request and send it to the local database.
		midnight::http::HttpRequest pushSubCategory;
		pushSubCategory.mMethod = midnight::http::HTTP_PUT;
		pushSubCategory.mUrl = mLocalServer + "subcategory/" + std::to_string(uuid) + "/";
		pushSubCategory.mRequestBody = mHttpClient->jsonToString(response->mBody);
		pushSubCategory.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {
			std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
			client->checkHttpStatus(response->mResponseCode);
		};

		client->addRequest(pushSubCategory);

		for (auto cloud : response->mBody["clouds"]) {
			syncCloud(atoi(cloud.asCString()));
		}
	};

	mHttpClient->addRequest(getSubCategory);
}

void DatabaseAPI::syncCloud(int uuid) {
	midnight::http::HttpRequest getCloud;
	getCloud.mMethod = midnight::http::HTTP_GET;
	getCloud.mUrl = mRemoteServer + "cloud/" + std::to_string(uuid) + "/";
	getCloud.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);

		// get json object, put it into a PUT request and send it to the local database.
		midnight::http::HttpRequest pushCloud;
		pushCloud.mMethod = midnight::http::HTTP_PUT;
		pushCloud.mUrl = mLocalServer + "cloud/" + std::to_string(uuid) + "/";
		pushCloud.mRequestBody = mHttpClient->jsonToString(response->mBody);
		pushCloud.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {
			std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
			client->checkHttpStatus(response->mResponseCode);
		};

		client->addRequest(pushCloud);

		for (auto product : response->mBody["products"]) {
			syncProduct(product.asString());
		}
	};

	mHttpClient->addRequest(getCloud);
}

void DatabaseAPI::syncProduct(std::string sku) {
	midnight::http::HttpRequest getProduct;
	getProduct.mMethod = midnight::http::HTTP_GET;
	getProduct.mUrl = mRemoteServer + "product/" + sku + "/";
	getProduct.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);

		// get json object, put it into a PUT request and send it to the local database.
		midnight::http::HttpRequest pushProduct;
		pushProduct.mMethod = midnight::http::HTTP_PUT;
		pushProduct.mUrl = mLocalServer + "product/" + sku + "/";
		pushProduct.mRequestBody = mHttpClient->jsonToString(response->mBody);
		pushProduct.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {
			std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
			client->checkHttpStatus(response->mResponseCode);
		};

		client->addRequest(pushProduct);

		for (auto allergen : response->mBody["allergens"]) {
			syncAllergen(allergen.asString());
		}
	};

	mHttpClient->addRequest(getProduct);
}

void DatabaseAPI::syncAllergen(std::string name) {
	midnight::http::HttpRequest getAllergen;
	getAllergen.mMethod = midnight::http::HTTP_GET;
	getAllergen.mUrl = mRemoteServer + "allergen/" + name + "/";
	getAllergen.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {

		std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
		client->checkHttpStatus(response->mResponseCode);

		// get json object, put it into a PUT request and send it to the local database.
		midnight::http::HttpRequest pushAllergen;
		pushAllergen.mMethod = midnight::http::HTTP_PUT;
		pushAllergen.mUrl = mLocalServer + "allergen/" + name + "/";
		pushAllergen.mRequestBody = mHttpClient->jsonToString(response->mBody);
		pushAllergen.mCallback = [=](midnight::http::HttpResponse* response, midnight::http::HttpClient* client) {
			std::printf("JSON Returned: %s\n", client->jsonToString(response->mBody).c_str());
			client->checkHttpStatus(response->mResponseCode);
		};

		client->addRequest(pushAllergen);
	};

	mHttpClient->addRequest(getAllergen);
}

void DatabaseAPI::syncParameters(std::string name) {

}

std::string DatabaseAPI::getRemoteServer() {
	return mRemoteServer;
}

std::string DatabaseAPI::getLocalServer() {
	return mLocalServer;
}

DatabaseAPI::DatabaseAPI() {
	init();
}

DatabaseAPI::~DatabaseAPI() {

}