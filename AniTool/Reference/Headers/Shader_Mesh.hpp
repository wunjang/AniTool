#include "Shader_BaseRender.hpp"
#include "Shader_TextureAnim.hpp"

float3 g_vLightPos = float3(0.f,1000.f,0.f);
float3 g_vLightDir = float3(0.f, -0.5f, 0.5f);

float g_fAlphaLevel = 1.f;

float4 g_vRefractColorTag = float4(1.f,1.f,1.f,1.f);

matrix				g_matOldWorld;

float4 g_vMeshColor = (1.f,1.f,1.f,1.f);
float4 g_RimLightColor;
float g_fAlpha = 0.f;

float3 g_vCamPos = (0.f,0.f,0.f);
//////////////////////////////////////////////
//R : Albedo
//G : Normal
//B : 아직없음
//A : 1로 둬야함

//////////////////////////////////////////////

texture2D				g_RoughnessTexture;

sampler		RoughnessSampler = sampler_state
{
	texture = g_RoughnessTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};
texture2D				g_MetallicTexture;

sampler		MetallicSampler = sampler_state
{
	texture = g_MetallicTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};


texture				g_ObejctShadowTexture;

sampler		ObjectShadowSampler = sampler_state
{
	texture = g_ObejctShadowTexture;


AddressU = clamp;
AddressV = clamp;

};


texture				g_ShadowTexture;

sampler		ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;


AddressU = clamp;
AddressV = clamp;

};
////////////////////////////////////////////////

////////////////////////////////////////////////

struct VS_IN
{
	vector		vPosition	: POSITION;		// semantic
	float3		m_Normal : NORMAL;
	float3		m_Tangent	: TANGENT;
	float3		m_Binormal	: BINORMAL;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos	: TEXCOORD1;
	float3		m_Normal : NORMAL;

	float3 m_T : TEXCOORD2;
	float3 m_B : TEXCOORD3;
	float3 m_N : TEXCOORD4;

	float		fRimLight : TEXCOORD5;

	float4		vLightPos : TEXCOORD6;
};


VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.m_N = normalize(worldNormal);  //< 월드 공간에서의 노말값 구하기

	float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	Out.m_T = normalize(worldTangent);  //< 월드 공간에서의 탄젠트값 구하기

	float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.m_B = normalize(worldBinormal);  //< 월드 공간에서의 바이노말값 구하기

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	float3 vCameraDir = normalize(g_vCamPos - (mul(vector(In.vPosition.xyz, 1.f), g_matWorld))).xyz;

	//Out.fRimLight = smoothstep(0.85f, 1.f, 1 - dot(normalize(mul(worldNormal, (float3x3)g_matView)), vCameraDir) - 0.f);
	//Out.fRimLight = 1.f - abs(dot(worldNormal,vCameraDir));

	//if (Out.fRimLight > 0.3)
	//	Out.fRimLight = 1;
	//else
	//	Out.fRimLight = -1;
	//Test
	Out.m_Normal = normalize(mul(vector(In.m_Normal.xyz, 0.f), g_matWorld));

	////////////////////////////
	//Shadow Map
	Out.vLightPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vLightPos = mul(Out.vLightPos, g_matLightView);
	Out.vLightPos = mul(Out.vLightPos, g_matLightProj);

	return Out;
}

struct	PS_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;

	float3		m_Normal : NORMAL;

	float3 m_T : TEXCOORD2;
	float3 m_B : TEXCOORD3;
	float3 m_N : TEXCOORD4;

	float		fRimLight : TEXCOORD5;

	float4 vLightPos : TEXCOORD6;

};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		m_Normal : COLOR1;
	vector		vDepth : COLOR2;
	vector		vGodRayMask: COLOR3;
};

// 픽셀 쉐이더

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
	float fMetallic = tex2D(MetallicSampler, In.vTexUV).r;

	Out.vColor = tex2D(BaseSampler,In.vTexUV) * g_vMeshColor;
	//Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	Out.vColor.a = 1.f;
	//Out.vColor.rgb = DecodeRGB16(Out.vColor);
	Out.vGodRayMask = vector(0.f, 0.f, 0.f, 1.f);


	////////////////////////////////////////////////
	//https://darkcatgame.tistory.com/84
	//노말 완성인듯?
	//DXT5nm
	vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV));
	float3 vNormal = vTexNormal.xyz;

	vNormal.xy = vTexNormal.xy * 2 - 1;
	vNormal.z = sqrt(1 - vNormal.x*vNormal.x - vNormal.y*vNormal.y);

	//vNormal.xyz = normalize(float3(vTexNormal.rg, 1) * 2 - 1);
	//vNormal.z = sqrt(1 -saturate(dot(vNormal.xy, vNormal.xy)));
	//Out.m_Normal = vector(normalize(vNormal.xyz), 0.f);
	//vNormal = vector( normalize(vNormal.xyz), 0.f);

	Out.m_Normal.xyz = vNormal.xyz;
	float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	TBN = transpose(TBN);
	Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);

	//vector vConvertNormal = (vector)0;
	//Out.m_Normal.xyz = Out.m_Normal.xyz;
	//Out.m_Normal.xyz = normalize(Out.m_Normal.xyz * 2 - 1);
	//vConvertNormal.xy = vTexNormal.wy * 2 - 1;
	//vConvertNormal.z = sqrt(1 - vTexNormal.x*vTexNormal.x - vTexNormal.y*vTexNormal.y);
	//vConvertNormal = vConvertNormal*2.f - 1.f;
	//Out.m_Normal.xyz =normalize( In.m_T + vConvertNormal.x * In.m_T + vConvertNormal.y * In.m_B);
	//Out.m_Normal.w = 1.f;
	////////////////////////////////////////////////



	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, fMetallic, fRoughness);

	float3 vLightDir = -g_vLightDir;
	float fBias = 0.00095;

	float fLightDepth = 0.f;
	float2 vShadowProjTex;
	//vShadowProjTex.x = (In.vLightPos.x / In.vLightPos.w) *0.5f + 0.5f;
	//vShadowProjTex.y = (-In.vLightPos.y / In.vLightPos.w) *0.5f + 0.5f;

	vShadowProjTex.xy = In.vLightPos.xy / In.vLightPos.w;
	vShadowProjTex.y = -vShadowProjTex.y;
	vShadowProjTex = vShadowProjTex * 0.5f + 0.5f;
	//조명의 범위 내에 있을 경우
	if ((saturate(vShadowProjTex.x) == vShadowProjTex.x) && saturate(vShadowProjTex.y) == vShadowProjTex.y)
	{
		float fShadowMap = tex2D(ShadowSampler, (float2)vShadowProjTex.xy).r;
		float fObjectShadowMap = tex2D(ObjectShadowSampler, (float2)vShadowProjTex.xy).r;

		//vector vShadowMap = tex2D(ShadowSampler, (float2)vShadowProjTex.xy);

		fLightDepth = In.vLightPos.z/ In.vLightPos.w;
		fLightDepth = fLightDepth - fBias;
	
		if (fLightDepth > fShadowMap + 0.000 || fLightDepth > fObjectShadowMap + 0.000)
			Out.vColor *= 0.4f ;
		//Out.vColor = float4(fLightDepth.xxx,1.f);
	}


	return Out;
}


