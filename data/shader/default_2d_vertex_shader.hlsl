//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
float4x4 world_projection;

//--------------------------------------------------------------------------------
//  In/Out struct
//--------------------------------------------------------------------------------
struct VertexIn
{
	float3 position : POSITION0;
	float4 color : COLOR0;
};

struct VertexOut
{
	float4 position : POSITION0;
	float4 color : COLOR0;
};


//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
VertexOut main(VertexIn vertex)
{
	VertexOut result = (VertexOut)0;
	result.position = mul(float4(vertex.position, 1.0f), world_projection);
	result.color = vertex.color;
	return result;
}