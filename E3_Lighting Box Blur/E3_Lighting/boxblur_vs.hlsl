cbuffer MatrixBuffer: register(b0) 
{ 
	matrix worldMatrix; 
	matrix viewMatrix; 
	matrix projectionMatrix; 
}; 
struct InputType
{ 
	float4 position: POSITION; 
	float2 tex: TEXCOORD0; 
}; 

struct OutputType
{ 
	float4 position: SV_POSITION; 
	float2 tex: TEXCOORD0; 
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	OutputType output;
    output.position = mul(input.position, worldMatrix); 
	output.position = mul(output.position, viewMatrix); 
	output.position = mul(output.position, projectionMatrix); 
	output.tex = input.tex; 
	return output;
}