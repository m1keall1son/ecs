#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/Rand.h"

#include "ECS.h"

using namespace ci;
using namespace ci::app;
using namespace std;

///////////////////////////////////////////////////////////
//Example Scene Initializer////////////////////////////////
///////////////////////////////////////////////////////////

class ExampleInitializer : public ISceneInitializer
{
public:

	static ISceneInitializerRef create(Scene* context) { return std::unique_ptr<ExampleInitializer>(new ExampleInitializer(context)); }

	//Async scene loading
	void load(std::atomic<float>* progress, float progress_step)override;

	//creates the first scene logic
	ISceneLogicStateRef beginLogic()override;

	Handle& getRoot() { return mSceneRoot; }

private:

	Handle mSceneRoot;

	ExampleInitializer(Scene* context) :ISceneInitializer(context) {}

};


///////////////////////////////////////////////////////////
//Example Scene Logic State////////////////////////////////
///////////////////////////////////////////////////////////

class ExampleState : public ISceneLogicState
{
public:

	static ISceneLogicStateRef create(Scene* context) { return std::unique_ptr<ExampleState>(new ExampleState(context)); }

	void update()override;

	void enter()override;

	void exit()override;

	ISceneLogicStateRef nextLogicState()override;

private:

	void onKeyPress(ci::app::KeyEvent event);

	ExampleState(Scene* context) : ISceneLogicState(context) {}

	ci::signals::Connection mKeyPress;

};

Handle createRandomSphere(Scene* scene, const ci::vec3& color)
{
	auto & render_system = *scene->getSystem<RenderSystem>();
	static int sSphereCounter = 0;
	auto sphere_handle = scene->newEntity("Random Sphere " + std::to_string(sSphereCounter++));

	auto size = vec3(randFloat(10, 50));

	auto sphere = sphere_handle.get<Entity>();
	sphere->setScale(size);
	sphere->setPosition(randVec3() * vec3(ci::app::getWindowSize(), randFloat(-200, 200))*.33f);

	auto lambert_material_id = render_system.getMaterialId("LambertMaterial");
	uint32_t mesh_id = render_system.getMeshId("unit_cube");

	sphere->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<LambertMaterial>(render_system.getMainRenderTarget(), lambert_material_id, vec4(color, 1.0f)));

	auto animation_handle = sphere->createComponent<AnimationComponent>();
	auto animation = animation_handle.get<AnimationComponent>();
	animation->translateBy(randVec3()*100.f, 2.f, Animation::Format().pingPong().easeFn(ci::EaseInOutQuad()));

	auto ui_handle = sphere->createComponent<UIComponent>(mesh_id, 0, InputType::CLICK);

	auto ui = ui_handle.get<UIComponent>();
	ui->connectClickHandler([sphere_handle](const InputSample& sample) mutable -> void {
		sphere_handle.destroy();
	});

	return sphere_handle;
}

////////////////////////////////////////////////////////////////
//Scene Initializer Impl////////////////////////////////////////
////////////////////////////////////////////////////////////////

