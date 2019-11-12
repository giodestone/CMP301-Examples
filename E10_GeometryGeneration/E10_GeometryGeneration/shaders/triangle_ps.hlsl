// Simple colour pixel shader

Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPos : POSITION1;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 lightPos, float4 ldiffuse, float3 worldPosition, float attenuationConstant, float attenuationLinear, float attenuationExp)
{
	float3 lightToPxDistanceVec = lightPos.xyz - worldPosition;
	float lightToPxDistance = length(lightToPxDistanceVec);

	float intensity = saturate(dot(normal, lightDirection));
	if (intensity < 0.f)
		return float4(0.f, 0.f, 0.f, 0.f); //dont light

	float4 colour = saturate(ldiffuse * intensity);
	colour /= attenuationConstant + (attenuationLinear * lightToPxDistance) + (attenuationExp * (lightToPxDistance * lightToPxDistance));

	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour = texture0.Sample(Sampler0, input.tex);

	float4 lightColor = calculateLighting(-float3(0.f, 0.f, -1.f), input.normal, float4(0.f, 0.f, -5.f, 1.f), float4(0.8f, 1.f, 0.2f, 0.f), input.worldPos, 1.f, 0.0f, 0.05f);

	return saturate(textureColour + lightColor);
}