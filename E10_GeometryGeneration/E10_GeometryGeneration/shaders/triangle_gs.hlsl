// triangle_gs
// Geometry shader that generates a triangle for every vertex.

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraPosBuffer : register(b1)
{
	float4 cameraPos;
};

cbuffer GeometryBuffer
{
	static float4 QuadVertices[4] =
	{
		float4(-0.5,  0.5, 0.0, 0.0), //top left
		float4(-0.5, -0.5, 0.0, 0.0), //bottom left
		float4( 0.5, -0.5, 0.0, 0.0), //bottom right
		float4( 0.5,  0.5, 0.0, 0.0) //top right
	};
	
	static float2 QuadUVs[4] =
	{
		float2(0.0, 0.0),
		float2(0.0, 1.0),
		float2(1.0, 1.0),
		float2(1.0, 0.0)
	};

	static float3 QuadNormal = float3(0.0, 0.0, 1.0);
	static float padding;
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
	float4 worldPosition : POSITION1;
};

////https://gist.github.com/neilmendoza/4512992
//matrix rotationMatrix(float3 axis, float angle)
//{
//	axis = normalize(axis);
//	float s = sin(angle);
//	float c = cos(angle);
//	float oc = 1.0 - c;

//	matrix returnMatrix = { oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
//		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
//		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
//		0.0, 0.0, 0.0, 1.0 };

//	return returnMatrix;
//}

//https://gist.github.com/mattatz/86fff4b32d198d0928d0fa4ff32cf6fa#file-matrix-hlsl-L203
float4x4 axis_matrix(float3 right, float3 up, float3 forward)
{
	float3 xaxis = right;
	float3 yaxis = up;
	float3 zaxis = forward;
	return float4x4(
		xaxis.x, yaxis.x, zaxis.x, 0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		0, 0, 0, 1
		);
}

float4x4 look_at_matrix(float3 at, float3 eye, float3 up)
{
	float3 zaxis = normalize(at - eye);
	float3 xaxis = normalize(cross(up, zaxis));
	float3 yaxis = cross(zaxis, xaxis);
	return axis_matrix(xaxis, yaxis, zaxis);
}

[maxvertexcount(6)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;
	
	////generate a new world matrix that looks at x 
	//float4 pos = input[0].cameraWorldPos - input[0].position;
	//float angleYaw = atan(pos.z / pos.x);

	//float anglePitch = atan(pos.y / pos.z);

	//matrix rotationMatrixYaw = (float3(0.f, 1.f, 0.f), angleYaw);
	//matrix newWorldMatrix = mul(worldMatrix, rotationMatrixYaw);

	//make sprite look at camera

	//https://www.braynzarsoft.net/viewtutorial/q16390-36-billboarding-geometry-shader

    float3 posToCamVec = normalize(cameraPos.xyz - input[0].position.xyz);

    float3 upVector = float3(0.f, 1.f, 0.f);
	upVector = float3(0.0, 0.0, 0.0);

    float3 rightVector = normalize(cross(posToCamVec, upVector));
	rightVector = float3(0.0, 0.0, 0.0);
    //float4 rightVector = float4(1.f, 1.f, 1.f, 1.f);
    /*rightVector = normalize(rightVector);*/

    //rightVector = float4(normalize(posToCamVec), 1.f);

	float posW = input[0].position.w;

	// Move the vertex away from the point position
    output.position = float4(input[0].position.xyz + QuadVertices[0].xyz - rightVector /*+ upVector*/, posW);
    output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex + QuadUVs[0];
    output.normal = mul(QuadNormal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position.xyz = float4(input[0].position.xyz + QuadVertices[1].xyz - rightVector, posW);
    output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex + QuadUVs[1];
    output.normal = mul(QuadNormal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position.xyz = float4(input[0].position.xyz + QuadVertices[2].xyz + rightVector, posW);
    output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex + QuadUVs[2];
    output.normal = mul(QuadNormal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	
	/*
	0    3

	1    2
	
	
	*/

	//tri 2
	output.position.xyz = float4(input[0].position.xyz + QuadVertices[3].xyz + rightVector + upVector, posW);
	output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex + QuadUVs[3];
	output.normal = mul(QuadNormal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

    output.position.xyz = float4(input[0].position.xyz + QuadVertices[0].xyz - rightVector /*+ upVector*/, posW);
	output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex + QuadUVs[0];
	output.normal = mul(QuadNormal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

	output.position.xyz = float4(input[0].position.xyz + QuadVertices[2].xyz + rightVector, posW);
	output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex + QuadUVs[2];
	output.normal = mul(QuadNormal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

    triStream.RestartStrip();
}