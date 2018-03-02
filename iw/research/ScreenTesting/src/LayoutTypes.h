#pragma once
#include "cinder/Rect.h"
#include <vector>

namespace Layout {

	struct Product {
		ci::Rectf placement;
	};

	struct Cloud {
		ci::Rectf label;
		ci::Rectf placement;
		std::vector<Product> products;
	};

	struct Subcategory {
		ci::Rectf label;
		ci::Rectf placement;
		std::vector<Cloud> clouds;
		void clear() { clouds.clear(); label = ci::Rectf(); placement = ci::Rectf(); }
	};

}
