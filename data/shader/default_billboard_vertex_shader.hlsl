// Constant Table
float4x4 world_view_projection;
float3	 scale;

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

VertexOut main(VertexIn vertex)
{
	VertexOut result;
	float3 scaled_position = vertex.position * scale;
	result.position = mul(float4(scaled_position, 1.0f), world_view_projection);
	result.uv = vertex.uv;
	result.color = vertex.color;
	return result;
}