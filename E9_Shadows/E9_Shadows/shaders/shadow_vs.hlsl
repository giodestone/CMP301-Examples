#define NO_OF_LIGHTS 2

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrixOLD_DO_NOT_USE;
    matrix lightProjectionMatrixOLD_DO_NOT_USE;
};

cbuffer LightMatrixBuffer : register(b1)
{
    matrix lightViewMatrix[NO_OF_LIGHTS];
    matrix lightProjectionMatrix[NO_OF_LIGHTS];
}

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
    float4 lightViewPos[NO_OF_LIGHTS] : TEXCOORD1;
};


OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the position of the vertice as viewed by the light source.
    
    for (int i = 0; i < NO_OF_LIGHTS; ++i)
    {
		output.lightViewPos[i] = mul(input.position, worldMatrix);
		output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
		output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    }


    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}