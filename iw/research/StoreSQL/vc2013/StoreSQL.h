#pragma once

#include "sqlite.h"
#include "jsoncpp\json.h"

namespace StoreAPI {
		class StoreSQL : public midnight::sqlite::SQLite {
		public:
			static std::shared_ptr<StoreSQL> make();
			~StoreSQL();
			Json::Value getCategory(std::string name);
			bool setCategory(Json::Value category);
			Json::Value getSubCategory(std::string id);
			bool setSubCategory(Json::Value subcategory);
			Json::Value getCloud(std::string id);
			bool setCloud(Json::Value cloud);
			Json::Value getProduct(std::string name);
			bool setProduct(Json::Value product);
			Json::Value getAllergen(std::string name);
			bool setAllergen(Json::Value allergen);
			Json::Value getPriority(std::string name);
			bool setPriority(Json::Value priority);
			Json::Value getGlobalParameter(std::string key);
			bool setGlobalParameter(Json::Value parameter);
		protected:
			StoreSQL();
	}
};