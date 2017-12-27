//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
float4x4 world_view_projection_light;

//--------------------------------------------------------------------------------
//  In/Out struct
//--------------------------------------------------------------------------------
struct VertexIn
{
	float3 position : POSITION0;
};

struct VertexOut
{
	float4 position : POSITION0;
	float4 depth : TEXCOORD0;
};

//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
VertexOut main(VertexIn vertex)
{
	VertexOut result = (VertexOut)0;
	result.position = mul(float4(vertex.position, 1.0f), world_view_projection_light);
	result.depth = result.position;
	return result;
}