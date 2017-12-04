// Constant table
sampler diffuse_texture;

struct PixelIn
{
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main(PixelIn pixel) : COLOR0
{
	return float4(1,1,1,1);
}