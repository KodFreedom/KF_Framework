//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
// Material
sampler color_texture;
sampler diffuse_texture;
sampler diffuse_texture_mask;
sampler specular_texture;
sampler specular_texture_mask;
sampler detail_texture;
sampler detail_mask;
sampler tint_by_base_mask;
sampler rim_mask;
sampler translucency;
sampler metalness_mask;
sampler self_illum_mask;
sampler normal_texture = sampler_state
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
};
//sampler fresnel_warp_color;
//sampler fresnel_warp_rim;
//sampler fresnel_warp_specular;

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

//--------------------------------------------------------------------------------
//  In/Out struct
//--------------------------------------------------------------------------------
struct PixelIn
{
	float2 uv : TEXCOORD0;
	float4 position_light : TEXCOORD1;
	float3 position_to_camera_tangent : TEXCOORD2;
	float3 light_direction_tangent : TEXCOORD3;
};

//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
float4 main(PixelIn pixel) : COLOR0
{
	// 色を算出する
	pixel.position_to_camera_tangent = normalize(pixel.position_to_camera_tangent);
	pixel.light_direction_tangent = normalize(pixel.light_direction_tangent);
	float3 normal_tangent = tex2D(normal_texture, pixel.uv).rgb * 2.0f - 1.0f;
	float3 reflect_light_direction_tangent = reflect(pixel.light_direction_tangent, normal_tangent);
	float brightness = max(dot(reflect_light_direction_tangent, pixel.position_to_camera_tangent), 0.0f);
	float half_lambert = dot(normal_tangent, -pixel.light_direction_tangent) * 0.5f + 0.5f;

	// color
	float3 color = tex2D(color_texture, pixel.uv).rgb;

	// diffuse
	float3 diffuse_color = tex2D(diffuse_texture, float2(half_lambert, brightness)).rgb;

	// specular
	float3 specular_color = brightness * tex2D(specular_texture, pixel.uv).rgb;

	// detail
	float3 detail_color = tex2D(detail_texture, pixel.uv).rgb;

	// total color
	float3 final_color = color * (tex2D(tint_by_base_mask, pixel.uv).rgb
		+ tex2D(self_illum_mask, pixel.uv).rgb
		+ diffuse_color * tex2D(diffuse_texture_mask, pixel.uv).rgb
		+ specular_color * tex2D(specular_texture_mask, pixel.uv).rgb * tex2D(metalness_mask, pixel.uv).rgb)
		+ detail_color * tex2D(detail_mask, pixel.uv).rgb;

	// shadowmapから値を取得
	float2 shadow_map_uv = 0.5f * pixel.position_light.xy / pixel.position_light.w + float2(0.5f, 0.5f);
	shadow_map_uv.y = 1.0f - shadow_map_uv.y;
	shadow_map_uv.x += shadow_map_offset.x;
	shadow_map_uv.y += shadow_map_offset.y;

	// 光源から頂点までの距離を計算
	float depth = pixel.position_light.z / pixel.position_light.w;

	// シャドウマップの深度値と比較
	float result = depth - tex2D(shadow_map, shadow_map_uv).x;
	float shadow_map_multiplier = step(0.00065f, result) * -0.5f + 1.0f;

	return float4(final_color * shadow_map_multiplier, tex2D(translucency, pixel.uv).r);
}