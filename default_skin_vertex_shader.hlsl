// Constant Table
float4x4 world_view_projection;
sampler bone_texture;

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

float4x4 getBoneMatrixBy(const int index)
{
	float2 uv;
	uv.x = 1.0f / 256.0f;
	uv.y = 1.0f / 256.0f * index;
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
	//float2 uv;
	//float texwidth = 256.0f;
	//uv.x = 1.0f / 256.0f;
	//uv.y = 1.0f / 256.0f;
	//float4x4 mtx;
	//
	//float fTexU = ((index * 4) % texwidth * uv.x);
	//
	//mtx._11 = tex2Dlod(bone_texture, float4(fTexU, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).x;
	//mtx._12 = tex2Dlod(bone_texture, float4(fTexU, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).y;
	//mtx._13 = tex2Dlod(bone_texture, float4(fTexU, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).z;
	//mtx._14 = tex2Dlod(bone_texture, float4(fTexU, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).w;
	//
	//mtx._21 = tex2Dlod(bone_texture, float4(fTexU + uv.x, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).x;
	//mtx._22 = tex2Dlod(bone_texture, float4(fTexU + uv.x, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).y;
	//mtx._23 = tex2Dlod(bone_texture, float4(fTexU + uv.x, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).z;
	//mtx._24 = tex2Dlod(bone_texture, float4(fTexU + uv.x, ((index * 4) / texwidth * uv.y), 0.0, 0.0)).w;
	//
	//mtx._31 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 2), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).x;
	//mtx._32 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 2), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).y;
	//mtx._33 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 2), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).z;
	//mtx._34 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 2), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).w;
	//
	//mtx._41 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 3), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).x;
	//mtx._42 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 3), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).y;
	//mtx._43 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 3), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).z;
	//mtx._44 = tex2Dlod(bone_texture, float4(fTexU + (uv.x * 3), ((index * 4) / texwidth * uv.y), 0.0, 0.0)).w;
	//
	//return mtx;
}

VertexOut main(VertexIn vertex)
{
	VertexOut result;
	//float4x4 bone_world
	//	= getBoneMatrixBy((int)vertex.bone_indexes.x) * vertex.bone_weights.x
	//	+ getBoneMatrixBy((int)vertex.bone_indexes.y) * vertex.bone_weights.y
	//	+ getBoneMatrixBy((int)vertex.bone_indexes.z) * vertex.bone_weights.z
	//	+ getBoneMatrixBy((int)vertex.bone_indexes.w) * vertex.bone_weights.w;
	//result.position = mul(float4(vertex.position, 1.0f), bone_world);
	result.position = mul(float4(vertex.position, 1.0f), world_view_projection);
	result.uv = vertex.uv;
	result.color = vertex.color;
	return result;
}

