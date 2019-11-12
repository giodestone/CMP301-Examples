// Simple throughput vertex shader. Work being done by the geometry shader.

cbuffer CameraPosBuffer : register(b0)
{
	float4 cameraPos;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float4 worldPos : POSITION1;
	float4 cameraWorldPos : POSITION2;
};

InputType main(InputType input)
{
	input.cameraWorldPos = cameraPos;
	return input;
}