PS_OUT PS_ALPHA_TEST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
	float fMetallic = tex2D(MetallicSampler, In.vTexUV).r;

	Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vMeshColor;
	//Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	Out.vColor.a = 1.f;
	//Out.vColor.rgb = DecodeRGB16(Out.vColor);
	Out.vGodRayMask = vector(0.f, 0.f, 0.f, 1.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, fMetallic, fRoughness);



	//vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV));
	//
	//Out.m_Normal.xy = vTexNormal.wy * 2 - 1;
	//
	//
	//Out.m_Normal.z = sqrt(1 - vTexNormal.x*vTexNormal.x - vTexNormal.y*vTexNormal.y);
	//
	//
	////if ( abs(vTexNormal.x - vTexNormal.y)<=0.15f &&step(vTexNormal.x,0.51f)&& step(0.49f,vTexNormal.x))
	////{
	////	Out.vColor.a = 0.f;
	////	return Out;
	////}
	//
	////Out.m_Normal.xyz = normalize(Out.m_Normal.xyz * 2 - 1);
	//
	//float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	//TBN = transpose(TBN);
	////Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);

	vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV));
	float3 vNormal = vTexNormal.xyz;
	//vNormal.xy = vTexNormal.wy * 2 - 1;
	vNormal.z = sqrt(1 - vNormal.x*vNormal.x - vNormal.y*vNormal.y);

	//vNormal.z = sqrt(1 -saturate(dot(vNormal.xy, vNormal.xy)));
	Out.m_Normal = vector(normalize(vNormal.xyz), 0.f);

	float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	TBN = transpose(TBN);
	Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);


	Out.m_Normal.a = 1.f;
	////////////////////////////////////////////////

	//스펙큘러 적용 강도
	//Out.m_Normal.a = 0.f;
	//Out.m_Normal = vector(Out.m_Normal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vColor.a =  fMetallic;
	//Out.m_Normal.a = fRoughness;

	return Out;
}


struct PS_PBR_OUT
{
	vector		vRoughness : COLOR0;
	vector		vMetallic : COLOR1;
};
PS_PBR_OUT PS_PBR_DEBUG(PS_IN In)
{
	PS_PBR_OUT		Out = (PS_PBR_OUT)0;
	Out.vRoughness = tex2D(RoughnessSampler, In.vTexUV);
	Out.vMetallic = tex2D(MetallicSampler, In.vTexUV);
	//Out.vRoughness = vector(1.f,1.f,1.f,1.f);
	return Out;
}
float g_fSpeed=10.f;
float g_fWaveHeight=100.f;
float g_fWaveFrequency=0.5f;


float2 g_vWaterUVSize = float2(1.f,1.f);
VS_OUT	VS_WATER_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.m_N = normalize(worldNormal);  //< 월드 공간에서의 노말값 구하기

	float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	Out.m_T = normalize(worldTangent);  //< 월드 공간에서의 탄젠트값 구하기

	float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.m_B = normalize(worldBinormal);  //< 월드 공간에서의 바이노말값 구하기


	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV * g_vWaterUVSize ;
	return Out;
}

static float g_fMetallic = 0.f;
static float g_fRouphness = 0.f;


