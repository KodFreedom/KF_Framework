//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
float3 camera_position_local;
float3 light_direction_local;
float4 light_diffuse;

// material
float4 material_diffuse;
float4 material_emissive;
float4 material_specular;
float  material_power;
sampler color_texture = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// shadowmap
float bias;
sampler shadow_map = sampler_state
{
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = BORDER;
	AddressV = BORDER;
};

//--------------------------------------------------------------------------------
//  In/Out struct
//--------------------------------------------------------------------------------
struct PixelIn
{
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float3 normal_local : TEXCOORD1;
	float3 position_local : TEXCOORD2;
	float4 position_light : TEXCOORD3;
};

//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
float computeShadow(PixelIn pixel)
{
	// Calculate the projected texture coordinates.
	float2 project_uv;
    project_uv.x = pixel.position_light.x / pixel.position_light.w * 0.5f + 0.5f;
    project_uv.y = -pixel.position_light.y / pixel.position_light.w * 0.5f + 0.5f;

	// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
    float depth = tex2D(shadow_map, project_uv).r;

	// Calculate the depth of the light.
    float light_depth = pixel.position_light.z / pixel.position_light.w;

	// Subtract the bias from the lightDepthValue.
    light_depth -= bias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
	// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
	float shadow = step(depth, light_depth);

	// Determine if the projected coordinates are in the 0 to 1 range
	// If so then this pixel is in the view of the light.
	shadow *= (1.0f - (float)any(saturate(project_uv) - project_uv));

	return shadow;
}

float4 main(PixelIn pixel) : COLOR0
{
	pixel.normal_local = normalize(pixel.normal_local);

	// Calculate the amount of light on this pixel.
	float light_intensity = dot(pixel.normal_local, - light_direction_local);

	// material color
	float3 position_to_camera = normalize(camera_position_local - pixel.position_local);
	float3 reflect_light_direction = reflect(light_direction_local, pixel.normal_local);
	float3 specular = material_specular.rgb * pow(max(dot(reflect_light_direction, position_to_camera), 0.0f), material_power);
	float3 diffuse = material_diffuse.rgb * ((light_intensity + 1.0f) * 0.5f) * light_diffuse.rgb;
	float4 material_color = float4(diffuse + specular + material_emissive.rgb, 1.0f);

	// テクスチャカラーとマテリアルカラー
	float4 color = material_color * tex2D(color_texture, pixel.uv) * pixel.color;

	// ShadowMap(0.0f - 1.0f)
	float shadow_map_multiplier = computeShadow(pixel) * -0.5f + 1.0f;

	return float4(color.rgb * shadow_map_multiplier, color.a);
}