void ExampleInitializer::load(std::atomic<float>* progress, float progress_step)
{
	//Load persistent scene data here.

	auto & render_system = *getScene()->getSystem<RenderSystem>();

	auto color_material_id = render_system.getMaterialId("ColorMaterial");
	auto lambert_material = render_system.getMaterialId("LambertMaterial");

	render_system.registerMesh("unit_sphere", geom::SourceRef(new geom::Sphere()));
	render_system.registerMesh("unit_cone", geom::SourceRef(new geom::Cone()));
	render_system.registerMesh("unit_cube", geom::SourceRef(new geom::Cube()));

	mSceneRoot = getScene()->newEntity("root");
	Entity & root = *mSceneRoot.get<Entity>();
	root.setPosition(vec3(getWindowCenter(), 0));

	auto button_handle = getScene()->newEntity("Button");
	auto button = button_handle.get<Entity>(); //get object

	auto size = vec3(100, 50, 1);
	button->setScale(size);
	button->setPosition(vec3(20.f + size.x / 2.f, 20.f + size.y / 2.f, 0) - vec3(vec2(getWindowSize()) / 2.f, 0.));

	//build button
	uint32_t mesh_id = render_system.getMeshId("unit_rect");

	auto render_component_handle = button->createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<ColorMaterial>(render_system.getMainRenderTarget(), color_material_id, vec4(1.0f, 0.5f, 0.0f, 1.0f)));

	auto ui_handle = button->createComponent<UIComponent>(mesh_id, 0, InputType::HOLD | InputType::CLICK);

	auto ui = ui_handle.get<UIComponent>();

	ui->connectHandler(InputType::HOLD, InputAction::BEGIN, [render_component_handle, color_material_id](const InputSample& sample) {
		if (auto render_component = render_component_handle.get<RenderComponent>()) {
			if (auto material_instance = render_component->getMaterialInstanceHandle(color_material_id).get<ColorMaterialInstance>()) {
				material_instance->setColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	});

	auto scene_root = mSceneRoot;
	auto scene = getScene();

	ui->connectHandler(InputType::HOLD, InputAction::END, [scene_root, scene, render_component_handle, color_material_id](const InputSample& sample) {
		if (auto render_component = render_component_handle.get<RenderComponent>()) {
			if (auto material_instance = render_component->getMaterialInstanceHandle(color_material_id).get<ColorMaterialInstance>()) {
				material_instance->setColor(vec4(1.0f, 0.5f, 0.0f, 1.0f));
			}
		}
		auto color = vec3(randFloat(.25, 1.), randFloat(.25, 1.), randFloat(.25, 1.));
		for (int i = 0; i < 150; i++) {
			auto sphere_handle = createRandomSphere(scene, color);
			scene_root.get<Entity>()->addChild(sphere_handle);
		}
	});

	auto & text_system = *getScene()->getSystem<TextSystem>();
	auto font_id = text_system.getFontManager().getIndex("Lobster-Regular");

	//Add another entity that has some text
	auto text_box_handle = getScene()->newEntity("button::text");
	auto text_box = text_box_handle.get<Entity>();
	text_box->setPosition(vec3(0, 0, 1));
	text_box->createComponent<TextComponent>("Create Sphere", font_id, ColorAf(.2, .2, .2, 1.), Rectf(vec2(0), vec2(size)), vec2(0), SdfText::DrawOptions().alignment(SdfText::Alignment::CENTER).leading(-10));
	//inverse the size to avoid over scaling
	text_box->setScale(vec3(1) / size);
	text_box->setRotation(toQuat(ci::rotate(float(M_PI), vec3(1, 0, 0))));

	button->addChild(text_box_handle);

	auto & animation_system = *getScene()->getSystem<AnimationSystem>();

	animation_system.newAnimation(2.f, Animation::Format().easeFn(ci::EaseInQuad()).pingPong().updateFn([render_component_handle, color_material_id](float animator) {

		if (auto render_component = render_component_handle.get<RenderComponent>()) {
			if (auto material_instance = render_component->getMaterialInstanceHandle(color_material_id).get<ColorMaterialInstance>()) {
				material_instance->setColor(glm::mix(vec4(1.0f, 0.5f, 0.0f, 1.0f), vec4(0., 0., 1., 1.), animator));
			}
		}

	}));

	mSceneRoot.get<Entity>()->addChild(button_handle);

	for (int i = 0; i < 50; i++) {
		auto sphere_handle = createRandomSphere(getScene(), vec3(1));
		mSceneRoot.get<Entity>()->addChild(sphere_handle);
	}

	progress->store(*progress + progress_step);
}

ISceneLogicStateRef ExampleInitializer::beginLogic()
{
	return ExampleState::create(getScene());
}


///////////////////////////////////////////////////////////////
//Scene Logic State Impl///////////////////////////////////////
///////////////////////////////////////////////////////////////


void ExampleState::onKeyPress(ci::app::KeyEvent event)
{
}

void ExampleState::update()
{
}

void ExampleState::enter()
{
	mKeyPress = getWindow()->getSignalKeyDown().connect(std::bind(&ExampleState::onKeyPress, this, std::placeholders::_1));

}

void ExampleState::exit()
{
	mKeyPress.disconnect();
}

ISceneLogicStateRef ExampleState::nextLogicState()
{
	//advance to next state, determined by the current state.
	//return MyNextState::create();
	return nullptr;
}


class ECSDevApp : public Engine {
public:
	void init()override;
};

void ECSDevApp::init()
{
	//Register scene states and new systems
	auto & factory = getSceneInitializerFactory();
	factory.emplace("ExampleInitializer", ExampleInitializer::create);

	//load config
	auto config = util::parseJson("config/EngineConfig.json");
	loadConfig(config);
}

CINDER_APP(ECSDevApp, RendererGl(RendererGl::Options().version(4, 3)), [](App::Settings* settings) { settings->setFullScreen(); })
