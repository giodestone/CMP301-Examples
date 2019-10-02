// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambient;
	float4 diffuse[2];
	float4 position[2];

	float attenuationConstant;
	float attenuationLinear;
	float atteunationExp;
	float padding2;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse, float3 worldPosition, float attenuationConstant, float attenuationLinear, float attenuationExp)
{
	float3 lightToPxDistanceVec = position - worldPosition;
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
	float3 lightVector = normalize(position - input.worldPosition);
	float4 lightColour = ambient + calculateLighting(lightVector, input.normal, diffuse, input.worldPosition, attenuationConstant, attenuationLinear, atteunationExp);
	
	return lightColour * textureColour;
}



