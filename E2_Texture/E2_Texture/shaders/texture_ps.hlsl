// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);

SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColor;
	float4 textureColorA;
	float4 textureColorB;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColorA = texture0.Sample(Sampler0, input.tex);

	// Sample colour from the second texture
	textureColorB = texture1.Sample(Sampler0, input.tex);

	// Now multiply them together, the last number makes it brighter (if more) or if smaller than 1 it gets dimmer
	textureColor = textureColorA * textureColorB * 1.f;

	return textureColor;
}