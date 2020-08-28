#include "Shader_BaseRender.hpp"
/////////////////////////////////////////////////////////////////
//ToneMapping

float fExposure = 1.f;
float fGaussianScalar = 0.f;
float3 linearToneMapping(float3 color,float exposure)
{
	//float exposure = 1.f;
	color = clamp(exposure * color, 0.f, 1.f);
	color = pow(abs(color), abs(float3(1.f / gamma, 1.f / gamma, 1.f / gamma)));
	return color;
}

float3 simpleReinhardToneMapping(float3 color, float exposure)
{
	//float exposure = 1.5;
	color *= exposure / (1.f + color / exposure);
	color = pow(abs(color), abs(float3(1.f / gamma, 1.f / gamma, 1.f / gamma)));
	return color;
}

float3 lumaBasedReinhardToneMapping(float3 color, float exposure)
{
	float luma = dot(color, float3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma / (1.f + luma);
	color *= toneMappedLuma / luma;
	color = pow(abs(color), abs(float3(1.f / gamma, 1.f / gamma, 1.f / gamma)));
	return color;
}

float3 whitePreservingLumaBasedReinhardToneMapping(float3 color, float exposure)
{
	float white = 2.f;
	float luma = dot(color, float3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma * (1.f + luma / (white*white)) / (1.f + luma);
	color *= toneMappedLuma / luma;
	color = pow(abs(color),abs (float3(1.f / gamma, 1.f / gamma, 1.f / gamma)));
	return color;
}

float3 RomBinDaHouseToneMapping(float3 color, float exposure)
{
	color = exp(-1.0 / (2.72*color + 0.15));
	color = pow(abs(color), abs(float3(1.f / gamma, 1.f / gamma, 1.f / gamma)));
	return color;
}

float3 filmicToneMapping(float3 color, float exposure)
{
	color = max(float3(0.f, 0.f, 0.f), color - float3(0.004, 0.004, 0.004));
	color = (color * (6.2 * color + 0.5f)) / (color * (6.2 * color + 1.7) + 0.06);
	return color;
}

float3 Uncharted2ToneMapping(float3 color)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	float exposure = 2.f;
	color *= exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
	color = pow(abs(color), abs(float3(1.f / gamma, 1.f / gamma, 1.f / gamma)));
	return color;
}

float3 TM_Reinhard(float3 hdr, float k = 1.0)
{
	return hdr / (hdr + k);
}

float g_fTonMappingA;
float g_fTonMappingB;
float g_fTonMappingC;
float g_fTonMappingD;
float g_fTonMappingE;
float g_fTonMappingF;
// The next generation of filmic tone operators.
float3 ToneMapACES(float3 hdr)
{
	//const float A = 2.51, B = 0.03, C = 2.43, D = 0.59, E = 0.14;
	//return saturate((hdr * (A * hdr + B)) / (hdr * (C * hdr + D) + E));

	const float A = 2.51, B = 0.03, C = 3.5, D = 0.59, E = 0.14,F = 1.f;
	return saturate((hdr * (g_fTonMappingA * hdr + g_fTonMappingB)) / (hdr * (g_fTonMappingC * hdr + g_fTonMappingD) + g_fTonMappingE)* g_fTonMappingF);
}
texture2D	g_RefractionDiffuseTexture;

sampler		RefractionDiffuseSampler = sampler_state
{
	texture = g_RefractionDiffuseTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture2D	g_RefractionTexture;

sampler		RefractionSampler = sampler_state
{
	texture = g_RefractionTexture;
};



texture			g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};

texture			g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_AlphaTexture;

sampler AlphaSampler = sampler_state
{
	texture = g_AlphaTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_BlendTexture;

sampler BlendSampler = sampler_state
{
	texture = g_BlendTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_BlurBlendTexture;

sampler BlurBlendSampler = sampler_state
{
	texture = g_BlurBlendTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};


texture			g_LuminanceTexture;

sampler LuminanceSampler = sampler_state
{
	texture = g_LuminanceTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};


texture			g_BloomTexture;

sampler BloomSampler = sampler_state
{
	texture = g_BloomTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};

texture			g_GodRayTexture;

sampler GodRaySampler = sampler_state
{
	texture = g_GodRayTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};


texture			g_RoughnessTexture;

sampler RoughnessSampler = sampler_state
{
	texture = g_RoughnessTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};

texture			g_MetallicTexture;

sampler MetallicSampler = sampler_state
{
	texture = g_MetallicTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};
texture			g_ForwardAlphaTexture;

sampler ForwardAlphaSampler = sampler_state
{
	texture = g_ForwardAlphaTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};


texture			g_DepthProjectionTexture;

sampler DepthProjectionAlphaSampler = sampler_state
{
	texture = g_DepthProjectionTexture;



};


texture			g_FogDensityTexture;

sampler FogDensitySampler = sampler_state
{
	texture = g_FogDensityTexture;



};
struct PS_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};
PS_OUT PS_BLEND_ALPHA_LIGHT(PS_IN In)
{
	//조명연산은 마지막에 다 섞어주는걸로

	PS_OUT		Out = (PS_OUT)0;

	vector		vAlpha = tex2D(AlphaSampler, In.vTexUV);
	vAlpha.rgb = EncodeLinear(vAlpha.rgb);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);


	//vAlpha.a = 0.2f;
	//vector		vAlpha = tex2D(AlphaSampler, In.vTexUV);

	Out.vColor = vAlpha * vShade;

	return Out;
}
PS_OUT PS_BLEND_FORWARD_ALPHA(PS_IN In)
{
	//조명연산은 마지막에 다 섞어주는걸로

	PS_OUT		Out = (PS_OUT)0;

	vector		vForwardAlpha = tex2D(ForwardAlphaSampler, In.vTexUV);
	//vForwardAlpha.rgb = vector((vForwardAlpha.rgb / (1 + vForwardAlpha.rgb)), 1.f);;

	//vAlpha.a = 0.2f;
	//vector		vAlpha = tex2D(AlphaSampler, In.vTexUV);

	Out.vColor = vForwardAlpha;
	return Out;
}
PS_OUT PS_BLEND_LIGHT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vAlpha = tex2D(AlphaSampler, In.vTexUV);
	//vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	//vAlbedo.rgb = EncodeLinear(vAlbedo.rgb);
	//vAlpha.rgb = EncodeLinear(vAlpha.rgb);
	//vAlpha.a = vDepth.b;
	//알파는 뎁스에 저장해야될지도
	vAlbedo.a = 1.f;
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	///////////////////////////////////////////////////////
	//조명을 나눌 필요가 있다
	//지금 조명한쪽이 안먹게됨
	///////////////////////////////////////////////////////

	Out.vColor =vAlbedo * vShade+ vSpecular;
	//Out.vColor.a -= vSpecular.r;

	//Out.vColor.rgb = EncodeGamma(Out.vColor.rgb);

	Out.vColor.a = 1.f;
	return Out;
}

