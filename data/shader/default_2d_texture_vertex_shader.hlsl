//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
float4x4 world;
float4x4 projection;
float2 offset;
float2 uv_offset;
float2 uv_scale;

//--------------------------------------------------------------------------------
//  In/Out struct
//--------------------------------------------------------------------------------
struct VertexIn
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

struct VertexOut
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
VertexOut main(VertexIn vertex)
{
	VertexOut result = (VertexOut)0;
	result.position = mul(float4(vertex.position, 1.0f), world);
	result.position = mul(result.position + float4(offset, 0.0f, 0.0f), projection);
	result.uv = uv_offset + vertex.uv * uv_scale;
	result.color = vertex.color;
	return result;
}