#include "IWInitializer.h"
#include "FarAwayState.h"
#include "ECS.h"
#include "cinder/Rand.h"
#include "IWEvents.h"
#include "ecs/utils/Triangluator.h"

using namespace ci;
using namespace ci::app;

void IWInitializer::load(std::atomic<float>* progress, float progress_step)
{
	//load all rendering considerations, targetsm, materials, and meshes

	//get the default color material
	RenderSystem& render_system = *getScene()->getSystem<RenderSystem>();

	//create product rect
	auto src = geom::Rect( Rectf(vec2(-.5), vec2(.5) )) >>  geom::AttribFn<vec2, vec2>(geom::Attrib::TEX_COORD_0, [](ci::vec2 tc) {
		tc.y = 1. - tc.y;
		return tc;
	});
	auto product_rect = geom::SourceRef( src.clone() );
	render_system.registerMesh("product_rect", product_rect);

	std::shared_ptr<geom::Circle> circle_source = std::shared_ptr<geom::Circle>( new geom::Circle() );
	circle_source->subdivisions(25);
	uint32_t circle_mesh_id = render_system.registerMesh("unit_circle", circle_source );

	// load up TaxonomySystem
	TaxonomySystem& taxonomy_system = *getScene()->getSystem<TaxonomySystem>();

	//Load taxonomy from the database
	mAPI = DatabaseAPI::make();
	mCategoryJson = mAPI->getCategory();

	for (auto& subcategory : mCategoryJson["inventory"]) {
		int subcategoryUUID = atoi(subcategory.asCString());
		Json::Value subcategoryJson = mAPI->getSubCategory(subcategoryUUID);
		mSubCategoryMap[subcategoryUUID] = subcategoryJson;
		for (auto& cloudId : subcategoryJson["clouds"]) {
			int cloudUUID = atoi(cloudId.asCString());
			Json::Value cloudJson = mAPI->getCloud(cloudUUID);
			mCloudMap[cloudUUID] = cloudJson;
			for (auto& productSku : cloudJson["products"]) {
				Json::Value productJson = mAPI->getProduct(productSku.asString());
				mProductMap[productSku.asString()] = productJson;
			}
		}
	}

	uint32_t bg_texture_pool_id = render_system.registerMaterial("BGTexturePoolMaterial", TexturePoolMaterial::create(false));
	TexturePoolMaterialRef bgtexturePool = std::dynamic_pointer_cast<TexturePoolMaterial>(render_system.retrieveMaterial(bg_texture_pool_id));

	ci::Surface far_away_bg = ci::loadImage(ci::app::loadAsset("images/demo/frozenDesserts_s1.jpg"));
	ci::Surface approach_bg = ci::loadImage(ci::app::loadAsset("images/demo/frozendesserts_s2.jpg"));
	ci::Surface engage_bg = ci::loadImage(ci::app::loadAsset("images/demo/frozenDessert_bg.jpg"));
	bgtexturePool->addSurface(std::move(far_away_bg));
	bgtexturePool->addSurface(std::move(approach_bg));
	bgtexturePool->addSurface(std::move(engage_bg));

	uint32_t texture_pool_id = render_system.registerMaterial("ProductTexturePoolMaterial", TexturePoolMaterial::create());
	TexturePoolMaterialRef texturePool = std::dynamic_pointer_cast<TexturePoolMaterial>(render_system.retrieveMaterial(texture_pool_id));

	for (auto& product : mProductMap) {
		ci::Surface productSurface = ci::loadImage(ci::app::loadAsset("images/products/" + product.first + ".png"));
		int index = texturePool->addSurface(std::move(productSurface));
		mProductTextureMap[product.first] = index;
	}

	std::map< int, std::vector<std::pair< Rectf, std::vector<Rectf> > > > initial_data;

	for (auto & subcategory : mSubCategoryMap ) {

		//key
		int uuid = subcategory.first;

		std::vector<std::pair<Rectf, std::vector<Rectf>>> cloud_initial_data;

		for (auto& cloud_index : subcategory.second["clouds"] ) {

			Json::Value& cloud = mCloudMap[atoi(cloud_index.asCString())];

			//size up name for label placement
			std::string cloud_name = cloud["name"].asString();

			Rectf label_area = Rectf( vec2(0), vec2( 120, 60) );

			std::vector<Rectf> cloud_products;

			for (auto& product_index : cloud["products"]) {

				Json::Value& product = mProductMap[product_index.asString()];

				std::string product_name = product["name"].asString();
				std::string product_price = std::to_string(randFloat(1., 10.));
				//auto product_image = url;
				int display_size = product["display_size"].asUInt();
				ci::vec2 product_size;
				switch ((ProductDisplaySize)display_size) {
				case ProductDisplaySize::STANDARD:
					{
						product_size = ci::vec2(80, 100);
					}
					break;
				case ProductDisplaySize::HIGHLIGHT:
					{
						product_size = ci::vec2(110, 150);
					}
					break;
				case ProductDisplaySize::FEATURED:
					{
						product_size = ci::vec2(90, 120);
					}
					break;
				default:
					break;
				}

				//Remove whitespace from image and size up the text to get an accurate height. let the above switch statement dictate the width.

				Rectf area = Rectf(vec2(0), product_size);

				//create rect and add it to cloud
				cloud_products.push_back( area );
			}

			cloud_initial_data.emplace_back(std::move(label_area), std::move(cloud_products));

		}

		initial_data.emplace( uuid, std::move(cloud_initial_data) );
	}

	//generate layouts
	for (auto& layout_data : initial_data ) {
		//TODO set bounds and epicenter
		Layout generator = Layout( getWindowCenter(), Rectf( vec2(getWindowSize())*.03f, vec2(getWindowSize())*.97f ) );
		mLayouts.emplace( layout_data.first, generator.generate( layout_data.second ) );
	}

	mPersonDetector = PersonDetector::create();

	//triangluate clouds and save connection pairs
	for ( auto & layout : mLayouts ) {
		auto uuid = layout.first;
		std::vector<std::pair< std::vector<int>, std::vector<std::pair<int, int>>>> sub_cat_connections;

		for (auto & cloud : layout.second.clouds ) {
			Triangluator triangulator;
			std::vector<vec2> points;

			points.push_back( cloud.label.getCenter() );

			for ( auto & product : cloud.products ) {
				points.push_back( product.placement.getCenter() );
			}
			triangulator.triangulate(points);

			auto & edges = triangulator.getEdges();

			std::vector<std::pair<int, int>> product_connections;

			std::vector<int> label_connections;

			for ( auto & edge : edges ) {
				std::pair<int, int> connection;
				int i = 0;
				bool found = false;
				bool connected_to_label = false;
				int label_connection = 0;
				for ( auto & product : cloud.products ) {
					if (edge.getP1() == product.placement.getCenter())
					{
						label_connection = i;
						connection.first = i;
						found = true;
						break;
					}
					++i;
				}
				if (!found) {
					if (edge.getP1() == cloud.label.getCenter()) {
						connected_to_label = true;
					}
				}
				i = 0;
				found = false;
				for (auto & product : cloud.products) {
					if (edge.getP2() == product.placement.getCenter())
					{
						connection.second = i;
						label_connection = i;
						found = true;
						break;
					}
					if (!found) {
						if (edge.getP2() == cloud.label.getCenter()) {
							connected_to_label = true;
						}
					}
					++i;
				}
				if(!connected_to_label)
					product_connections.emplace_back(std::move(connection));
				else
				{
					label_connections.push_back(label_connection);
				}
			}

			sub_cat_connections.emplace_back( std::move(label_connections), std::move(product_connections) );
		}
		mLayoutSpringConnections.emplace( uuid, std::move( sub_cat_connections ) );
	}

	//create background button
	//clicking the background button triggers a ClickOutEvent
	Handle bg_handle = getScene()->newEntity("background");

	Entity& bg_rect = *bg_handle.get_fast<Entity>();

	bg_rect.setPosition(ci::vec3(ci::app::getWindowCenter(), 0));
	bg_rect.setScale(ci::vec3(ci::app::getWindowSize(), 0));

	Handle ui_handle = bg_rect.createComponent<UIComponent>(Rectf( vec2(-.5), vec2(.5)), -1, InputType::CLICK);

	ui_handle.get<UIComponent>()->connectHandler(InputType::CLICK, InputAction::TRIGGER, [](const InputSample& sample) {
		auto eventManager = Engine::get()->scene()->getEventManager();
		eventManager->queueEvent(ClickOutEvent::create());
	});

	mBackgroundButton = bg_handle;

	//@Nathan
	// use a lambda like this for the zed callbacks
	// auto callback = [](){
	//	auto eventManager = Engine::get()->scene()->getEventManager();
	//	eventManager->queueEvent( PresenceDetectionEvent::create( true or false ) );
	// }
	//this way, the scene is accessed statically and no scope is captured in the lambda, and each state's handler implements different logic on what to do when they get that event. 
}

ISceneLogicStateRef IWInitializer::beginLogic()
{
	return FarAwayState::create(getScene());
}