Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
    float screenHeight;
    float3 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;  
};

float4 main(InputType input) : SV_TARGET
{
    float4 colour;

#ifdef TWO_PIXEL_BLUR
    float weight0, weight1, weight2;
	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 0.4062f;
    weight1 = 0.2442f;
    weight2 = 0.0545f;

    // Initialize the colour to black.
    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float texelSize = 1.0f / screenHeight;
    // Add the vertical pixels to the colour by the specific weight of each.
    colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -2.0f)) * weight2;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -1.0f)) * weight1;
    colour += shaderTexture.Sample(SampleType, input.tex) * weight0;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 1.0f)) * weight1;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 2.0f)) * weight2;
 
    // Set the alpha channel to one.
    colour.a = 1.0f;
#else
	float weight0 = 0.382928, weight1 = 0.241732, weight2 = 0.060598, weight3 = 0.005977, weight4 = 0.000229;
	
	// Initialize the colour to black.
	colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float texelSize = 1.0f / screenHeight;

	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -4.0f)) * weight4;
	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -3.0f)) * weight3;
	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -2.0f)) * weight2;
	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -1.0f)) * weight1;
	colour += shaderTexture.Sample(SampleType, input.tex) * weight0;
	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 1.0f)) * weight1;
	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 2.0f)) * weight2;
	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 3.0f)) * weight3;
	colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 4.0f)) * weight4;

	// Set the alpha channel to one.
	colour.a = 1.0f;
#endif
    return colour;
}

