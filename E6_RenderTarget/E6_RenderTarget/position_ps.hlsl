cbuffer PlayerPosBuffer : register(b0)
{
	float2 screenDimensions;
	float2 playerPosition;

	matrix worldMatrixAtTopDown;
	matrix viewMatrixAtTopDown;
	matrix projectionMatrixAtTopDown;
}

// place a dot where the player is... probably not the best place and should've made this into another shader, maybe?
float4 shadePlayerPos(in float4 position, in float2 playerPos, in float4 color, in float radius)
{
	float dx = position.x /*- (radius * 2.f)*/;
	float dy = position.y /*- (radius * 2.f)*/;

	if ((dx * dx) + (dy * dy) <= radius)
		return color;

	return float4(0.f, 0.f, 0.f, 0.f);
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}