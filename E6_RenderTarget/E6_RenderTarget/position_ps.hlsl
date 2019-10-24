SamplerState Sampler0 : register(s0);
Texture2D texture0 : register(t0);

cbuffer PlayerPosBuffer : register(b0)
{
	float2 screenDimensions;
	float2 padding;
	float3 playerPosition;
	float padding2;

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

// Make it grayscale by averaging the colour
float4 averageColor(in float4 colorToAverage)
{
	float avg = (colorToAverage.x + colorToAverage.y + colorToAverage.z) / 3.f;

	return float4(avg, avg, avg, colorToAverage.w);
}

// place a dot where the player is... probably not the best place and should've made this into another shader, maybe?
float4 shadePlayerPos(in float4 texel, in float4 playerPos, in float radius, in float4 shadeColor, in float4 noShadeColor)
{
	if (pow(texel.x - playerPos.x, 2.f) + pow(texel.y - playerPos.y , 2.f) < pow(radius, 2.f))
	{
		//inside circle
		return shadeColor;
	}
	else
	{
		return noShadeColor;
	}
}

float4 main(InputType input) : SV_TARGET
{
	//MULTIPLY PLAYER POS BY MATRICES
	float4 playerPosFloat4 = mul(float4(playerPosition, 1.f), worldMatrixAtTopDown);
	playerPosFloat4 = mul(playerPosFloat4, viewMatrixAtTopDown);
	playerPosFloat4 = mul(playerPosFloat4, projectionMatrixAtTopDown);

	//MAKE UNIT
	playerPosFloat4.xyz /= playerPosFloat4.w;
	//playerPosFloat4.y = playerPosFloat4.z;
	playerPosFloat4.xy *= float2(0.5f, -0.5f);
	playerPosFloat4.xy += 0.5f;

	//MAKE IT INTO SCREENSPACE
	playerPosFloat4.xy *= screenDimensions;
	
	/*bool x = fmod(input.position.x, playerPosFloat4.x) > 20.f;
	bool y = fmod(input.position.y, playerPosFloat4.y) > 20.f;*/


	//NOW DRAW A CIRCLE
	float4 textureColor = texture0.Sample(Sampler0, input.tex);
	return shadePlayerPos(input.position, playerPosFloat4, 20.f, float4(1.f, 0.f, 1.f, 0.f), averageColor(textureColor));

	return float4(0.f,0.f,0.f,1.f);
}