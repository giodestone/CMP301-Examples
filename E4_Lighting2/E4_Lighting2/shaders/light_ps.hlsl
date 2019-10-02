// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

#define maxLights 2

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambient;

	float attenuationConstant;
	float attenuationLinear;
	float atteunationExp;
	float padding2;
};

cbuffer MultiLightBuffer : register(b1)
{
	float4 diffuses[2];
	float4 positions[2];
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 lightPos, float4 ldiffuse, float3 worldPosition, float attenuationConstant, float attenuationLinear, float attenuationExp)
{
	float3 lightToPxDistanceVec = lightPos.xyz - worldPosition;
	float lightToPxDistance = length(lightToPxDistanceVec);

	float intensity = saturate(dot(normal, lightDirection));
	if (intensity < 0.f)
		return float4(0.f, 0.f, 0.f, 0.f); //dont light
	
	float4 colour = saturate(ldiffuse * intensity);
	colour /= attenuationConstant + (attenuationLinear * lightToPxDistance) + (atteunationExp * (lightToPxDistance * lightToPxDistance));
	
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	float4 textureColour = texture0.Sample(sampler0, input.tex);
	float4 lightColour = ambient;
	for (int i = 0; i < maxLights; ++i)
	{
		float3 lightVector = normalize(positions[i] - input.worldPosition);
		lightColour += calculateLighting(lightVector, input.normal, positions[i], diffuses[i], input.worldPosition, attenuationConstant, attenuationLinear, atteunationExp);
	}
	
	return lightColour * textureColour;
}



