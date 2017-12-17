//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
float3 camera_position_world;
float3 light_direction_world;

// Material
sampler color_texture;
sampler diffuse_texture;
sampler diffuse_texture_mask;
sampler specular_texture;
sampler specular_texture_mask;

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
	float3 normal_world : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 position_light : TEXCOORD1;
	float3 position_world : TEXCOORD2;
};

//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
float4 main(PixelIn pixel) : COLOR0
{
	// 色を算出する
	pixel.normal_world = normalize(pixel.normal_world);
	float3 position_to_camera = normalize(camera_position_world - pixel.position_world.xyz);
	float3 reflect_light_direction = reflect(light_direction_world, pixel.normal_world);
	float light_to_camera = max(dot(reflect_light_direction, position_to_camera), 0.0f);
	float half_lambert = dot(pixel.normal_world, -light_direction_world) * 0.5f + 0.5f;

	// diffuse
	float4 diffuse_color = tex2D(diffuse_texture, float2(half_lambert, light_to_camera));
	float4 diffuse_mask = tex2D(diffuse_texture_mask, pixel.uv);
	diffuse_color = diffuse_color * diffuse_mask;

	// specular
	float4 specular_color = light_to_camera * tex2D(specular_texture, pixel.uv);
	float4 specular_mask = tex2D(specular_texture_mask, pixel.uv);
	specular_color = specular_color * specular_mask;

	// total color
	float4 color = tex2D(color_texture, pixel.uv) + (diffuse_color + specular_color);

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

	return float4(color.rgb * shadow_map_multiplier, color.a);
}