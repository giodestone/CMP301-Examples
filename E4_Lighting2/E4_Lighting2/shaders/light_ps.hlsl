// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

#define maxLights 2

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColor;
	float4 specularDiffuse;

	float3 directnlLightDirection;
	float padding;
	float4 directnlLightDiffuse;

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
	float3 viewVector : TEXCOORD2;
};

float4 calculateSpecular(float3 viewVector, float3 normal, float3 directionalLightDirection, float4 specularColor)
{
	float3 halfWay = normalize(directionalLightDirection + viewVector);
	float specularIntensity = pow(max(dot(normal, halfWay), 0.f), 100.f /*THIS WOULD BE A VARIABLE - HIGHER VALUES MORE CONCENTRATED SPECULAR!*/);

	return specularColor * specularIntensity;
}

float4 calculateLightingDirectional(float3 lightDirection, float3 normal, float4 direcitonalDiffuse)
{
	float4 color = float4(0.f, 0.f, 0.f, 0.f);

	//calculate intensity of direcitonal light at pixel taking into account the normal
	float intensity = saturate(dot(normal, lightDirection));

	// add it to the color
	color += saturate(direcitonalDiffuse * intensity); //saturate clamps between 0 and 1

	return color;
}

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
	float4 lightColour = ambientColor;
	for (int i = 0; i < maxLights; ++i)
	{
		float3 lightVector = normalize(positions[i] - input.worldPosition);
		lightColour += calculateLighting(lightVector, input.normal, positions[i], diffuses[i], input.worldPosition, attenuationConstant, attenuationLinear, atteunationExp);
	}
	
	//Add directional
	lightColour += calculateLightingDirectional(-directnlLightDirection, input.normal, directnlLightDiffuse);

	lightColour *= textureColour; //multiply texture color by lught color

	//Return the light colour with the specular colour
	return saturate(lightColour + calculateSpecular(input.viewVector, input.normal, -directnlLightDirection /*Inverted so it comes from the right side, ask erin about maths*/, specularDiffuse)); //direction needs to be inverted to make sure it comes from the right side
}



