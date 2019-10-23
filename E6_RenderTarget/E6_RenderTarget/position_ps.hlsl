SamplerState Sampler0 : register(s0);
Texture2D texture0 : register(t0);

cbuffer PlayerPosBuffer : register(b0)
{
	float2 screenDimensions;
	float3 playerPosition;
	float3 padding;

	matrix worldMatrixAtTopDown;
	matrix viewMatrixAtTopDown;
	matrix projectionMatrixAtTopDown;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// place a dot where the player is... probably not the best place and should've made this into another shader, maybe?
float4 shadePlayerPos(in float4 position, in float2 playerPos, in float4 color, in float4 noColor, in float radius)
{
	float dx = position.x /*- (radius * 2.f)*/;
	float dy = position.y /*- (radius * 2.f)*/;

	if ((dx * dx) + (dy * dy) <= radius)
		return color;

	return float4(0.f, 0.f, 0.f, 0.f);
}

float4 main(InputType input) : SV_TARGET
{
	//MULTIPLY PLAYER POS BY MATRICES
	float4 playerPosFloat4 = mul(float4(playerPosition, 1.f), worldMatrixAtTopDown);
	playerPosFloat4 = mul(playerPosFloat4, viewMatrixAtTopDown);
	playerPosFloat4 = mul(playerPosFloat4, projectionMatrixAtTopDown);

	//MAKE UNIT
	playerPosFloat4.xyz /= playerPosFloat4.w;
	playerPosFloat4.y = playerPosFloat4.z;
	playerPosFloat4.xy *= float2(0.5f, -0.5f);
	playerPosFloat4.xy += 0.5f;

	//MAKE IT INTO SCREENSPACE
	playerPosFloat4.x *= 1200.f;
	playerPosFloat4.y *= 675.f;
	
	bool x = fmod(input.position.x, playerPosFloat4.x) > 20.f;
	bool y = fmod(input.position.y, playerPosFloat4.y) > 20.f;

	//NOW DRAW A CIRCLE
	//if (playerPosFloat4.x == input.position.x && playerPosFloat4.y == input.position.y)
	if (x || y)
		return float4(1.f, 1.f, 1.f, 1.f);


	return float4(0.f,0.f,0.f,1.f);
}