float4 g_vWaterColor = float4(0.05f, 0.05f, 0.05f, 0.02f);
// 픽셀 쉐이더
struct PS_WATER_OUT
{
	vector		vColor : COLOR0;
	vector		m_Normal : COLOR1;
	vector		vDepth : COLOR2;
};
PS_WATER_OUT PS_WATER_MAIN(PS_IN In)
{
	//***************************물은 포워드 렌더링*******************************//


	PS_WATER_OUT		Out = (PS_WATER_OUT)0;
	float fDistortion = tex2D(DistortionSampler,In.vTexUV ).r * (g_fDistiortionPower);

	Out.vColor = g_vWaterColor;
	//Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	//Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	//Out.vColor.rgb = DecodeRGB16(Out.vColor);
	//Out.m_Normal = vector(In.m_N, 1.f);
	//float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
	//float fMetallic =  tex2D(MetallicSampler, In.vTexUV).r;

	//vector vNormal = vector(tex2D(NormalSampler, In.vTexUV + fDistortion + float2(g_fTime * g_vUVSpeed)));
	//vector vNormal = vector(tex2D(NormalSampler, (In.vTexUV + float2(g_fTime * g_vUVSpeed))*fDistortion));

	//New!
	//Out.m_Normal =normalize(mul(vTexNormal,(g_matWorld.xyz,0.f)));
	//Out.m_Normal.xy = vNormal.wy * 2 - 1;
	//Out.m_Normal.z = sqrt(1 - Out.m_Normal.x*Out.m_Normal.x - Out.m_Normal.y*Out.m_Normal.y);
	//
	//float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	//TBN = transpose(TBN);
	////Out.m_Normal.xyz = mul(Out.m_Normal.xyz, TBN);
	//Out.m_Normal.a = 1.f;

	vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV + fDistortion + float2(g_fTime * g_vUVSpeed)));
	float3 vNormal = vTexNormal.xyz;
	//vNormal.xy = vTexNormal.wy * 2 - 1;
	vNormal.z = sqrt(1 - vNormal.x*vNormal.x - vNormal.y*vNormal.y);

	//vNormal.z = sqrt(1 -saturate(dot(vNormal.xy, vNormal.xy)));
	Out.m_Normal = vector(normalize(vNormal.xyz), 0.f);

	float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	TBN = transpose(TBN);
	Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);

	//Out.m_Normal.xyz = (Out.m_Normal.xyz * 2.f) - 1.f;
	//Out.m_Normal = vector(Out.m_Normal.xyz * 2.f - 1.f, Out.m_Normal.a)*3.f;

	//vector vCamPos = g_matView[3].xyzw;
	vector vCamPos = vector(g_vLightPos,1.f);
	vector	vLook = normalize(vCamPos - In.vProjPos);
	
	vector	vReflect = reflect(normalize(vLook), Out.m_Normal);
	
	vector vLight = (pow(abs(saturate(dot(vLook, vReflect))), 25.f)  * 1.f);
	Out.vColor.a = (vLight.r + vLight.g + vLight.b + vLight.a) *0.25f;

	Out.vColor.a = (-Out.vColor.a + 1.f) * 1.f;

	//Out.vColor.rgb *= vLight.rgb ;
	//Out.vColor.a = 0.2f;

	//Out.vColor = saturate(dot(normalize(vLook) * -1.f, Out.m_Normal));
	//Out.vColor.a = 1.f;

	float fMetallic = 0.3f;
	float fRoughness = 0.15f;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, fMetallic, fRoughness);
	return Out;
}

struct	PS_2D_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;

	float3 m_T : TEXCOORD2;
	float3 m_B : TEXCOORD3;
	float3 m_N : TEXCOORD4;

	float		fRimLight : TEXCOORD5;
};

struct PS_2D_OUT
{
	vector		vColor : COLOR0;

};

// 픽셀 쉐이더

PS_2D_OUT PS_2D_MAIN(PS_2D_IN In)
{
	//2D는 포워드

	PS_2D_OUT		Out = (PS_2D_OUT)0;
	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	Out.vColor.a = 1.f;
	return Out;
}


struct PS_POSTPROCESS_OUT
{
	vector		vBloom: COLOR0;
	vector		vGodRay : COLOR1;

};
PS_POSTPROCESS_OUT PS_POSTPROCESS(PS_IN In)
{
	PS_POSTPROCESS_OUT		Out = (PS_POSTPROCESS_OUT)0;

	float4 vMask = float4(tex2D(MaskSampler, In.vTexUV) * g_vMaskTag);

	float4 vBaseColor = tex2D(BaseSampler, In.vTexUV);
	//Distortion 은 막아놓고 블룸에 빛깔 넣어주는데 쓰자
	//Out.vBloom = float4(tex2D(BaseSampler, In.vTexUV + vDistortion.r)) * g_vBloomLevel/* * vMask*/;
	Out.vBloom = vBaseColor * g_vBloomLevel/* * vMask*/;
	Out.vGodRay = vBaseColor* g_vGodRayTag;


	return Out;
}
///////////////////////////////////////////////////////////////////////////////////////
struct VS_VELOCITY_IN
{
	vector		vPosition : POSITION;		// semantic
	float3		m_Normal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_VELOCITY_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		Velocity : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};
VS_VELOCITY_OUT	VS_VELOCITY(VS_VELOCITY_IN In)
{
	VS_VELOCITY_OUT		Out = (VS_VELOCITY_OUT)0;


	matrix	matWV, matVP;
	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matrix	matOldWV;
	matOldWV = mul(g_matOldWorld, g_matView);
	float4 NewPos = mul(In.vPosition, matWV);
	float4 OldPos = mul(In.vPosition, matOldWV);

	float4 Velocity = NewPos - OldPos;
	float3 Normal = normalize(mul(In.m_Normal, matWV));

	bool bFront = 0<dot(Normal, normalize(Velocity.xyz));

	float4 Pos = bFront ? NewPos : OldPos;

	Out.vPosition = mul(Pos, g_matProj);
	Out.vProjPos = Out.vPosition;
	NewPos = mul(NewPos, g_matProj);
	OldPos = mul(OldPos, g_matProj);

	Out.Velocity.xy = (NewPos.xy / NewPos.w - OldPos.xy / OldPos.w) *5.f;
	Out.Velocity.xy = Out.Velocity.xy;
	Out.Velocity.y *= -1.f;
	Out.Velocity.z = 0.5f;
	Out.Velocity.w = Out.vPosition.w;
	Out.vTexUV = In.vTexUV;
	return Out;
}
struct	PS_VELOCITY_IN
{
	float2		vTexUV : TEXCOORD0;
	float4		Velocity : TEXCOORD2;
	float4		vProjPos : TEXCOORD1;

};

