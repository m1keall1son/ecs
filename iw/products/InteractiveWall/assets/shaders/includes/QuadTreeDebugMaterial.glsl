
struct QuadTreeNode
{
	vec4 area;
	vec4 quantity;
};

layout( std430, binding=QuadTreeDebugMaterial_binding ) buffer quad_tree_debug_material_instance_buffer
{
	QuadTreeNode quadTreeDebugMaterials[];
};

