// Constant table
sampler color_texture = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// shadowmap
float4 shadow_map_offset;
sampler shadow_map = sampler_state
{
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct PixelIn
{
	float3 normal_world : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 position_light : TEXCOORD1;
};

float4 main(PixelIn pixel) : COLOR0
{
	// TODO : 後でテクスチャから色を算出する
	float4 color = tex2D(color_texture, pixel.uv);

	// shadowmapから値を取得
	float2 shadow_map_uv = 0.5f * pixel.position_light.xy / pixel.position_light.w + float2(0.5f, 0.5f);
	shadow_map_uv.y = 1.0f - shadow_map_uv.y;
	shadow_map_uv.x += shadow_map_offset.x;
	shadow_map_uv.y += shadow_map_offset.y;

	// 光源から頂点までの距離を計算
	float depth = pixel.position_light.z / pixel.position_light.w;

	// シャドウマップの深度値と比較
	if (depth - tex2D(shadow_map, shadow_map_uv).x > 0.00065f)
	{
		color.xyz *= 0.5f;
	}

	return color;
}