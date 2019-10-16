// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer PlayerDotBuffer : register(b0)
{
	float4 dotColor;
	float dotRadius;
	bool shouldDrawDot;
	float2 padding2;
};


struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 playerScreenPos : TEXCOORD1;
};

// Make it grayscale by averaging the colour
float4 averageColor(in float4 colorToAverage)
{
	float avg = (colorToAverage.x + colorToAverage.y + colorToAverage.z) / 3.f;

	return float4(avg, avg, avg, colorToAverage.w);
}

// place a dot where the player is... probably not the best place and should've made this into another shader, maybe?
float4 shadePlayerPos(in float4 position, in float2 playerPos, in float4 color, in float radius)
{
	float dx = position.x /*- (radius * 2.f)*/;
	float dy = position.y /*- (radius * 2.f)*/;

	if ((dx * dx) + (dy * dy) <= radius)
		return color;

	return float4(0.f, 0.f, 0.f, 0.f);
}


float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = texture0.Sample(Sampler0, input.tex);

	//textureColor = averageColor(textureColor);

	return shadePlayerPos(input.position - (input.playerScreenPos / 2.f), float4(0.f, 0.f, 0.f, 0.f), float4(1.f, 0.f, 1.f, 1.f), 200.f) + textureColor;
}