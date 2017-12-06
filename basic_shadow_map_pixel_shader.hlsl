struct PixelIn
{
	float4 depth : TEXCOORD0;
};

float4 main(PixelIn pixel) : COLOR0
{
	float color = pixel.depth.z / pixel.depth.w;
	return float4(color, color, color, 1.0f);
}