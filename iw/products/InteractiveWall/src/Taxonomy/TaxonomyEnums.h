#pragma once

enum ProductDisplaySize { 
	STANDARD, 
	FEATURED, 
	HIGHLIGHT
};

enum ModelType {
	CATEGORY,
	SUBCATEGORY,
	CLOUD,
	PRODUCT,
	ALLERGEN,
	PARAMETER
};

enum ParameterType {
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING
};