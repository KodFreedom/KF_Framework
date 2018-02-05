//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
// Material
sampler color_texture;
sampler normal_texture = sampler_state
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
};
sampler specular_texture;

// shadowmap
float bias;
float light_far;
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
	float4 position_light : TEXCOORD1;
	float3 position_to_camera_tangent : TEXCOORD2;
	float3 light_direction_tangent : TEXCOORD3;
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
	// material color
	pixel.position_to_camera_tangent = normalize(pixel.position_to_camera_tangent);
	pixel.light_direction_tangent = normalize(pixel.light_direction_tangent);
	float3 normal_tangent = tex2D(normal_texture, pixel.uv).rgb * 2.0f - 1.0f;
	float3 reflect_light_direction_tangent = reflect(pixel.light_direction_tangent, normal_tangent);
	float half_lambert = dot(normal_tangent, -pixel.light_direction_tangent) * 0.5f + 0.5f;

	// color
	float3 color = tex2D(color_texture, pixel.uv).rgb * half_lambert;

	// specular
	float brightness = pow(max(dot(reflect_light_direction_tangent, pixel.position_to_camera_tangent), 0.0f), 100.0f);
	float3 specular_color = tex2D(specular_texture, pixel.uv).rgb;
	float3 specular = specular_color * brightness;

	// ShadowMap(0.0f - 1.0f)
	float shadow_map_multiplier = computeShadow(pixel) * -0.5f + 1.0f;

	return float4((color + specular) * shadow_map_multiplier, 1.0f);
}