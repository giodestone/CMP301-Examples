// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour;
	float3 lightDirection;
	float padding; //REMEMBER THE PADING AFTER EVERY FLOAT3
	
	float4 ambientColour;
	
	//for the point light
	float3 lightPosition;
	float padding2; //AGAIN REMEMBER THE PADDING
	float pointLightRange;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationExponential;
};

struct InputType
{
	float4 position : SV_POSITION;
	float4 worldPos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse, float4 position)
{
	//calculate ambient and diffuse
	//float intensity = saturate(dot(normal, lightDirection));
	//float4 colour = saturate(diffuse * intensity);
	//NOTE GOTTEN RID OF THE DIRECTIONAL LIGHT FUNCTIONALITY!

	// https://www.braynzarsoft.net/viewtutorial/q16390-17-point-lights
	float4 colour = diffuse * ambientColour; //Add ambient to colour

	//now calculate point light
	float3 lightToPixelVec = position.xyz - lightPosition; //////////////////////////////////////////TODO: ASK IF WORLD POS IS ACTUALLY GIVING WORLD POS OR PIXEL POS BECAUSE THAT WOULD EXPLAIN THE STRANGE BEHAVIOUR
	float distance = length(lightToPixelVec);

	if (distance > pointLightRange)
		return colour;

	lightToPixelVec /= distance;

	float lightIntensity = dot(lightToPixelVec, normal);

	if (lightIntensity > 0.f)
	{
		float4 lightColour = float4(0.f, 0.f, 0.f, 0.f);
		
		lightColour += saturate(lightIntensity * diffuse * diffuseColour);

		lightColour /= attenuationConstant + (attenuationLinear * distance) + (attenuationExponential * (distance * distance));

		colour = saturate(lightColour + colour);
	}

	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);
	lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour, input.worldPos);

	return lightColour * textureColour;
}



