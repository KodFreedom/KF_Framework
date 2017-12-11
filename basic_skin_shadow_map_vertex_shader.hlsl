// constant table
float4x4 view_light;
float4x4 projection_light;
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
	float3 position_local : POSITION0;
	float3 bone_indexes_02 : TEXCOORD1;
	float3 bone_indexes_35 : TEXCOORD2;
	float3 bone_indexes_68 : TEXCOORD3;
	float3 bone_weights_02 : TEXCOORD4;
	float3 bone_weights_35 : TEXCOORD5;
	float3 bone_weights_68 : TEXCOORD6;
};

struct VertexOut
{
	float4 position : POSITION0;
	float4 depth : TEXCOORD0;
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

	// s—ñ‚ÌŽZo
	float4x4 bone_world = (float4x4)0;
	for (int count = 0; count < 3; ++count)
	{
		bone_world +=
			GetBoneMatrixBy((int)vertex.bone_indexes_02[count]) * vertex.bone_weights_02[count] +
			GetBoneMatrixBy((int)vertex.bone_indexes_35[count]) * vertex.bone_weights_35[count] +
			GetBoneMatrixBy((int)vertex.bone_indexes_68[count]) * vertex.bone_weights_68[count];
	}
	float4x4 world_view_projection_light = mul(mul(bone_world, view_light), projection_light);

	result.position = mul(float4(vertex.position_local, 1.0f), world_view_projection_light);
	result.depth = result.position;
	return result;
}