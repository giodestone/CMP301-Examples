// triangle_gs
// Geometry shader that generates a triangle for every vertex.

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer GeometryBuffer
{
	static float4 QuadVertices[4] =
	{
		float4(-1.0, 1.0, 0.0, 0.0),
		float4(-1.0, 0.0, 0.0, 0.0),
		float4(1.0, 0.0, 0.0, 0.0),
		float4(1.0, 1.0, 0.0, 0.0)
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
	float4 worldPosition : POSITION1;
	float4 cameraWorldPos : POSITION2;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPosition : POSITION1;
};

//https://gist.github.com/neilmendoza/4512992
matrix rotationMatrix(float3 axis, float angle)
{
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;

	matrix returnMatrix = { oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	return returnMatrix;
}

[maxvertexcount(6)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;
	
	//generate a new world matrix that looks at x 
	float4 pos = input[0].cameraWorldPos - input[0].position;
	float angleYaw = atan(pos.z / pos.x);

	float anglePitch = atan(pos.y / pos.z);

	matrix rotationMatrixYaw = (float3(0.f, 1.f, 0.f), angleYaw);
	matrix newWorldMatrix = mul(worldMatrix, rotationMatrixYaw);

	// Move the vertex away from the point position
    output.position = input[0].position + QuadVertices[0];
    output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex + QuadUVs[0];
    output.normal = mul(QuadNormal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position = input[0].position + QuadVertices[1];
    output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex + QuadUVs[1];
    output.normal = mul(QuadNormal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position = input[0].position + QuadVertices[2];
    output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex + QuadUVs[2];
    output.normal = mul(QuadNormal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	
	//tri 2
	output.position = input[0].position + QuadVertices[2];
	output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex + QuadUVs[2];
	output.normal = mul(QuadNormal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

	output.position = input[0].position + QuadVertices[0];
	output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex + QuadUVs[0];
	output.normal = mul(QuadNormal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

	output.position = input[0].position + QuadVertices[3];
	output.position = mul(output.position, worldMatrix);
	output.worldPosition = output.position; //world pos
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex + QuadUVs[3];
	output.normal = mul(QuadNormal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

    triStream.RestartStrip();
}