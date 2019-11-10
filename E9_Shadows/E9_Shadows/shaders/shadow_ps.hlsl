#define NO_OF_LIGHTS 2

Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture[NO_OF_LIGHTS] : register(t1);

SamplerState diffuseSampler  : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambient;
	float4 diffuse;
	float3 direction;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPos[NO_OF_LIGHTS] : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float depthValue;
    float lightDepthValue;
    float shadowMapBias = 0.005f; //0.000 bad shadow acne, 0.100 peter paning
    float4 colour = float4(0.f, 0.f, 0.f, 1.f);
	float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);

    for (int i = 0; i < 1; i++)
    {
		// Calculate the projected texture coordinates.
		float2 pTexCoord = input.lightViewPos[i].xy / input.lightViewPos[i].w;
		pTexCoord *= float2(0.5, -0.5);
		pTexCoord += float2(0.5f, 0.5f);

		// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
		if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
		{
            //continue;
			//return textureColour;
        }
        else
        {
			// Sample the shadow map (get depth of geometry)
			depthValue = depthMapTexture[i].Sample(shadowSampler, pTexCoord).r;
			// Calculate the depth from the light.
			lightDepthValue = input.lightViewPos[i].z / input.lightViewPos[i].w;
			lightDepthValue -= shadowMapBias;

			// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
			if (lightDepthValue < depthValue)
			{
				colour += calculateLighting(-direction, input.normal, diffuse);
			}
        }
    }
    
    colour = saturate(colour + ambient);
    return saturate(colour) * textureColour;
}