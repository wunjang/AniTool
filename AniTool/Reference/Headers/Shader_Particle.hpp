#include "Shader_BaseRender.hpp"
#include "Shader_TextureAnim.hpp"

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float2 vTexUVSize : TEXCOORD1;
	float4 vParticlePos : TEXCOORD2;
	float4 vColor : COLOR0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float2 vConvertTexUV : TEXCOORD1;
	float4 vColor : COLOR0;

	float fSize : PSIZE;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	//matWVP = mul(matWV, g_matProj);

	vector vViewPos = mul(vector(In.vParticlePos.xyz,1.f), matWV);
	//Out.fSize = 1.f / sqrt(vViewPos.x*vViewPos.x+ vViewPos.y*vViewPos.y+ vViewPos.z*vViewPos.z);
	//Out.fSize = 1.f;

	Out.vPosition = mul(vViewPos, g_matProj);
	Out.vTexUV = In.vTexUV;
	Out.vConvertTexUV = In.vTexUVSize;
	Out.vColor = In.vColor;
	Out.fSize = (1000.f) * sqrt(1.f / pow(Out.vPosition.w, 2.f));
	return Out;
}

struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float2 vConvertTexUV : TEXCOORD1;
	float4 vColor : COLOR0;

};

struct PS_OUT
{
	vector	vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vUV = In.vTexUV * g_vUVOffset + In.vConvertTexUV * g_vUVOffset;
	//vUV.x += In.vConvertTexUV.x * g_vUVOffset;
	Out.vColor = tex2D(AlbedoSampler, vUV);
	Out.vColor *= In.vColor;
	return Out;
}


technique Default_Technique
{
	pass Opacity_Alpha
	{
		// RenderState
		Lighting = false;
	AlphaBlendEnable = TRUE;
	SrcBlend = SrcAlpha;
	DestBlend = InvSrcAlpha;
	
	pointspriteenable = true;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Additive_Blend
	{
		// RenderState
		Lighting = false;
	AlphaBlendEnable = TRUE;
	srcblend = srcalpha;
	destblend = one;

	pointspriteenable = true;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
	}
}


