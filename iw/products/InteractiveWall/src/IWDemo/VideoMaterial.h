#pragma once

#include "ciWMFVideoPlayer.h"
#include "ecs/rendering/materials/Material.hpp"

class VideoMaterialBind : public IMaterialBind
{
public:

	//TODO video options struct

	VideoMaterialBind(const std::string& path_to_video) :mVideoPath(path_to_video) {}

	MATERIAL_SETUP_H(VideoMaterial);

	void play();
	void pause();
	void setAlpha(float a) { mAlpha = a; }

	void init()override;
	void bind()override;
	void unbind()override;
	const SortOrder getSortOrder() const { return  mAlpha < 1.f ? SortOrder::BACK_TO_FRONT : SortOrder::FRONT_TO_BACK; }

	const DrawLayer getLayer() const { return mAlpha < 1.f ? DrawLayer::TRANSPARENT_LAYER : DrawLayer::OPAQUE_LAYER; };
	const int getSubLayer() const { return 0; };

	void loadVideo(const std::string& path);

private:

	ciWMFVideoPlayer mVideoPlayer;
	std::string mVideoPath;
	float mAlpha{ 1.f };
};

using VideoMaterial = BasicMaterial< VideoMaterialBind >;
using VideoMaterialRef = std::shared_ptr<VideoMaterial>;