#include "Shader_BaseRender.hpp"
#include "Shader_TextureAnim.hpp"


float1		g_Gauge;
float		g_Alpha;
bool		g_ItemBack;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	//bool b = g_ItemBack;
	//if (b == true)
	//{
	//	float1 a = g_Alpha;

	//	Out.vColor.w = a;
	//}

	return Out;
}

PS_OUT PS_MAIN1(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	if (Out.vColor.x <= 1 && Out.vColor.x >= 0.1 && Out.vColor.y <= 1 && Out.vColor.y >= 0.1 && Out.vColor.z <= 1 && Out.vColor.z >= 0.1 && Out.vColor.w <= 1 && Out.vColor.w >= 0.1)
	{
		Out.vColor = vector(1, 0, 0, 1);
	}
	return Out;
}
PS_OUT PS_MAIN2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	float1 Gauge = g_Gauge;

	if (Out.vColor.x <= 1 && Out.vColor.x >= 0.1 && Out.vColor.y <= 1 && Out.vColor.y >= 0.1 && Out.vColor.z <= 1 && Out.vColor.z >= 0.1 && Out.vColor.w <= 1 && Out.vColor.w >= 0.1)
	{
		if (In.vTexUV.x < Gauge)
		{
			Out.vColor = vector(0, 0, 0, 1);
		}
		else
			Out.vColor = vector(1, 1, 1, 1);
	}
	return Out;
}

PS_OUT PS_MAIN3(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	if (Out.vColor.x <= 1 && Out.vColor.x >= 0.1 && Out.vColor.y <= 1 && Out.vColor.y >= 0.1 && Out.vColor.z <= 1 && Out.vColor.z >= 0.1 && Out.vColor.w <= 1 && Out.vColor.w >= 0.1)
	{
		Out.vColor = vector(1, 0.8, 0, 1);
	}
	
	return Out;
}

PS_OUT PS_MAIN4(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	if (Out.vColor.x <= 1 && Out.vColor.x >= 0.1 && Out.vColor.y <= 1 && Out.vColor.y >= 0.1 && Out.vColor.z <= 1 && Out.vColor.z >= 0.1 && Out.vColor.w <= 1 && Out.vColor.w >= 0.1)
	{
		Out.vColor = vector(0.93, 0.93, 0.93, 1);
	}

	return Out;
}

PS_OUT PS_MAIN5(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	float1 a = g_Alpha;

	Out.vColor.w = a;



	return Out;
}
PS_OUT PS_MAIN6(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	if (Out.vColor.x <= 1 && Out.vColor.x >= 0.1 && Out.vColor.y <= 1 && Out.vColor.y >= 0.1 && Out.vColor.z <= 1 && Out.vColor.z >= 0.1 && Out.vColor.w <= 1 && Out.vColor.w >= 0.1)
	{
		Out.vColor = vector(0, 0, 0, 0.8);
	}
	return Out;
}
PS_OUT PS_MAIN7(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	if (Out.vColor.x <= 1 && Out.vColor.x >= 0.1 && Out.vColor.y <= 1 && Out.vColor.y >= 0.1 && Out.vColor.z <= 1 && Out.vColor.z >= 0.1 && Out.vColor.w <= 1 && Out.vColor.w >= 0.1)
	{
		Out.vColor = vector(0, 0, 0, 1);
	}
	return Out;
}


technique Default_Technique
{
		pass Default_Rendering ////////////////////////////////////////////////0
	{
	// RenderState
	ZwriteEnable = false;

	cullmode = none;
	Lighting = false;
	AlphaBlendEnable = false;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
}


pass Opacity////////////////////////////////////////////////1
{
	// RenderState
	ZwriteEnable = false;
	cullmode = none;
	Lighting = false;
	AlphaBlendEnable = TRUE;
	SrcBlend = SrcAlpha;
	DestBlend = InvSrcAlpha;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
}
	pass Additive////////////////////////////////////////////////2
{
	// RenderState
	Lighting = false;
	AlphaBlendEnable = TRUE;
	SrcBlend = SrcAlpha;
	DestBlend = One;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
}

	pass Red////////////////////////////////////////////////3
{
	// RenderState
	ZwriteEnable = false;
cullmode = none;
Lighting = false;
AlphaBlendEnable = TRUE;
SrcBlend = SrcAlpha;
DestBlend = InvSrcAlpha;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN1();
}

pass Menu////////////////////////////////////////////////4
{
	// RenderState
	ZwriteEnable = true;
cullmode = none;
Lighting = false;
AlphaBlendEnable = TRUE;
SrcBlend = SrcAlpha;
DestBlend = InvSrcAlpha;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN2();
}

	pass yellow////////////////////////////////////////////////5
{
	// RenderState
	ZwriteEnable = true;
cullmode = none;
Lighting = false;
AlphaBlendEnable = TRUE;
SrcBlend = SrcAlpha;
DestBlend = InvSrcAlpha;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN3();
}

	pass gray////////////////////////////////////////////////6
{
	// RenderState
	ZwriteEnable = true;
cullmode = none;
Lighting = false;
AlphaBlendEnable = TRUE;
SrcBlend = SrcAlpha;
DestBlend = InvSrcAlpha;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN4();
}


	pass UIAlpha////////////////////////////////////////////////7
{
	// RenderState
	ZwriteEnable = false;
cullmode = none;
Lighting = false;
AlphaBlendEnable = TRUE;
SrcBlend = SrcAlpha;
DestBlend = InvSrcAlpha;

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN5();
}

	pass SpeechBubble////////////////////////////////////////////////8
{
	// RenderState
	ZwriteEnable = true;
	cullmode = none;
	Lighting = false;
	AlphaBlendEnable = TRUE;
	SrcBlend = SrcAlpha;
	DestBlend = InvSrcAlpha;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN6();
}

	pass Black////////////////////////////////////////////////9
{
	// RenderState
	ZwriteEnable = false;
	cullmode = none;
	Lighting = false;
	AlphaBlendEnable = TRUE;
	SrcBlend = SrcAlpha;
	DestBlend = InvSrcAlpha;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN7();
}

}