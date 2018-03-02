#pragma once
#include "StoreSQL.h"

using namespace StoreAPI;

Json::Value StoreSQL::getCategory(std::string name){
};

bool StoreSQL::setCategory(Json::Value category) {
	if (category["type"].asString() == "category") {

	}
	else {
		std::printf("StoreSQL::setCategory -- JSON object not of type 'category'!\n");
		return false;
	}
};

Json::Value StoreSQL::getSubCategory(std::string id){
};

bool StoreSQL::setSubCategory(Json::Value subcategory){
	if (subcategory["type"].asString() == "subcategory") {

	}
	else {
		std::printf("StoreSQL::setSubCategory -- JSON object not of type 'subcategory'!\n");
		return false;
	}
};

Json::Value StoreSQL::getCloud(std::string id){
};

bool StoreSQL::setCloud(Json::Value cloud){
	if (cloud["type"].asString() == "cloud") {

	}
	else {
		std::printf("StoreSQL::setCloud -- JSON object not of type 'cloud'!\n");
		return false;
	}

};

Json::Value StoreSQL::getProduct(std::string name){
};

bool StoreSQL::setProduct(Json::Value product){
		if (product["type"].asString() == "product") {

	}
	else {
		std::printf("StoreSQL::setProduct -- JSON object not of type 'product'!\n");
		return false;
	}

};

Json::Value StoreSQL::getAllergen(std::string name){
};

bool StoreSQL::setAllergen(Json::Value allergen){
	if (allergen["type"].asString() == "allergen") {
		char sqlStatement[256];

		std::sprintf(sqlStatement, "INSERT INTO StoreAPI_Allergen (mName, mDescription)  VALUES (%s, %s)", allergen["name"].asCString(), allergen["description"].asCString());

		query(sqlStatement);
	}
	else {
		std::printf("StoreSQL::setAllergen -- JSON object not of type 'allergen'!\n");
		return false;
	}

};

Json::Value StoreSQL::getPriority(std::string name){
};

bool StoreSQL::setPriority(Json::Value priority){
	if (priority["type"].asString() == "priority") {

	}
	else {
		std::printf("StoreSQL::setPriority -- JSON object not of type 'priority'!\n");
		return false;
	}

};

Json::Value StoreSQL::getGlobalParameter(std::string key){
};

bool StoreSQL::setGlobalParameter(Json::Value parameter){
	if (parameter["type"].asString() == "parameter") {
		char sqlStatement[256];

		std::sprintf(sqlStatement, "INSERT INTO StoreAPI_Parameter (mKey, mValue, mType)  VALUES (%s, %s, %s)", parameter["key"].asCString(), parameter["value"].asCString(), parameter["type"].asCString());

		query(sqlStatement);
	}
	else {
		std::printf("StoreSQL::setParameter -- JSON object not of type 'parameter'!\n");
		return false;
	}

};