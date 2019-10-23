// texture vertex shader
// Basic shader for rendering textured geometry

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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

OutputType main(InputType input)
{
	OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

    output.normal = input.normal;

	//output.playerScreenPos = float4(200.f, 200.f, 200.f, 1.f);

	///////////////////////////////////////////////////////////////////TODO FIX: ISSUE ISD SOMETHING TO DO WITH THESE MATRICES AND PROBABLY HOW A CIRLCE IS DRAWN IN TEXTURE_PS,.HLSL
	// IF YOU GO FOWARD YOU WILL SEE A PINK DOT THAT MOVES WITH THE CAMERA BUT NOT HOW WANTED. IT COULD BE IN HOW THE CIRLCE IS DRAWN...............,.,.,.,

	//multiply the player by matrices to get them into screen space
	//output.playerScreenPos = mul(playerPos, worldAtTopDown);
	//output.playerScreenPos = mul(output.playerScreenPos, orthoViewMatrix);
	//output.playerScreenPos = mul(output.playerScreenPos, orthoMatrix);

	//output.playerScreenPos.xyz /= output.playerScreenPos.w;
	//output.playerScreenPos.y = output.playerScreenPos.z;
	//output.playerScreenPos.xy *= float2(0.5f, -0.5f);
	//output.playerScreenPos.xy += 0.5f;
	//
	//output.playerScreenPos.x *= 1200.f /*/ 4.f*/;
	//output.playerScreenPos.y *= 675.f /*/ 4.f*/;

	return output;
}