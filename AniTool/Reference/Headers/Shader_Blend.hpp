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

// The next generation of filmic tone operators.
float3 ToneMapACES(float3 hdr)
{
	const float A = 2.51, B = 0.03, C = 2.43, D = 0.59, E = 0.14;
	return saturate((hdr * (A * hdr + B)) / (hdr * (C * hdr + D) + E));
}

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

	//vAlpha.a = 0.2f;
	//vector		vAlpha = tex2D(AlphaSampler, In.vTexUV);

	Out.vColor = vAlpha;
	return Out;
}

PS_OUT PS_BLEND_LIGHT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vAlpha = tex2D(AlphaSampler, In.vTexUV);
	//vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	vAlbedo.rgb = EncodeLinear(vAlbedo.rgb);
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

	Out.vColor = vAlbedo * vShade + vSpecular * 0.005f;
	//Out.vColor.a -= vSpecular.r;
	//Out.vColor.a = 1.f;
	return Out;
}

PS_OUT PS_RenderBackBuffer(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vBlend = tex2D(BlendSampler, In.vTexUV);
	vector		vLuminance = tex2D(LuminanceSampler, float2(0.5f, 0.5f));
	vector		vBloom = tex2D(BloomSampler, In.vTexUV);

	//vBloom.xyz = EncodeGamma(vBloom.xyz);
	//vBlend.xyz = EncodeGamma(vBlend.xyz);
	//float4 vFinal = vBlend;
	//
	//float Lp = (fExposure / vLuminance.r) * max(vFinal.r, max(vFinal.g, vFinal.b));
	//
	//float LmSqr = (vLuminance.g + fGaussianScalar * vLuminance.g) * (vLuminance.g + fGaussianScalar * vLuminance.g);
	//
	//float toneScalar = (Lp * (1.0f + (Lp / (LmSqr)))) / (1.0f + Lp);
	//
	//Out.vColor = vFinal * toneScalar;
	//Out.vColor.a = 1.f;
	//Out.vColor = vBlend;
	//Out.vColor =(vBlend) * vector(simpleReinhardToneMapping( vBlend.rgb, fExposure),vBlend.a)+ vBloom*0.25f;

	vBloom = vector((vBloom.rgb / (1 + vBloom.rgb)),1.f);
	vBloom.rgb *= 0.25f;
	//Out.vColor = (vector((vBlend.rgb / (1 + vBlend.rgb)), vBlend.a) + vBloom)  ;

	Out.vColor = vector(whitePreservingLumaBasedReinhardToneMapping(vBlend.xyz,fExposure), vBlend.a) + vBloom;
	Out.vColor.xyz = EncodeGamma(Out.vColor.xyz);
	//Out.vColor = (vBlend)+ vBloom*0.25f;
	//Out.vColor *= vector(simpleReinhardToneMapping(Out.vColor.rgb, fExposure), Out.vColor.a);

	return Out;
}

technique Default_Device
{
	pass Blend
	{
	alphablendenable = false;


	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_BLEND_LIGHT();
	}

	pass Blend_Alpha
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = One;
	//srcblend = srcalpha;
	//destblend = invsrcalpha;

		zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_BLEND_ALPHA_LIGHT();
	}

		pass RenderBackBuffer
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

		zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_RenderBackBuffer();
	}
}