Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer PixelShaderBuffer : register(b0)
{
	float2 screenDimensions;
	float2 padding;
}

struct InputType
{ 
	float4 position: SV_POSITION; 
	float2 tex: TEXCOORD0; 
};

float4 main(InputType input) : SV_TARGET
{
	float4 colour = float4(0.f, 0.f, 0.f, 0.f);

	float texelWidth = 1.f / screenDimensions.x;
	float texelHeight = 1.f / screenDimensions.y;

	colour += texture0.Sample(sampler0, input.tex);
	colour += texture0.Sample(sampler0, input.tex + float2(-texelWidth, -texelHeight));
	colour += texture0.Sample(sampler0, input.tex + float2(0.0f, texelHeight));
	colour += texture0.Sample(sampler0, input.tex + float2(texelWidth, -texelHeight));
	colour += texture0.Sample(sampler0, input.tex + float2(-texelWidth, 0.0f));
	colour += texture0.Sample(sampler0, input.tex + float2(texelWidth, 0.0f));
	colour += texture0.Sample(sampler0, input.tex + float2(-texelWidth, texelHeight));
	colour += texture0.Sample(sampler0, input.tex + float2(0.0f, texelHeight));
	colour += texture0.Sample(sampler0, input.tex + float2(texelWidth, texelHeight));

	colour /= 9.f;

	colour.a = 1.f;

	return colour;
}