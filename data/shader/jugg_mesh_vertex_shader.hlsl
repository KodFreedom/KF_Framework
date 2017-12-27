// Constant Table
float4x4 world_view_projection;
float4x4 world_view_projection_light;

struct VertexIn
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

struct VertexOut
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

VertexOut main(VertexIn vertex)
{
	VertexOut result;
	result.position = mul(float4(vertex.position, 1.0f), world_view_projection);
	result.uv = vertex.uv;
	return result;
}