struct PS_VELOCITY_OUT
{
	vector		vColor : COLOR0;
	vector		vProjDepth : COLOR1;
};

PS_VELOCITY_OUT PS_VELOCITY(PS_VELOCITY_IN In)
{
	PS_VELOCITY_OUT		Out = (PS_VELOCITY_OUT)0;

	//Out.vColor = In.Velocity;

	//
	//float	fViewZ = vDepth.y * 1000.f;
	//
	//vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	//
	//vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	//vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	//vPosition.z = vDepth.x * fViewZ;
	//vPosition.w = fViewZ;
	//
	//vPosition = mul(vPosition, g_matProjInv);
	//vPosition = mul(vPosition, g_matViewInv);
	//
	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	if (In.vProjPos.z / In.vProjPos.w > vDepth.x )
	{
		Out.vProjDepth = float4(1.f, 1.f, 1.f, 1.f);
	}
	else
		Out.vProjDepth = float4(0.2f, 0.2f, 0.2f, 1.f);

		//Out.vProjDepth = float4(In.vProjPos.z / In.vProjPos.w, 0.f, 0.f, 1.f);
	//Out.vProjDepth = float4(0.5f, 0.5f, 0.5f, 1.f);
	return Out;
}


float4 PS_DEPTH_PROJECTION(PS_IN In) : COLOR0
{
	float4		Out = (float4)0;


	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	//if (In.vProjPos.z / In.vProjPos.w > vDepth.x)
	//{
		Out = float4(0.65f, 0.65f, 0.65f, 1.f);
	//}
	//else
	//Out = float4(1.f, 1.f, 1.f, 1.f);


	return Out;
}


float4 PS_DRAW_ALBEDO(PS_IN In) : COLOR0
{
	return tex2D(BaseSampler, In.vTexUV) * g_vMeshColor;
}
///////////////////////////////////////////////////////////////////////////////////////
technique		Default_Device
{
	// 기능의 캡슐화
	pass	DefualtRander
	{
		alphablendenable = false;
		//cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
		
	}

	pass	PostProcess
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = One;
		//cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_POSTPROCESS();

	}

	pass	OpacityBlend
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	//srcblend = srcalpha;
	//destblend = invsrcalpha;
		//cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();

	}

		pass	AdditiveBlend
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	//cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();

	}

		pass	Render_Water
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	//SrcBlend = SrcAlpha;
	//DestBlend = One;
	zwriteenable = false;
	cullmode = none;
	vertexshader = compile vs_3_0 VS_WATER_MAIN();
	pixelshader = compile ps_3_0 PS_WATER_MAIN();

	}

		pass	Render_2D
	{
		alphablendenable = false;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_2D_MAIN();

	}

		pass	Render_BackGround_Water
	{
		alphablendenable = false;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	//SrcBlend = SrcAlpha;
	//DestBlend = One;
	cullmode = none;
	vertexshader = compile vs_3_0 VS_WATER_MAIN();
	pixelshader = compile ps_3_0 PS_WATER_MAIN();

	}

	pass PBR_Debug_Render
	{
		alphablendenable = false;
	zenable = false;
	zwriteenable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_PBR_DEBUG();
	}

		pass	Alpha_Test
	{
		alphatestenable = true;
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	Alphafunc = greater;
	alpharef = 0xb0;

	//cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_ALPHA_TEST();

	}

		pass	Make_Dark_BackGround
	{
		alphablendenable = true;
	//srcblend = srcalpha;
	//destblend = one;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	//AlphaOp[0] = modulate;
	//cullmode = none;

	//zwriteenable = false;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();

	}

		pass	Velocity
	{
		//cullmode = none;
	zwriteenable = false;
	zenable = false;
	alphablendenable = true;
	//zfunc = greater;

	srcblend = zero;
	destblend = srcalpha;


	//blendop = min;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_VELOCITY();
	}

		pass	DepthProjection
	{
		cullmode = cw;
		zwriteenable = false;
	//zenable = false;
	alphablendenable = false;
	zfunc = greater;

	srcblend = one;
	destblend = zero;

	//zfunc = less;

	//blendop = min;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_DEPTH_PROJECTION();
	}

		pass DrawAlbedo
	{
	alphablendenable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_DRAW_ALBEDO();

	}

		pass DrawAlbedoAlpha
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_DRAW_ALBEDO();

	}

};
struct VS_SHADOW_IN
{
	vector		vPosition : POSITION;		// semantic
	float3		m_Normal : NORMAL;
	float3		m_Tangent : TANGENT;
	float3		m_Binormal : BINORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_SHADOW_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float3		m_Normal : NORMAL;

	float3 m_T : TEXCOORD1;
	float3 m_B : TEXCOORD2;
	float3 m_N : TEXCOORD3;

	float4		vLightPos : TEXCOORD4;
};


