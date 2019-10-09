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
	float amplitudeSin;
	float amplitudeCos;
	float speedSin;
	float speedCos;
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

// 'wave' the y of the vertex according to the time and position on the sine and cosine waves
float4 waveY(in float4 inputPos, in float time, in float amplitudeSin, in float amplitudeCos, in float speedSin, in float speedCos)
{
	float4 outputPos = inputPos;

	outputPos.y += sin(inputPos.x + time * speedSin) * amplitudeSin;
	outputPos.y += cos(inputPos.z + time * speedCos) * amplitudeCos;

	return outputPos;
}

// calculate normals based on the average of four verticies which were waved. If the vertices
// do not exist, create a hypothetical one, one unit next to them.
float3 calculateNormal(in float4 position, in float time, in float amplitudeSin, in float amplitudeCos, in float speedSin, in float speedCos)
{
	//get positions 1u from N, E, S, W and wave them
	float4 northPos = position;
	northPos.z += 1.f;
	northPos = waveY(northPos, time, amplitudeSin, amplitudeCos, speedSin, speedCos);
	northPos -= position;
	normalize(northPos);

	float4 eastPos = position;
	eastPos.x += 1.f;
	eastPos = waveY(eastPos, time, amplitudeSin, amplitudeCos, speedSin, speedCos);
	eastPos -= position;
	normalize(eastPos);

	float4 southPos = position;
	southPos.z -= 1.f;
	southPos = waveY(southPos, time, amplitudeSin, amplitudeCos, speedSin, speedCos);
	southPos -= position;
	normalize(southPos);

	float4 westPos = position;
	westPos.x -= 1.f;
	westPos = waveY(westPos, time, amplitudeSin, amplitudeCos, speedSin, speedCos);
	westPos -= position;
	normalize(westPos);

	//Now cross to find their normal
	float3 NorthCrossEastNormal = normalize(cross(northPos.xyz, eastPos.xyz));
	float3 EastCrossSouthNormal = normalize(cross(eastPos.xyz, southPos.xyz));
	float3 SouthCrossWestNormal = normalize(cross(southPos.xyz, westPos.xyz));
	float3 WestCrossNorthNormal = normalize(cross(westPos.xyz, northPos.xyz));

	//Average the normals
	return normalize((NorthCrossEastNormal + EastCrossSouthNormal + SouthCrossWestNormal + WestCrossNorthNormal) / 4.f);
}

OutputType main(InputType input)
{
	OutputType output;

	input.position = waveY(input.position, time, amplitudeSin, amplitudeCos, speedSin, speedCos);

	input.normal = calculateNormal(input.position, time, amplitudeSin, amplitudeCos, speedSin, speedCos);

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