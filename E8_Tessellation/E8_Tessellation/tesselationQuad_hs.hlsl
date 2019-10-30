// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TesselationBuffer : register(b0)
{
	float tessFactorInside;
	float tessFactorInside2;

	float tessFactorEdgeTLQuad;
	float tessFactorEdgeTRQuad;
	float tessFactorEdgeBLQuad;
	float tessFactorEdgeBRQuad;

	float2 padding;
}

struct InputType
{
	float3 position : POSITION;
	float4 colour : COLOR;
};

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
	float3 position : POSITION;
	float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output;

	// Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = tessFactorEdgeTLQuad;
	output.edges[1] = tessFactorEdgeBLQuad;
	output.edges[2] = tessFactorEdgeBRQuad;
	output.edges[3] = tessFactorEdgeTRQuad;

	// Set the tessellation factor for tessallating inside the triangle.
	output.inside[0] = tessFactorInside;
	output.inside[1] = tessFactorInside2;

	return output;
}


[domain("quad")]
//[patchsize(4)]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	OutputType output;

	// Set the position for this control point as the output position.
	output.position = patch[pointId].position;

	// Set the input colour as the output colour.
	output.colour = patch[pointId].colour;

	return output;
}


/*
// Input control point
struct VS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS;
	// TODO: change/add other stuff
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS; 
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] = 
		Output.EdgeTessFactor[1] = 
		Output.EdgeTessFactor[2] = 
		Output.InsideTessFactor = 15; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.vPosition = ip[i].vPosition;

	return Output;
}
*/