VS_SHADOW_OUT	VS_SHADOW(VS_SHADOW_IN In)
{
	VS_SHADOW_OUT		Out = (VS_SHADOW_OUT)0;

	matrix	matWV, matWVP;

	//matWV = mul(g_matWorld, g_matView);
	//matWVP = mul(matWV, g_matProj);
	//
	//
	//float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	//Out.m_N = normalize(worldNormal);  //< 월드 공간에서의 노말값 구하기
	//
	//float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	//Out.m_T = normalize(worldTangent);  //< 월드 공간에서의 탄젠트값 구하기
	//
	//float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	//Out.m_B = normalize(worldBinormal);  //< 월드 공간에서의 바이노말값 구하기

	//Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	//Test
	//Out.m_Normal = normalize(mul(vector(In.m_Normal.xyz, 0.f), g_matWorld));

	////////////////////////////
	//Shadow Map
	Out.vLightPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);


	Out.vLightPos = mul(Out.vLightPos, g_matLightView);
	Out.vLightPos = mul(Out.vLightPos, g_matLightProj);
	Out.vPosition = Out.vLightPos;
	return Out;
}

struct	PS_SHADOW_IN
{
	float2			vTexUV : TEXCOORD0;

	float3 m_T : TEXCOORD1;
	float3 m_B : TEXCOORD2;
	float3 m_N : TEXCOORD3;

	float4 vLightPos : TEXCOORD4;

};

struct PS_SHADOW_OUT
{
	vector		vShadow : COLOR0;
};

// 픽셀 쉐이더

PS_SHADOW_OUT PS_SHADOW(PS_SHADOW_IN In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;
	
	float fLight = In.vLightPos.z / In.vLightPos.w;
	Out.vShadow = vector(fLight, 0.f, 0.f,1.f) ;
	return Out;
}
technique Shadow
{
	pass Render_ShadowMap
	{
	alphablendenable = true;
srcblend = one;
destblend = zero;
blendop = min;


	vertexshader = compile vs_3_0 VS_SHADOW();
	pixelshader = compile ps_3_0 PS_SHADOW();

	}
}



struct PS_REFRACT_OUT
	{
		vector		vColor : COLOR0;
		vector		m_Normal : COLOR1;
		vector		vDepth : COLOR2;
		vector		vRefraction : COLOR3;
	};

	// 픽셀 쉐이더

	PS_REFRACT_OUT PS_Refract(PS_IN In)
	{
		PS_REFRACT_OUT		Out = (PS_REFRACT_OUT)0;
		float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
		float fMetallic = tex2D(MetallicSampler, In.vTexUV).r;

		Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vMeshColor + g_vRefractColorTag;
		//Out.vColor.xyz = Out.vColor.xyz / (Out.vColor.xyz + 1.f);
		//Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
		Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
		Out.vColor.a = 1.f;
		//Out.vColor.rgb = DecodeRGB16(Out.vColor);
		////////////////////////////////////////////////
		//https://darkcatgame.tistory.com/84
		//노말 완성인듯?
		//DXT5nm
		//vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV));
		//
		//Out.m_Normal.xy = vTexNormal.wy * 2 - 1;
		//Out.m_Normal.z = sqrt(1 - vTexNormal.x*vTexNormal.x - vTexNormal.y*vTexNormal.y);
		//
		////Out.m_Normal.xyz = normalize(Out.m_Normal.xyz * 2 - 1);
		//
		//float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
		//TBN = transpose(TBN);
		////Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);
		vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV));
		float3 vNormal = vTexNormal.xyz;
		//vNormal.xy = vTexNormal.wy * 2 - 1;
		vNormal.z = sqrt(1 - vNormal.x*vNormal.x - vNormal.y*vNormal.y);

		//vNormal.z = sqrt(1 -saturate(dot(vNormal.xy, vNormal.xy)));
		Out.m_Normal = vector(normalize(vNormal.xyz), 0.f);

		float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
		TBN = transpose(TBN);
		Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);
		////////////////////////////////////////////////

		Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, fRoughness * 0.25f);

		float3 vLightDir = -g_vLightDir;
		float fBias = 0.0035;

		float fLightDepth = 0.f;
		float2 vShadowProjTex;
		//vShadowProjTex.x = (In.vLightPos.x / In.vLightPos.w) *0.5f + 0.5f;
		//vShadowProjTex.y = (-In.vLightPos.y / In.vLightPos.w) *0.5f + 0.5f;

		vShadowProjTex.xy = In.vLightPos.xy / In.vLightPos.w;
		vShadowProjTex.y = -vShadowProjTex.y;
		vShadowProjTex = vShadowProjTex * 0.5f + 0.5f;
		////조명의 범위 내에 있을 경우
		//if ((saturate(vShadowProjTex.x) == vShadowProjTex.x) && saturate(vShadowProjTex.y) == vShadowProjTex.y)
		//{
		//	float fShadowMap = tex2D(ShadowSampler, (float2)vShadowProjTex.xy).r;
		//	//vector vShadowMap = tex2D(ShadowSampler, (float2)vShadowProjTex.xy);
		//
		//	fLightDepth = In.vLightPos.z / In.vLightPos.w;
		//	fLightDepth = fLightDepth - fBias;
		//
		//	if (fLightDepth > fShadowMap + 0.000)
		//		Out.vColor *= 0.4f;
		//	//Out.vColor = float4(fLightDepth.xxx,1.f);
		//}

		Out.vRefraction = vector(0.f, 0.f, 0.f, 1.f);
		//Out.vRefraction.x = float(In.vProjPos.x/ In.vProjPos.w / 10.f);
		//Out.vRefraction.y = float(-In.vProjPos.y / In.vProjPos.w / 10.f);
		Out.vRefraction.a = 1.f;
		Out.vRefraction.xy = Out.m_Normal.xy * 0.055f;

		return Out;
	}

technique Refraction
{
		pass Refraction
	{
		alphablendenable = false;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_Refract();

	}
}

extern float4x4 MatrixPalette[60];

//한 정점에 영향을 미치는 최대 본 수
extern int NumBoneInfluences = 3;
struct VS_HSKIN_IN
{
	vector		vPosition : POSITION;		// semantic
	float3		m_Normal : NORMAL;
	float3		m_Tangent : TANGENT;
	float3		m_Binormal : BINORMAL;
	float2		vTexUV : TEXCOORD0;



