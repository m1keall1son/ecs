#include "DjangoDatabase.h"

using namespace midnight::http;

std::shared_ptr<DjangoDatabase> DjangoDatabase::make(std::string root_url) {
	return std::shared_ptr<DjangoDatabase>(new DjangoDatabase(root_url));
}

DjangoDatabase::DjangoDatabase(std::string root_url) {
	mRootUrl = root_url;

	//check for trailing backslash
	if (mRootUrl.at(mRootUrl.length() - 1) != '/') {
		mRootUrl += "/";
	}

	mClient = HttpClient::make();
}

DjangoDatabase::~DjangoDatabase() {

}

void DjangoDatabase::setCategory(const Json::Value& category) {
	std::string json = mJsonWriter.write(category);

	HttpRequest Post;
	Post.mUrl = mRootUrl + "categories/";
	Post.mMethod = HTTP_POST;
	Post.mRequestBody = json;
	Post.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("POST request complete with code %d\n", response->mResponseCode); };

	mClient->addHttpRequest(Post);
}

void DjangoDatabase::getCategory() {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "categories/";
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all categories: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}


void DjangoDatabase::setSubCategory(const Json::Value& subcategory) {
	std::string json = mJsonWriter.write(subcategory);

	HttpRequest Post;
	Post.mUrl = mRootUrl + "subcategories/";
	Post.mMethod = HTTP_POST;
	Post.mRequestBody = json;
	Post.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("POST request complete with code %d\n", response->mResponseCode); };

	mClient->addHttpRequest(Post);

}

void DjangoDatabase::getSubCategories() {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "subcategories/";
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all subcategories: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::getSubCategory(const std::string& uuid) {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "subcategories/" + uuid;
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all subcategories: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::setCloud(const Json::Value& cloud) {
	std::string json = mJsonWriter.write(cloud);

	HttpRequest Post;
	Post.mUrl = mRootUrl + "clouds/";
	Post.mMethod = HTTP_POST;
	Post.mRequestBody = json;
	Post.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("POST request complete with code %d\n", response->mResponseCode); };

	mClient->addHttpRequest(Post);
}

void DjangoDatabase::getClouds() {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "clouds/";
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all clouds: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::getCloud(const std::string& uuid) {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "clouds/" + uuid;
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all clouds: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::setProduct(const Json::Value& product) {
	std::string json = mJsonWriter.write(product);

	HttpRequest Post;
	Post.mUrl = mRootUrl + "products/";
	Post.mMethod = HTTP_POST;
	Post.mRequestBody = json;
	Post.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("POST request complete with code %d\n", response->mResponseCode); };

	mClient->addHttpRequest(Post);
}

void DjangoDatabase::getProducts() {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "products/";
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all products: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::getProduct(const std::string& sku) {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "products/" + sku;
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all products: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::setAllergen(const Json::Value& allergen) {
	std::string json = mJsonWriter.write(allergen);

	HttpRequest Post;
	Post.mUrl = mRootUrl + "allergens/";
	Post.mMethod = HTTP_POST;
	Post.mRequestBody = json;
	Post.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("POST request complete with code %d\n", response->mResponseCode); };

	mClient->addHttpRequest(Post);
}

void DjangoDatabase::getAllergens() {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "allergens/";
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all allergens: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::getAllergen(const std::string& name) {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "allergens/" + name;
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all allergens: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::setParameter(const Json::Value& parameter) {
	std::string json = mJsonWriter.write(parameter);

	HttpRequest Post;
	Post.mUrl = mRootUrl + "parameters/";
	Post.mMethod = HTTP_POST;
	Post.mRequestBody = json;
	Post.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("POST request complete with code %d\n", response->mResponseCode); };

	mClient->addHttpRequest(Post);
}

void DjangoDatabase::getParameters() {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "parameters/";
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all parameters: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}

void DjangoDatabase::getParameter(const std::string& key) {
	HttpRequest Get;
	Get.mUrl = mRootUrl + "parameters/" + key;
	Get.mMethod = HTTP_GET;
	Get.mCallback = [=](HttpResponse* response, HttpClient* client) { std::printf("Request complete with code %d\n Here is the list of all parameters: %s\n", response->mResponseCode, mClient->jsonToString(response->mBody).c_str()); };

	mClient->addHttpRequest(Get);
}