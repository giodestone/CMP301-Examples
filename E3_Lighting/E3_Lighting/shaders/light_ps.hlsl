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
	float paddingt21;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationExponential;

	float4 specularColor;
	float specularPower;
	float3 padding3;
};

struct InputType
{
	float4 position : SV_POSITION;
	float4 worldPos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewVector : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLightingDirectional(float3 lightDirection, float3 normal, float4 diffuse)
{
	float4 color = ambientColour;

	//calculate intensity of direcitonal light at pixel
	float intensity = saturate(dot(normal, lightDirection));
	
	// add it to the color
	color += saturate(diffuse * intensity); //saturate clamps between 0 and 1

	return color;
}

// Calculate a point light
float4 calculateLightingPoint(float3 lightDirection, float3 normal, float4 diffuse, float4 position)
{
	float4 colour = ambientColour; //Set colour ambient colour

	//now calculate point light
	float3 pxToLightVec = position.xyz - lightPosition; //get vector between pos and light
	float distance = length(pxToLightVec); //get the length between the two
	pxToLightVec = normalize(pxToLightVec); //now make it unit after getting length

	float normalIntensity = saturate(dot(normal, -pxToLightVec)); //calculate the angle between the normal at the surface and the negative normal between the light and px (as to have a valid angle comparison - otherwise you would have two normals that face away from eachother (see Erin's notes))

	float4 lightColour = saturate(normalIntensity * diffuse); //add the instensity

	lightColour /= attenuationConstant + (attenuationLinear * distance) + (attenuationExponential * (distance * distance)); //divide the intensity at the pixel by the attenuation factor

	colour = saturate(lightColour + colour); //finally add the calculated light intensity at the pixel to the final colour and clamp between 0 and 1

	return colour;
}

// Calculate a spot light
float4 calculateLightingSpot(float3 lightDirection, float3 normal, float4 diffuse, float4 position)
{
	float4 colour = ambientColour; //Set colour to ambient colour
	
	float halfAngle = cos(0.785f); //0.785 radians to cos which is 45 degrees

	//now calculate point light
	float3 pxToLightVec = position.xyz - lightPosition; //get vector between pos and light
	float distance = length(pxToLightVec); //get the length between the two
	pxToLightVec = normalize(pxToLightVec); //now make it unit after getting length

	float normalIntensity = saturate(dot(normal, -pxToLightVec)); //calculate the angle between the normal at the surface and the negative normal between the light and px (as to have a valid angle comparison - otherwise you would have two normals that face away from eachother (see Erin's notes))

	float cosOfAngleBetweenPxAndLight = dot(lightDirection, -pxToLightVec /* negative makes it from light to px */); //get angle between the two to see if the light is actually within the cone of light

	if (cosOfAngleBetweenPxAndLight > halfAngle) //check if the normals 
	{
		float4 lightColour = saturate(normalIntensity * diffuse); //add the instensity

		lightColour /= attenuationConstant + (attenuationLinear * distance) + (attenuationExponential * (distance * distance)); //divide the intensity at the pixel by the attenuation factor

		colour = saturate(lightColour + colour); //finally add the calculated light intensity at the pixel to the final colour and clamp between 0 and 1
	}

	return colour;

}


float4 calculateSpecular(float3 viewVector, float3 normal)
{
	float3 halfWay = normalize(-lightDirection + viewVector);
	float specularIntensity = pow(max(dot(normal, halfWay), 0.f), specularPower);

	return specularColor * specularIntensity;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour = float4(0.f, 0.f, 0.f, 0.f);

	////////////////////////NOTE THE NEGATIVE DIRECTION!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);
	//lightColour += calculateLightingDirectional(-lightDirection, input.normal, diffuseColour);
	//lightColour += calculateLightingPoint(-lightDirection, input.normal, diffuseColour, input.worldPos);
	lightColour += calculateLightingSpot(-lightDirection, input.normal, diffuseColour, input.worldPos);

	lightColour *= textureColour;
	lightColour += calculateSpecular(input.viewVector, input.normal);
	return lightColour;
}



