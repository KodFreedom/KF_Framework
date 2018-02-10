// constant table
float4x4 view_light;
float4x4 projection_light;
sampler bone_texture;
float texture_size;

struct VertexIn
{
	float3 position_local : POSITION0;
	uint2  bone_indexes_0 : BLENDINDICES0;
	uint2  bone_indexes_1 : BLENDINDICES1;
	uint2  bone_indexes_2 : BLENDINDICES2;
	uint2  bone_indexes_3 : BLENDINDICES3;
	uint2  bone_indexes_4 : BLENDINDICES4;
	float2 bone_weights_0 : BLENDWEIGHT0;
	float2 bone_weights_1 : BLENDWEIGHT1;
	float2 bone_weights_2 : BLENDWEIGHT2;
	float2 bone_weights_3 : BLENDWEIGHT3;
	float2 bone_weights_4 : BLENDWEIGHT4;
};

struct VertexOut
{
	float4 position : POSITION0;
	float4 depth : TEXCOORD0;
};

float4x4 GetBoneMatrixBy(const uint index)
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

	// �s��̎Z�o
	float4x4 bone_world = (float4x4)0;
	for (int count = 0; count < 2; ++count)
	{
		bone_world +=
			GetBoneMatrixBy(vertex.bone_indexes_0[count]) * vertex.bone_weights_0[count] +
			GetBoneMatrixBy(vertex.bone_indexes_1[count]) * vertex.bone_weights_1[count] +
			GetBoneMatrixBy(vertex.bone_indexes_2[count]) * vertex.bone_weights_2[count] +
			GetBoneMatrixBy(vertex.bone_indexes_3[count]) * vertex.bone_weights_3[count] +
			GetBoneMatrixBy(vertex.bone_indexes_4[count]) * vertex.bone_weights_4[count];
	}
	float4x4 world_view_projection_light = mul(mul(bone_world, view_light), projection_light);

	result.position = mul(float4(vertex.position_local, 1.0f), world_view_projection_light);
	result.depth = result.position;
	return result;
}