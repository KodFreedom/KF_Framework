struct PixelIn
{
	float4 depth : TEXCOORD0;
};

float4 main(PixelIn pixel) : COLOR0
{
	float4 color = pixel.depth.z / pixel.depth.w;
	return color;
}