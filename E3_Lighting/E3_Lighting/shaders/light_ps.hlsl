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
	float4 colour = ambientColour; //Add ambient to colour

	//now calculate point light
	float3 lightToPixelVec = position.xyz - lightPosition; //get vector between pos and light
	float distance = length(lightToPixelVec); //get the length between the two

	if (distance > pointLightRange) //dont draw if out of range
		return colour; // i.e. just return color (ambient

	lightToPixelVec /= distance; //make it a unit vector

	float normalIntensity = saturate(dot(normal, lightDirection));

	float lightIntensity = -dot(lightToPixelVec, normal); //get the instensity the negative dot fixes the light

	if (lightIntensity > 0.f) //for attenuation
	{
		float4 lightColour = saturate(lightIntensity * diffuse * diffuseColour); //add the instensity

		lightColour /= attenuationConstant + (attenuationLinear * distance) + (attenuationExponential * (distance * distance)); //divide the intensity at the pixel by the attenuation factor

		colour = saturate(lightColour + colour); //finally add the calculated light intensity at the pixel to the final colour and clamp between 0 and 1
	}

	return colour;
}

// Calculate a spot light
float4 calculateLightingSpot(float3 lightDirection, float3 normal, float4 diffuse, float4 position)
{
	float4 colour = ambientColour; //Add ambient to colour

	float angle = cos(0.785f); //0.785 radians to cos which is 45 degrees

	//now calculate the distance between the pixel and the spot light position
	float3 lightToPixelVec = lightPosition - position.xyz;

	float distance = length(lightToPixelVec); //to make lightToPixelVec a unit vector later

	if (distance > pointLightRange) //if out of range
		return colour;

	lightToPixelVec /= distance; //make unit vector as to make calculating the angle possible

	float cosAngleBetweenTwo = dot(lightDirection, lightToPixelVec); //calculate angle between the normal of the light and the normal of the vector between px and point

	//float normalIntensity = saturate(dot(normal, lightDirection)); //so the light respects the normals

	if (cosAngleBetweenTwo > angle /*&& normalIntensity > 0.f*/) // light the pixel if the angle is correct and the normal is correctly aligned
		return colour + (diffuseColour /** normalIntensity*/);
		
	return colour; //otherwise dont light
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

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);
	//lightColour += calculateLightingDirectional(-lightDirection, input.normal, diffuseColour);
	//lightColour += calculateLightingPoint(-lightDirection, input.normal, diffuseColour, input.worldPos);
	lightColour += calculateLightingSpot(-lightDirection, input.normal, diffuseColour, input.worldPos);

	lightColour *= textureColour;
	lightColour += calculateSpecular(input.viewVector, input.normal);
	return lightColour;
}



