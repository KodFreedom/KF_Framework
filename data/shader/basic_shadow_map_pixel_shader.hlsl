//--------------------------------------------------------------------------------
//  Constant table
//--------------------------------------------------------------------------------
float light_far;

//--------------------------------------------------------------------------------
//  In/Out struct
//--------------------------------------------------------------------------------
struct PixelIn
{
	float4 depth : TEXCOORD0;
};

//--------------------------------------------------------------------------------
//  Shader method
//--------------------------------------------------------------------------------
float4 main(PixelIn pixel) : COLOR0
{
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	float depth = pixel.depth.z / pixel.depth.w;
	return float4(depth, depth, depth, 1.0f);
}