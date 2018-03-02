struct TexturePoolData {
	ivec4 texture_id;
	vec4 color;
};

layout( std430, binding=TexturePoolMaterial_binding ) buffer texture_pool_material_buffer {
	TexturePoolData texture_pools[];
};

