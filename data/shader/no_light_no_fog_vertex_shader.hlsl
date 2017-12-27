// Constant Table
float4x4 world_view_projection;

struct VertexIn
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

struct VertexOut
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

VertexOut main(VertexIn vertex)
{
	VertexOut result;
	result.position = mul(float4(vertex.position, 1.0f), world_view_projection);
	result.uv = vertex.uv;
	result.color = vertex.color;
	return result;
}