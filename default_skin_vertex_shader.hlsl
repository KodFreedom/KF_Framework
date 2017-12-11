// Constant Table
float4x4 world_view_projection;
sampler bone_texture = sampler_state 
{
	MipFilter = NONE;
	MagFilter = POINT;
	MinFilter = POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};
float texture_size;

struct VertexIn
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 bone_indexes : TEXCOORD1;
	float4 bone_weights : TEXCOORD2;
};

struct VertexOut
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

float4x4 GetBoneMatrixBy(const int index)
{
	float2 uv;
	uv.x = 1.0f / texture_size;
	uv.y = 1.0f / texture_size * index;
	float4x4 result;
	for (int count_y = 0; count_y < 4; ++count_y)
	{
		float4 row = tex2Dlod(bone_texture, float4(uv.x * count_y, uv.y, 0.0f, 0.0f));
		result[count_y][0] = row.x;
		result[count_y][1] = row.y;
		result[count_y][2] = row.z;
		result[count_y][3] = row.w;
	}
	return result;
}

VertexOut main(VertexIn vertex)
{
	VertexOut result;
	float4x4 bone_world
		= GetBoneMatrixBy((int)vertex.bone_indexes.x) * vertex.bone_weights.x
		+ GetBoneMatrixBy((int)vertex.bone_indexes.y) * vertex.bone_weights.y
		+ GetBoneMatrixBy((int)vertex.bone_indexes.z) * vertex.bone_weights.z
		+ GetBoneMatrixBy((int)vertex.bone_indexes.w) * vertex.bone_weights.w;
	result.position = mul(float4(vertex.position, 1.0f), bone_world);
	result.position = mul(result.position, world_view_projection);
	result.uv = vertex.uv;
	result.color = vertex.color;
	return result;
}