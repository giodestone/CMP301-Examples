// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TesselationBuffer : register(b0)
{
	float tesselationFactorInside;
	float tesselationFactorTopEdge;
	float tesselationFactorBottomLeft;
	float tesselationFactorBottomRight;
}

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;


    // Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = tesselationFactorTopEdge;
	output.edges[1] = tesselationFactorBottomLeft;
	output.edges[2] = tesselationFactorBottomRight;

    // Set the tessellation factor for tessallating inside the triangle.
	output.inside = tesselationFactorInside;

    return output;
}


[domain("tri")]
[partitioning("integer")] //TODO: TASK 3
[outputtopology("triangle_ccw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input colour as the output colour.
    output.colour = patch[pointId].colour;

    return output;
}