	float4		Weights : BLENDWEIGHT0;
	int4		BoneIndices : BLENDINDICES0;
};

struct VS_HSKIN_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;


	float3 m_T : TEXCOORD2;
	float3 m_B : TEXCOORD3;
	float3 m_N : TEXCOORD4;

	float		fRimLight : TEXCOORD5;
};


VS_HSKIN_OUT	VS_HARDWARE_SKIN(VS_HSKIN_IN In)
{
	VS_HSKIN_OUT		Out = (VS_HSKIN_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	/////////////////////////////
	//Skinning
	float4 p = float4(0.f,0.f,0.f,1.f);
	float3 norm = float3(0.f,0.f,0.f);
	float lastWeight = 0.f;
	int n = NumBoneInfluences - 1;

	for (int i = 0; i < n; ++i)
	{
		lastWeight += In.Weights[i];
		p += In.Weights[i] * mul(In.vPosition, MatrixPalette[In.BoneIndices[i]]);

		norm += In.Weights[i] * mul(normalize(In.m_Normal),MatrixPalette[In.BoneIndices[i]]);
	}

	lastWeight = 1.f - lastWeight;
	p += lastWeight * mul(In.vPosition,MatrixPalette[In.BoneIndices[n]]);

	norm += lastWeight * mul(normalize(In.m_Normal), MatrixPalette[In.BoneIndices[n]]);
	p.w = 1.f;
	/////////////////////////////


	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.m_N = normalize(worldNormal);  //< 월드 공간에서의 노말값 구하기

	float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	Out.m_T = normalize(worldTangent);  //< 월드 공간에서의 탄젠트값 구하기

	float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.m_B = normalize(worldBinormal);  //< 월드 공간에서의 바이노말값 구하기

	Out.vPosition = vector(mul(p, matWVP));
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	float3 vCameraDir = normalize(g_matView[3].xyz - (mul(p, matWV), 1.f));

	Out.fRimLight = smoothstep(0.8f, 1.f, 1 - max(0, dot(mul(worldNormal, (float3x3)g_matView), vCameraDir) - 0.3f));
	return Out;
}

technique		Hardware_Skin
{
	// 기능의 캡슐화
	pass	DefualtRander
	{
		vertexshader = compile vs_3_0 VS_HARDWARE_SKIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};

struct PS_EFFECT_OUT
{
	vector		vColor : COLOR0;
	vector		m_Normal : COLOR1;
	vector		vDepth : COLOR2;

};


VS_OUT	VS_EFFECT_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.m_N = normalize(worldNormal);  //< 월드 공간에서의 노말값 구하기

	float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	Out.m_T = normalize(worldTangent);  //< 월드 공간에서의 탄젠트값 구하기

	float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.m_B = normalize(worldBinormal);  //< 월드 공간에서의 바이노말값 구하기

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV*g_vUVSize + g_vUVStart + float2((float)g_iUVIndexX, (float)g_iUVIndexY)*g_vUVSize + g_vUVSpeed;

	float3 vCameraDir = normalize(g_matView[3].xyz - (mul(vector(In.vPosition.xyz, 1.f), matWV), 1.f));

	Out.fRimLight = smoothstep(0.8f, 1.f, 1 - max(0, dot(mul(worldNormal, (float3x3)g_matView), vCameraDir) - 0.3f));
	return Out;
}

PS_EFFECT_OUT PS_EFFECT_MAIN(PS_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;
	float fDistortion = tex2D(DistortionSampler, In.vTexUV +g_vDistortionTime) * g_fDistiortionTag;
	vector vMask = tex2D(MaskSampler, In.vTexUV ) * g_vMaskTag + vector(g_fUsingMaskTag, g_fUsingMaskTag, g_fUsingMaskTag, g_fUsingMaskTag);
	Out.vColor = ((tex2D(AlbedoSampler, In.vTexUV ) + g_vAmbient) * vMask) * g_vDiffuseColor;
	//Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	//Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	//Out.vColor.rgb = DecodeRGB16(Out.vColor);
	//Out.m_Normal = tex2D(NormalSampler, In.vTexUV + fDistortion);
	//Out.m_Normal.xy = Out.m_Normal.wy * 2 - 1;
	//Out.m_Normal.z = sqrt(1 - Out.m_Normal.x*Out.m_Normal.x - Out.m_Normal.y*Out.m_Normal.y);
	//
	//float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	////TBN = transpose(TBN);
	////Out.m_Normal.xyz = mul(Out.m_Normal.xyz, TBN);
	//Out.m_Normal.a = 1.f;
	vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV + fDistortion));
	float3 vNormal = vTexNormal.xyz;
	//vNormal.xy = vTexNormal.wy * 2 - 1;
	vNormal.z = sqrt(1 - vNormal.x*vNormal.x - vNormal.y*vNormal.y);

	//vNormal.z = sqrt(1 -saturate(dot(vNormal.xy, vNormal.xy)));
	Out.m_Normal = vector(normalize(vNormal.xyz), 0.f);

	float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	TBN = transpose(TBN);
	Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 1.f, 1.f);

	//float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
	//float fMetallic = tex2D(MetallicSampler, In.vTexUV).r;
	//Out.m_Normal = tex2D(NormalSampler, In.vTexUV);
	//Out.vColor.a =  fMetallic;
	//Out.m_Normal.a = fRoughness;
	
	return Out;
}

