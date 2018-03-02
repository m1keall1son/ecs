#include "StartUpInit.h"
#include "cinder/app/App.h"
#include "LoadState.h"
#include "ECS.h"

using namespace ci;
using namespace ci::app;

void StartUpInit::load(std::atomic<float>* progress, float progress_step) {
	RenderSystem& render_system = *getScene()->getSystem<RenderSystem>();
	uint32_t color_material_id = render_system.getMaterialId("ColorMaterial");

	mLoadingProgressBar = getScene()->newEntity("progress bar");

	Entity & progress_bar = *mLoadingProgressBar.get<Entity>();
	progress_bar.setPosition(vec3(getWindowCenter() - vec2(250, 0), 0));

	vec3 size = vec3(0, 10, 1);
	progress_bar.setScale(size);
	progress_bar.setAnchor(vec3(-.5, -.5, 0));

	uint32_t mesh_id = render_system.getMeshId("unit_rect");
	progress_bar.createComponent<RenderComponent>(RenderTechnique(mesh_id).pass<ColorMaterial>(render_system.getMainRenderTarget(), color_material_id, vec4(1.0f)));

	mLoadingText = getScene()->newEntity("loading...");

	TextSystem& text_system = *getScene()->getSystem<TextSystem>();
	uint32_t font_id = text_system.getFontManager().getIndex("OpenSans-Regular-60");

	//Add another entity that has some text
	auto text_box = mLoadingText.get<Entity>();
	text_box->createComponent<TextComponent>("Loading", font_id, ColorAf(0., 0., 0., 1.), vec2(0), SdfText::DrawOptions().alignment(SdfText::Alignment::CENTER).leading(-10));
	text_box->setPosition(vec3(getWindowCenter(), 0) + vec3(-250, -50, 1));
	text_box->setRotation(toQuat(ci::rotate(float(M_PI), vec3(1, 0, 0))));

	//Make initial API call from async curl
	//when it returns load the database and then call...
	mAPI = DatabaseAPI::make();
	//mAPI->syncCategory();
}

ISceneLogicStateRef StartUpInit::beginLogic() {
	return LoadState::create(getScene());
}