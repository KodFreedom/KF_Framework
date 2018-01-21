// Constant table
sampler color_texture = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};
float4 material_diffuse;

struct PixelIn
{
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main(PixelIn pixel) : COLOR0
{
	return tex2D(color_texture, pixel.uv) * pixel.color * material_diffuse;
}