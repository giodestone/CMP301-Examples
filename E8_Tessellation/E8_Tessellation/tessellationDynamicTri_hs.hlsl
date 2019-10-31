// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer DynamicCamTesselBuffer : register(b0)
{
	float3 cameraPos;
	float tessFactor;
	float maxDistance;
	float maxTessFactor;
	float2 padding;
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


	//vertex to camera = camera pos - vertex pos

	float3 patchDist = inputPatch[patchId].position - cameraPos;
	//float3 p1Dist = inputPatch[patchId].position - cameraPos;
	//float3 p2Dist = inputPatch[patchId].position - cameraPos;
	//float3 centrePos = (p0Dist + p1Dist + p2Dist) / 3.f; //average of the three aka the middle

	float distanceCamToCentre = length(patchDist);
	float distanceZeroToOneCentre = 1.f - clamp(distanceCamToCentre / maxDistance, 0.f, 1.f);
	float tesselFactorCentre = clamp(distanceZeroToOneCentre * maxTessFactor, 1.f, maxTessFactor);

	//// Set the tessellation factors for the three edges of the triangle.
	//output.edges[0] = tesselationFactorTopEdge;
	//output.edges[1] = tesselationFactorBottomLeft;
	//output.edges[2] = tesselationFactorBottomRight;

	//// Set the tessellation factor for tessallating inside the triangle.
	//output.inside = tesselationFactorInside;

	output.edges[0] = output.edges[1] = output.edges[2] = output.inside = tesselFactorCentre;

	return output;
}


[domain("tri")]
[partitioning("fractional_odd")]
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

/*// Input control point
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
}*/
