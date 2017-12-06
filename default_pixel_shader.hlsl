// Constant table
float3 camera_position_local;
float3 light_direction_local;
float4 light_diffuse;
float4 light_ambient;

float4 material_diffuse;
float4 material_ambient;
float4 material_emissive;
float4 material_specular;
float  material_power;
sampler diffuse_texture;
sampler normal_texture;
sampler specular_texture;

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
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float3 normal_local : TEXCOORD1;
	float3 position_local : TEXCOORD2;
	float4 position_light : TEXCOORD3;
};

float4 main(PixelIn pixel) : COLOR0
{
	//pixel.normal_local = normalize(pixel.normal_local);
	//float3 position_to_camera = normalize(camera_position_local - pixel.position_local);
	//float3 reflect_light_direction = reflect(light_direction_local, pixel.normal_local);
	//float3 specular = material_specular.rgb * pow(max(dot(reflect_light_direction, position_to_camera), 0.0f), material_power); // material * lightcolor
	//float3 diffuse = material_diffuse.rgb * ((dot(pixel.normal_local, - light_direction_local) + 1.0f) * 0.5f) * light_diffuse.rgb; // material * lightcolor
	//float3 ambient = material_ambient.rgb * light_ambient.rgb; // material * lightcolor
	//float4 color = float4(diffuse + ambient + specular + material_emissive.rgb, 1.0f);

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
		pixel.color.xyz *= 0.5f;
	}

	return /*color * */tex2D(diffuse_texture, pixel.uv) * pixel.color;
}