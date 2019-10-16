// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Make it grayscale by averaging the colour
float4 averageColor(in float4 colorToAverage)
{
	float avg = (colorToAverage.x + colorToAverage.y + colorToAverage.z) / 3.f;

	return float4(avg, avg, avg, colorToAverage.w);
}


float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = texture0.Sample(Sampler0, input.tex);

	textureColor = averageColor(textureColor);

	return textureColor;
}