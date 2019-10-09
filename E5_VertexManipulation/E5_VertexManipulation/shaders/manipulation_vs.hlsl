// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer OtherDataBuffer : register(b1)
{
	float time;
	float3 padding;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// 'wave' the vertex according to the time and position on the sine and cosine waves
float4 waveVertex(in float4 inputPos, in float time)
{
	float4 outputPos = inputPos;

	outputPos.y = sin(inputPos.x + time);
	outputPos.y += cos(inputPos.z + time);

	return outputPos;
}

// calculate normals based on the average of four verticies which were waved. If the vertices
// do not exist, create a hypothetical one, one unit next to them.
float3 calculateNormal(in float4 position, in float time)
{
	float4 northPos = position;
	northPos.z += 1.f;
	northPos = waveVertex(northPos, time);
	northPos = normalize(northPos);

	float4 eastPos = position;
	eastPos.x += 1.f;
	eastPos = waveVertex(eastPos, time);
	eastPos = normalize(eastPos);

	float4 southPos = position;
	southPos.z -= 1.f;
	southPos = waveVertex(southPos, time);
	southPos = normalize(southPos);

	float4 westPos = position;
	westPos.x -= 1.f;
	westPos = waveVertex(westPos, time);
	westPos = normalize(westPos);

	float3 NorthCrossEastNormal = normalize(cross(northPos.xyz, eastPos.xyz));
	float3 EastCrossSouthNormal = normalize(cross(eastPos.xyz, southPos.xyz));
	float3 SouthCrossWestNormal = normalize(cross(southPos.xyz, westPos.xyz));
	float3 WestCrossNorthNormal = normalize(cross(westPos.xyz, northPos.xyz));

	return (NorthCrossEastNormal + EastCrossSouthNormal + SouthCrossWestNormal + WestCrossNorthNormal) / 4.f;
	//float3 

}

OutputType main(InputType input)
{
	OutputType output;

	input.position = waveVertex(input.position, time);

	input.normal = calculateNormal(input.position, time);
	//input.normal.x = 1 - cos(input.position.x + time);
	//input.normal.y = abs(cos(input.position.x + time));

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}