PS_EFFECT_OUT PS_EFFECT_LUT_MAIN(PS_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;
	float fDistortion = tex2D(DistortionSampler, In.vTexUV) * g_fDistiortionTag;
	vector vMask = tex2D(MaskSampler, In.vTexUV) * g_vMaskTag + vector(g_fUsingMaskTag, g_fUsingMaskTag, g_fUsingMaskTag, g_fUsingMaskTag);
	Out.vColor = (tex2D(LUTSampler, float2(0.f,g_fLUT_Time) + fDistortion)) * vMask;
	////Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	////Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	////Out.vColor.rgb = DecodeRGB16(Out.vColor);
	//Out.m_Normal = tex2D(NormalSampler, In.vTexUV + fDistortion);
	//Out.m_Normal.xy = Out.m_Normal.wy * 2 - 1;
	//Out.m_Normal.z = sqrt(1 - Out.m_Normal.x*Out.m_Normal.x - Out.m_Normal.y*Out.m_Normal.y);
	//
	//float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	////TBN = transpose(TBN);
	////Out.m_Normal.xyz = mul(Out.m_Normal.xyz, TBN);
	vector vTexNormal = vector(tex2D(NormalSampler, In.vTexUV + fDistortion));
	float3 vNormal = vTexNormal.xyz;
	//vNormal.xy = vTexNormal.wy * 2 - 1;
	vNormal.z = sqrt(1 - vNormal.x*vNormal.x - vNormal.y*vNormal.y);

	//vNormal.z = sqrt(1 -saturate(dot(vNormal.xy, vNormal.xy)));
	Out.m_Normal = vector(normalize(vNormal.xyz), 0.f);

	float3x3 TBN = float3x3(In.m_T.xyz, In.m_B.xyz, In.m_N.xyz);
	TBN = transpose(TBN);
	Out.m_Normal.xyz = mul(TBN, Out.m_Normal.xyz);


	Out.m_Normal.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 1.f, 1.f);

	//float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
	//float fMetallic = tex2D(MetallicSampler, In.vTexUV).r;
	//Out.m_Normal = tex2D(NormalSampler, In.vTexUV);
	//Out.vColor.a =  fMetallic;
	//Out.m_Normal.a = fRoughness;

	return Out;
}

float4 PS_EFFECT_ALPHAMASK_MAIN(PS_IN In) : COLOR0
{
	float4		Out = (float4)0;
	Out = float4(0.f, 0.f, 0.f, g_fAlphaLevel);


	return Out;
}

float4 PS_MASK(PS_IN In) : COLOR0
{
	float4 fOut = g_vAmbient;

float4 vMask = tex2D(MaskSampler,In.vTexUV) * g_vMaskTag;

fOut.a = saturate(vMask.r + vMask.g + vMask.b);
return fOut;
}
float4 PS_INV_MASK(PS_IN In) : COLOR0
{
	float4 fOut = g_vAmbient;

float4 vMask = tex2D(MaskSampler,In.vTexUV) * g_vMaskTag;

fOut.a = 1.f - saturate(vMask.r + vMask.g + vMask.b);
return fOut;
}

float4 PS_DRAW_SCREEN(PS_IN In) : COLOR0
{

return g_vAmbient;
}

float4 g_fFireMainColor = {0.7f,0.7f,0.f,1.f};
float4 g_fFireOutsideColor = {0.7f,0.2f,0.f,1.f};
float4 g_fFireMiddleColor = {0.5f,0.4f,0.f,1.f};
PS_EFFECT_OUT PS_FIRE(PS_IN In)
{
	/*
		https://www.febucci.com/2019/05/fire-shader/
		0. 준비물 -> noise,gradient,Mask(Glow효과 연출용)
		1. 노이즈를 읽고 시간에 따른 y상승을 준다
			* 여기서 노이즈 옥타브를 3개 생성해서 따로 연산하는것도 괜찮을듯
		2. step을 이용하여 화염 효과의 범위 #1을 만든다
		step(y,x)  y = Noise.x , x = gradient.x
		3. 불꽃의 끄트머리 #2를 만들기 위해 gradient를 약간 내린 상태로 한번 더 계산
		step(y,x-0.2)
		4. #1과 #2의 색상값을 빼서 끄트머리 색상을 만들어주는 L1을 만든다
		5. 선형 보간을 통해 불꽃 색상 C1 을 만들어준다
		lerp(color1,color2,L1) 겹쳐진 부분일 수록 color2에 가깝게 만들어짐 
		6. 중간 범위#3을 만들어 주기 위해 step 사용
		step(y,x-0.4)
		7.#2와 #3의 색상값을 빼서 중간범위 L2 를 만들고 선형보간
		lerp(C1,Color3,L2)
		8.만들어진 색상을 Mask를 씌워주자
	*/

	PS_EFFECT_OUT Out = (PS_EFFECT_OUT)0;

	vector vNoise = tex2D(NoiseSampler, float2(In.vTexUV.x,In.vTexUV.y + g_fTime));
	vector vGradient = tex2D(GradientSampler, In.vTexUV);
	vector vMask = tex2D(MaskSampler, In.vTexUV);

	float fFireMain = step(vNoise.x,vGradient.a);
	float fFireInterval = step(vNoise.x, vGradient.a - 0.3f);

	float L1 = fFireMain - fFireInterval;
	float4 C1 = lerp(g_fFireMainColor,g_fFireOutsideColor,L1);
	float fFireMiddle = step(vNoise.x, vGradient.x - 0.5f);
	float L2 = fFireInterval - fFireMiddle;
	float4 C2 = lerp(C1, g_fFireMiddleColor, L2);

	Out.vColor = lerp(C1,g_fFireMiddleColor,L2)/* * vMask.a*/;
	//Out.vColor.a = vMask.a;

	//vector vNoise1 = tex2D(NoiseSampler, float2(In.vTexUV.x, In.vTexUV.y + g_fTime));
	//vector vNoise2 = tex2D(NoiseSampler, float2(In.vTexUV.x * 2.f, (In.vTexUV.y + g_fTime)*2.f));
	//vector vNoise3 = tex2D(NoiseSampler, float2(In.vTexUV.x * 3.f, (In.vTexUV.y + g_fTime)*3.f));
	//
	//vNoise1 = (vNoise1 - 0.5f)* 2.f;
	//vNoise2 = (vNoise2 - 0.5f)* 2.f;
	//vNoise3 = (vNoise3 - 0.5f)* 2.f;
	//
	//vNoise1.xy = vNoise1.xy * float2(1.f,1.f);
	//vNoise2.xy = vNoise2.xy * float2(0.5f,1.f);
	//vNoise3.xy = vNoise3.xy * float2(1.f,0.5f);
	//
	//vector vFinalNoise = vNoise1 + vNoise2 + vNoise3;
	//
	//vector vGradient = tex2D(GradientSampler, In.vTexUV);
	//vector vMask = tex2D(MaskSampler, In.vTexUV);
	//
	//float fFireMain = step(vFinalNoise.x, vGradient.a);
	//float fFireInterval = step(vFinalNoise.x, vGradient.a - 0.3f);
	//
	//float L1 = fFireMain - fFireInterval;
	//float4 C1 = lerp(g_fFireMainColor, g_fFireOutsideColor, L1);
	//float fFireMiddle = step(vFinalNoise.x, vGradient.x - 0.5f);
	//float L2 = fFireInterval - fFireMiddle;
	//float4 C2 = lerp(C1, g_fFireMiddleColor, L2);
	//
	//Out.vColor = lerp(C1, g_fFireMiddleColor, L2)/* * vMask.a*/;
	//Out.vColor.a = vMask.a;

	Out.m_Normal.xyz = In.m_N.xyz;
	Out.m_Normal.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 1.f, 1.f);

	return Out;
}
technique		Effect
{
	// 기능의 캡슐화
	pass	NonAlpha
{
	alphablendenable = false;
//zwriteenable = false;
	vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_EFFECT_MAIN();
}


pass	OpacityBlend
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;
//cullmode = none;
//zwriteenable = false;

vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_EFFECT_MAIN();

}

