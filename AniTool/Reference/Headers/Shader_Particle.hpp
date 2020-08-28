#include "Shader_BaseRender.hpp"
#include "Shader_TextureAnim.hpp"

vector g_vCamPos;
struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float2 vTexUVSize : TEXCOORD1;
	float4 vColor : COLOR0;

	float fSize : PSIZE;

};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float2 vConvertTexUV : COLOR3;
	float4 vProjPos : COLOR2;
	float2 vConvertTexUVSize : COLOR1;
	float4 vColor : COLOR0;

	float fSize : PSIZE;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	//matWVP = mul(matWV, g_matProj);

	vector vViewPos = mul(vector(In.vPosition.xyz,1.f), matWV);
	//Out.fSize = 1.f / sqrt(vViewPos.x*vViewPos.x+ vViewPos.y*vViewPos.y+ vViewPos.z*vViewPos.z);
	//Out.fSize = 1.f;

	Out.vPosition = mul(vViewPos, g_matProj); 
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	Out.vConvertTexUV = In.vTexUV;
	Out.vConvertTexUVSize = In.vTexUVSize;
	Out.vColor = In.vColor;
	Out.fSize = (1000.f) * sqrt(1.f / pow(Out.vPosition.w, 2.f)) * In.fSize;
	//Out.fSize = 10.f;
	return Out;
}

struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float2 vConvertTexUV : COLOR3;
	float4 vProjPos : COLOR2;
	float2 vConvertTexUVSize : COLOR1;
	float4 vColor : COLOR0;

};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	////////////////////////////////////파티클을 포워드 ////////////////////////////////////////
	PS_OUT		Out = (PS_OUT)0;

	float2 vUV = In.vTexUV * g_vUVOffset + In.vConvertTexUV * g_vUVOffset/* + In.vConvertTexUVSize * g_vUVOffset*/;
	vector vMask = tex2D(MaskSampler, In.vTexUV) * g_vMaskTag + vector(g_fUsingMaskTag, g_fUsingMaskTag, g_fUsingMaskTag, g_fUsingMaskTag);
	float fDistortion = tex2D(DistortionSampler, In.vTexUV).r * g_fDistiortionTag;
	//vUV.x += In.vConvertTexUV.x * g_vUVOffset;
	Out.vColor =( tex2D(AlbedoSampler, vUV + fDistortion) + g_vAmbient) *vMask * saturate(In.vColor) * g_vDiffuseColor;
	//Out.vColor *= saturate(In.vColor);
	Out.vColor.xyz = EncodeLinear(Out.vColor.xyz);

	//vector vPosition =mul(mul( In.vProjPos,g_matProjInv),g_matViewInv);

	//vector vLightDir = -vPosition /*- g_vCamPos*/;
	vector vLightDir = vector(0.f,0.f,-1.f,0.f);

	vLightDir.a = 0.f;
	vector vNormal = vector(tex2D(NormalSampler, vUV + fDistortion).rgb,0.f);

	//Out.vNormal.xy = Out.vNormal.wy * 2 - 1;
	vNormal.z = sqrt(1 - vNormal.x*vNormal.x - vNormal.y*vNormal.y);
	vNormal = vector(normalize(vNormal.xyz),0.f);
	//Out.vNormal = normalize(vector(Out.vNormal.xyz * 2.f - 1.f, 0.f));


	//vNormal.xyz = normalize(vNormal.xyz);
	//하프렘버트 적용
	Out.vColor.rgb *= (dot(normalize(vLightDir) * -1.f, vNormal) + 1.f) * 0.5f/**0.5f + 0.5f*/;//그림자 음영을 줄이기 위해 0.5더하고 saturate해줌
	Out.vColor.xyz = EncodeGamma(Out.vColor.xyz);

	//Out.vNormal = vector(Out.vNormal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vNormal.a = 1.f;
	//Forward Lighting

	//In.vProjPos.x = (vUV.x * 2.f - 1.f) * In.vProjPos.w;
	//In.vProjPos.y = (vUV.y * -2.f + 1.f) * In.vProjPos.w;

	//vector vCamPos = g_matView[3].xyzw;
	//vector	vLook = normalize(vCamPos - In.vProjPos);
	//
	//vector	vReflect = reflect(normalize(vLook), Out.vNormal);
	//
	//vector vLight = saturate(dot(normalize(vCamPos) * -1.f, Out.vNormal)) + 0.5f;

	//Out.vColor.rgb *= vLight.rgb;
	//Out.vColor.a = 1.f;

	//float2 vDepthUV;
	//vDepthUV.x = ((In.vProjPos.x + 1.f)  *0.25f);
	//vDepthUV.y = ((In.vProjPos.y + 1.f)  *0.25f);
	//float2 vDownScaleUV = vDepthUV * float2(1280.f, 720.f)/* + In.vConvertTexUV * float2(1.f / 1280.f, 1.f / 720.f)*/;
	
	//vector	vDepth = tex2D(DepthSampler, vUV);
	//float	fViewZ = vDepth.y * 1000.f;
	//vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	//vPosition.x = (vUV.x * 2.f - 1.f) * fViewZ;
	//vPosition.y = (vUV.y * -2.f + 1.f) * fViewZ;
	//vPosition.z = vDepth.x * fViewZ;
	//vPosition.w = fViewZ;
	////
	////Out.vColor.a -= saturate(vPosition.z - In.vProjPos.z);
	//if (vPosition.z < In.vProjPos.z)
	//{
	//	Out.vColor = vector(0.f, 0.f, 0.f, 0.f);
	//}


	return Out;
}

struct PS_POSTPROCESS_OUT
{
	vector		vBloom : COLOR0;
};

PS_POSTPROCESS_OUT PS_POSTPROCESS_PARTICLE(PS_IN In)
{
	PS_POSTPROCESS_OUT		Out = (PS_POSTPROCESS_OUT)0;
	float2 vUV = In.vTexUV * g_vUVOffset*In.vConvertTexUV + In.vConvertTexUVSize * g_vUVOffset;

		//vUV.x += In.vConvertTexUV.x * g_vUVOffset;
		Out.vBloom = float4((tex2D(AlbedoSampler, vUV)+ g_vAmbient) * g_vBloomLevel) * saturate(In.vColor) * g_vDiffuseColor;
		//Out.vDistortion = float4(tex2D(DistortionSampler, vUV)*g_vDistiortionTag);
		//Out.vDistortion.a = 1.f;
	return Out;
}

technique Default_Technique
{
	pass Opacity_Alpha
	{
		// RenderState
		Lighting = false;
	AlphaBlendEnable = TRUE;

	zwriteenable = false;

	SrcBlend = SrcAlpha;
	DestBlend = InvSrcAlpha;
	
	//zwriteenable = false;

	pointspriteenable = true;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Additive_Blend
	{
		// RenderState
		Lighting = false;
	AlphaBlendEnable = TRUE;

	zwriteenable = false;

	srcblend = srcalpha;
	destblend = one;

	//zwriteenable = false;

	pointspriteenable = true;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN();
	}

		pass Opacity_PostProcess
	{
		// RenderState
		Lighting = false;
	alphablendenable = true;
	SrcBlend = SrcAlpha;
	DestBlend = InvSrcAlpha;

	zwriteenable = false;

	pointspriteenable = true;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_POSTPROCESS_PARTICLE();
	}

		pass Additive_PostProcess
	{
		// RenderState
		Lighting = false;
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = One;

	zwriteenable = false;

	pointspriteenable = true;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_POSTPROCESS_PARTICLE();
	}
}


