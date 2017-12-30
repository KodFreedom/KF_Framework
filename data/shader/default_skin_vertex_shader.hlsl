//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
float4x4 view;
float4x4 projection;
float4x4 view_light;
float4x4 projection_light;
float3 light_direction_world;
float3 camera_position_world;

sampler bone_texture = sampler_state
{
	MipFilter = NONE;
	MagFilter = POINT;
	MinFilter = POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};
float texture_size;

//--------------------------------------------------------------------------------
//  In/Out struct
//--------------------------------------------------------------------------------
struct VertexIn
{
	float3 position_local : POSITION0;
	float3 normal_local : NORMAL0;
	float3 tangent_local : TANGENT0;
	float3 binormal_local : BINORMAL0;
	float2 uv : TEXCOORD0;
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
	float2 uv : TEXCOORD0;
	float4 position_light : TEXCOORD1;
	float3 position_to_camera_tangent : TEXCOORD2;
	float3 light_direction_tangent : TEXCOORD3;
};

//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
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
	VertexOut result = (VertexOut)0;
	result.uv = vertex.uv;

	// compute world matrix
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
	float4x4 world_view_projection = mul(mul(bone_world, view), projection);
	float4x4 world_transpose = transpose(bone_world);

	// position
	float3 position_world = mul(float4(vertex.position_local, 1.0f), bone_world).xyz;
	result.position = mul(float4(vertex.position_local, 1.0f), world_view_projection);

	// tangent matrix
	float3x3 tangent_matrix;
	tangent_matrix[0] = vertex.tangent_local;
	tangent_matrix[1] = vertex.binormal_local;
	tangent_matrix[2] = vertex.normal_local;
	float3x3 local_to_tangent = transpose(tangent_matrix);

	// camera
	float3 position_to_camera_world = camera_position_world - position_world;
	position_to_camera_world = normalize(position_to_camera_world);
	float4 position_to_camera_local = mul(float4(position_to_camera_world, 0.0f), world_transpose);
	result.position_to_camera_tangent = mul(position_to_camera_local, local_to_tangent).xyz;
	result.position_to_camera_tangent = normalize(result.position_to_camera_tangent);

	// light
	float4 light_direction_local = mul(float4(light_direction_world, 0.0f), world_transpose);
	result.light_direction_tangent = mul(light_direction_local, local_to_tangent).xyz;
	result.light_direction_tangent = normalize(result.light_direction_tangent);

	// ShadowMap
	float4x4 world_view_projection_light = mul(mul(bone_world, view_light), projection_light);
	result.position_light = mul(float4(vertex.position_local, 1.0f), world_view_projection_light);

	return result;
}