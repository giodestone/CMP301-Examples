Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{ 
	float4 position: SV_POSITION; 
	float2 tex: TEXCOORD0; 
};

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}