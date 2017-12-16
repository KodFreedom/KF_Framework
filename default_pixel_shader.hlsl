// Constant table
float3 camera_position_local;
float3 light_direction_local;
float4 light_diffuse;
float4 light_ambient;

// material
float4 material_diffuse;
float4 material_ambient;
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
	// material color
	pixel.normal_local = normalize(pixel.normal_local);
	float3 position_to_camera = normalize(camera_position_local - pixel.position_local);
	float3 reflect_light_direction = reflect(light_direction_local, pixel.normal_local);
	float3 specular = material_specular.rgb * pow(max(dot(reflect_light_direction, position_to_camera), 0.0f), material_power); // material * lightcolor
	float3 diffuse = material_diffuse.rgb * ((dot(pixel.normal_local, - light_direction_local) + 1.0f) * 0.5f) * light_diffuse.rgb; // material * lightcolor
	float3 ambient = material_ambient.rgb * light_ambient.rgb; // material * lightcolor
	float4 color = float4(diffuse + ambient + specular + material_emissive.rgb, 1.0f);

	// shadowmap����l���擾
	float2 shadow_map_uv = 0.5f * pixel.position_light.xy / pixel.position_light.w + float2(0.5f, 0.5f);
	shadow_map_uv.y = 1.0f - shadow_map_uv.y;
	shadow_map_uv.x += shadow_map_offset.x;
	shadow_map_uv.y += shadow_map_offset.y;

	// �������璸�_�܂ł̋������v�Z
	float depth = pixel.position_light.z / pixel.position_light.w;

	// �V���h�E�}�b�v�̐[�x�l�Ɣ�r
	if (depth - tex2D(shadow_map, shadow_map_uv).x > 0.00065f)
	{
		pixel.color.xyz *= 0.5f;
	}

	return color * tex2D(color_texture, pixel.uv) * pixel.color;
}