pass	AdditiveBlend
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = One;
//cullmode = none;
//zwriteenable = false;

vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_EFFECT_MAIN();

}

pass	PostProcess
{
	alphablendenable = false;
srcblend = srcalpha;
destblend = One;
//cullmode = none;
zwriteenable = false;

vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_POSTPROCESS();

}


pass	LUT
{
	alphablendenable = false;
zwriteenable = false;

vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_EFFECT_LUT_MAIN();
}

pass	ALPHA
{
	alphablendenable = true;
srcblend = zero;
destblend = srcAlpha;
//srcblend = One;
//destblend = One;
//blendop = min;
zwriteenable = false;
zenable = false;
cullmode = none;

vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_EFFECT_ALPHAMASK_MAIN();
}

pass	MASK
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = zero;
zwriteenable = false;
zenable = false;


vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_MASK();
}
pass	INV_MASK
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = zero;
zwriteenable = false;
zenable = false;


vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_INV_MASK();
}

pass	DRAW_SCREEN
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = zero;
zwriteenable = false;
zenable = false;


//Uv Animation을 따로 해줄거라 VS를 Main으로 했다
vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
pixelshader = compile ps_3_0 PS_DRAW_SCREEN();
}

pass	FIRE
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;



//Uv Animation을 따로 해줄거라 VS를 Main으로 했다
vertexshader = compile vs_3_0 VS_MAIN(); 
pixelshader = compile ps_3_0 PS_FIRE();
}
};



struct VS_SKY_IN
{
	float4		vPosition : POSITION;		// semantic
	float3		vTexUV : TEXCOORD0;
};


struct VS_SKY_OUT
{
	vector		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

// 버텍스 쉐이딩

matrix				g_matCubeWorld;		// 상수테이블 : 실제 데이터를 대신하는 상수 문자열
matrix				g_matCubeView;
matrix				g_matCubeProj;

VS_SKY_OUT	VS_SKY_MAIN(VS_SKY_IN In)
{
	VS_SKY_OUT		Out = (VS_SKY_OUT)0;

	matrix	matWV, matWVP;

	matrix ViewPosition = g_matCubeView;
	ViewPosition[3].xyz = (float3)(0.f, 0.f, 0.f);

	matWV = mul(g_matCubeWorld, ViewPosition);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	return Out;
}
struct	PS_SKY_IN
{
	float3		vTexUV : TEXCOORD0;
};
struct	PS_SKY_OUT
{
	float4		vColor : COLOR0;
};

textureCUBE g_CubeTexture;
samplerCUBE		CubeSampler = sampler_state
{
	texture = g_CubeTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};




// 픽셀 쉐이더
PS_SKY_OUT PS_SKY_MAIN(PS_SKY_IN In)
{
	PS_SKY_OUT		Out = (PS_SKY_OUT)0;

	Out.vColor = texCUBE(CubeSampler, In.vTexUV) * 0.4f;		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	return Out;
}


technique		Cube
{
	pass SkyBox
	{
		alphablendenable = false;

		ZWriteEnable = false;
		ZEnable = false;
		Cullmode = none;
		Lighting = false;
		

		vertexshader = compile vs_3_0 VS_SKY_MAIN();
		pixelshader = compile ps_3_0 PS_SKY_MAIN();
	}
};