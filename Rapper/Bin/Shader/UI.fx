
#include "Share.fx"

cbuffer Button	: register(b9)
{
	float4	g_vBSColor;
}

cbuffer Animation2D : register(b8)
{
	float2	g_vAnimvUVLT;
	float2	g_vAnimvUVRB;
	int		g_iAnimFrame;
	float3	g_vAnimEmpty;
}

cbuffer Bar	: register(b9)
{
	float	g_fBarPercent;
	int		g_iBarMoveDir;
	float2	g_vBarEmpty;
	float4	g_vBarLight;
}

#define	BAR_LEFT	0
#define	BAR_RIGHT	1
#define	BAR_UP		2
#define	BAR_DOWN	3

VS_OUTPUT_TEX UIVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vPivot * g_vLength;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	if (g_iAnimation2DEnable == 1)
	{
		if (input.vUV.x == 0.f)
			output.vUV.x = g_vAnimvUVLT.x;

		else
			output.vUV.x = g_vAnimvUVRB.x;

		if (input.vUV.y == 0.f)
			output.vUV.y = g_vAnimvUVLT.y;

		else
			output.vUV.y = g_vAnimvUVRB.y;
	}

	else
		output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE ButtonPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * g_vBSColor;

	return output;
}

PS_OUTPUT_SINGLE BarPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV) * g_vBarLight;

	return output;
}