PS_OUT PS_RenderBackBuffer(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vRefractionDiffuse = tex2D(RefractionDiffuseSampler, In.vTexUV);
	vector	vRefraction = tex2D(RefractionSampler, In.vTexUV);

	vector		vBlend = tex2D(BlendSampler, In.vTexUV + vRefraction.xy) + vRefractionDiffuse;
	vector		vBlurBlend = tex2D(BlurBlendSampler, In.vTexUV + vRefraction.xy)+ vRefractionDiffuse;
	//vector		vBlend = tex2D(BlendSampler, In.vTexUV ) ;	
	//vector		vBlurBlend = tex2D(BlurBlendSampler, In.vTexUV ) ;


	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	vector		vLuminance = tex2D(LuminanceSampler, float2(0.5f, 0.5f));
	vector		vBloom = tex2D(BloomSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector		vGodRay = tex2D(GodRaySampler, In.vTexUV);
	vector		vMotionBlur= tex2D(MotionBlurSampler, In.vTexUV);
	vector		vDepthProjection = tex2D(DepthProjectionAlphaSampler, In.vTexUV);
	vector		vFogDensity = tex2D(FogDensitySampler, In.vTexUV);


	float fFar = 1000.f;
	float fNear = 0.1f;
	float Q = fFar / (fFar - fNear);
	float fPixelZ = (-fNear * Q) / (vDepth.x - Q);

	//vBlend += vSpecular;

	//vector vFinal = lerp(vBlend,vBlurBlend,saturate(0.005f * abs(10.f - fPixelZ)));
	vector vFinal = vBlend;

	//vector vFinal = vBlend;

	//vector vFinal = vBlend + +vSpecular;
	//vector vFinal = lerp(vBlend, vBlurBlend, saturate(pow(abs(10.f - fPixelZ),1000.f)));
	//Out.vColor = vFinal * toneScalar;
	//Out.vColor.a = 1.f;
	//Out.vColor = vBlend;
	//Out.vColor =(vBlend) * vector(simpleReinhardToneMapping( vBlend.rgb, fExposure),vBlend.a)+ vBloom*0.25f;
	//vBlend = vBlend + vSpecular;

	////////////////////////////////////////////////////////
	//모션블러
	int iNumBlurSample = 10;

	//vector		vVelocity = tex2D(VelocitySampler, In.vTexUV);
	//vVelocity.xy *= 0.01f;
	//
	//int icnt = 1;
	//float4 BColor;
	//
	//for (int i = icnt; i < iNumBlurSample; i++)
	//{
	//	BColor = tex2D(BlendSampler, In.vTexUV + vVelocity.xy * (float)i * 2.f);
	//
	//	//if (vVelocity.a < BColor.a + 0.04f)
	//	//{
	//	++icnt;
	//	vBlend += BColor;
	//	//}
	//}
	//
	//vBlend /= (float)(icnt);
	////////////////////////////////////////////////////////


	//float4 vEffect = vBloom + vGodRay;
	//vEffect = vector((vEffect.rgb / (1 + vEffect.rgb)),1.f);
	//Out.vColor = (vector((vBlend.rgb / (1 + vBlend.rgb)), vBlend.a) + vEffect) ;
	//Out.vColor = vBlend * vDepthProjection  + vEffect + vSpecular;


	//톤매핑 적용하면 음영이 사라지고 안하면 흰색이 너무 강해짐

	vFinal = (vFinal * vDepthProjection + vMotionBlur);


	float4 vEffect = vBloom + vGodRay;

	//1
	vEffect = vector((vEffect.rgb / (1 + vEffect.rgb)), 1.f);
	vFinal = vector((vFinal.rgb / (1 + vFinal.rgb)), 1.f);
	//Out.vColor = vector(ToneMapACES(vFinal.xyz), vFinal.a) + vEffect;

	//fog

	Out.vColor = vFinal ;
	Out.vColor.xyz = lerp(vFinal, vFogDensity.xyz, vFogDensity.a) + vEffect;
	//Out.vColor.xyz = lerp(vFogDensity.xyz, Out.vColor.xyz, vFogDensity.a);

	//2
	//vFinal += vEffect;
	//Out.vColor = vector((vFinal.rgb / (1 + vFinal.rgb)), 1.f);

	//Out.vColor = vFinal;
	//Out.vColor = vFinal + vBloom;
	//Out.vColor.xyz = simpleReinhardToneMapping(vFinal.xyz,fExposure) + vEffect;



	Out.vColor.a = 1.f;

	return Out;
}

PS_OUT PS_RenderBackBuffer_DebugShader(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vBlend = tex2D(BlendSampler, In.vTexUV) ;
	vector		vLuminance = tex2D(LuminanceSampler, float2(0.5f, 0.5f));
	vector		vBloom = tex2D(BloomSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector		vGodRay = tex2D(GodRaySampler, In.vTexUV);
	vector		vMotionBlur = tex2D(MotionBlurSampler, In.vTexUV);
	vector		vDepthProjection = tex2D(DepthProjectionAlphaSampler, In.vTexUV);
	//Out.vColor = vFinal * toneScalar;
	//Out.vColor.a = 1.f;
	//Out.vColor = vBlend;
	//Out.vColor =(vBlend) * vector(simpleReinhardToneMapping( vBlend.rgb, fExposure),vBlend.a)+ vBloom*0.25f;
	//vBlend = vBlend + vSpecular;
	vBlend = (vBlend * vDepthProjection + vMotionBlur ) + vSpecular;

	float4 vEffect = vBloom + vGodRay;

	vEffect = vector(ToneMapACES(vEffect.xyz), vEffect.a);
	Out.vColor = vector(ToneMapACES(vBlend.xyz), vBlend.a) + vEffect ;
	//Out.vColor = (vector((vBlend.rgb / (1 + vBlend.rgb)), vBlend.a) + vEffect);
	//Out.vColor = vBlend + vBloom;

	//Out.vColor.xyz = lumaBasedReinhardToneMapping(vBlend.xyz, 1.f) + vBloom.xyz;

	if (In.vTexUV.x < 0.5f)
		Out.vColor = vAlbedo;
	Out.vColor.a = 1.f;
	return Out;
}
PS_OUT PS_DRAW_ONLY(PS_IN In)
{
	//조명연산은 마지막에 다 섞어주는걸로

	PS_OUT		Out = (PS_OUT)0;

	vector		vTexture = tex2D(BaseSampler, In.vTexUV);

	//vAlpha.a = 0.2f;
	//vector		vAlpha = tex2D(AlphaSampler, In.vTexUV);

	Out.vColor = vTexture;
	return Out;
}
technique Default_Device
{
	pass Blend
	{
	alphablendenable = false;
srcblend = one;
destblend = one;

	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_BLEND_LIGHT();
	}

	pass Blend_Alpha
	{
		alphablendenable = false;
	srcblend = srcalpha;
	destblend = one;
	//blendop = multiply;
	//srcblend = srcalpha;
	//destblend = invsrcalpha;

		zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_BLEND_ALPHA_LIGHT();
	}

		pass RenderBackBuffer
	{
		alphablendenable = false;
	srcblend = srcalpha;
	destblend = srccolor;

		zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_RenderBackBuffer();
	}

		pass Draw_Only 
	{
		alphablendenable = false;

	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_DRAW_ONLY();
	}

		pass Blend_ForwardAlpha
	{
		alphablendenable = true;
	//srcblend = one;
	//destblend = one;
	//srcblend = srcalpha;
	//destblend = invsrcalpha;
	srcblend = one;
	destblend = one;

	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_BLEND_FORWARD_ALPHA();
	}

		pass Blend_Alpha
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;

	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_DRAW_ONLY();
	}

		pass RenderBackBuffer_DebugShader
	{
		alphablendenable = false;
	srcblend = srcalpha;
	destblend = srccolor;

	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_RenderBackBuffer_DebugShader();
	}

		pass Blend_Only
	{
		alphablendenable = true;
	srcblend = destalpha;
	destblend = invsrcalpha;


	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_DRAW_ONLY();
	}
}