// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColor;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = texture0.Sample(Sampler0, input.tex);

	//Due to the way colour theory, inverse of A COLOUR CHANNEL AT 0.3 is 0.7 as you take away from 1 (as you cannot have -0.3 color)
	textureColor.x = 1.f - textureColor.x;
	textureColor.y = 1.f - textureColor.y;
	textureColor.z = 1.f - textureColor.z;

	return textureColor;
}