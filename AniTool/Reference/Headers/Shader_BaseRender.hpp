 matrix				g_matWorld;		// 상수테이블 : 실제 데이터를 대신하는 상수 문자열
 matrix				g_matView;
 matrix				g_matProj;

 matrix				g_matLightWorld;
 matrix				g_matLightView;
 matrix				g_matLightProj;



 static matrix		g_matOldView;
 static matrix		g_matOldProj;

 matrix		g_matProjInv;
 matrix		g_matViewInv;

 float gamma = 2.2f;
 float g_fDistiortionPower = 1.f;

 float4 g_vNormalKey = (0.5f, 0.5f, 0.5f, 0.f);
texture2D	g_BaseTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

};

texture2D	g_AlbedoTexture;

sampler		AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture2D	g_MaskTexture;

sampler		MaskSampler = sampler_state
{
	texture = g_MaskTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture2D	g_DisolveTexture;

sampler		DisolveSampler = sampler_state
{
	texture = g_DisolveTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture2D	g_MotionBlurTexture;

sampler		MotionBlurSampler = sampler_state
{
	texture = g_MotionBlurTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};
//A 는 스펙큘러 적용 범위로 하자

texture		g_NormalTexture;
sampler		NormalSampler = sampler_state
{
	texture = g_NormalTexture;

};
/////////////////////////////////////////////////////
//B : 알파
texture2D	g_DepthTexture;
sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;


};
////////////////////////////////////////////////////
texture		g_EmissiveTexture;
sampler		EmissiveSampler = sampler_state
{
	texture = g_EmissiveTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;

};

texture				g_NoiseTexture;

sampler		NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture				g_GradientTexture;

sampler		GradientSampler = sampler_state
{
	texture = g_GradientTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture2D	g_DistortionTexture;
sampler		DistortionSampler = sampler_state
{
	texture = g_DistortionTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture2D	g_SmokeTexture;
sampler		SmokeSampler = sampler_state
{
	texture = g_SmokeTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture2D	g_VelocityTexture;

sampler		VelocitySampler = sampler_state
{
	texture = g_VelocityTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};


float4 EncodeRGB16(in float3 rgb)
{
	float4 vEncoded = 0;

	vEncoded.rgb = rgb / 100.f;

	return vEncoded;
}

float3 DecodeRGB16(in float4 rgbx)
{
	float3 vDecoded;

	vDecoded = rgbx.rgb * 100.f;

	return vDecoded;
}

float3 EncodeGamma(float3 vLinear)
{
	return pow(abs(vLinear),1/ gamma);
}
float3 EncodeLinear(float3 vGamma)
{
	return pow(abs(vGamma), gamma);
}


float4 GetDistortionAlbedo(float2 vUV)
{
	vector		vDistortion = tex2D(DistortionSampler, vUV);
	vector		vAlbedo = tex2D(AlbedoSampler, vUV + vDistortion.r);
	return vector(EncodeLinear(vAlbedo.rgb),vAlbedo.a);
}
float4 GetDistortionNormal(float2 vUV)
{
	vector		vDistortion = tex2D(DistortionSampler, vUV);
	return tex2D(NormalSampler, vUV + vDistortion.g);
}



////////////////////////////////////////////////////////////
/*
Render Sequence
1. Object Render
Tequnic 조정을 통해 하는게 좋을듯
대상 -> 알베도,노말,이미시브
R 정수부 :NonAlpha의 R
R 소수부 :NonAlpha의 G
G 정수부 :NonAlpha의 B
G 소수부 :NonAlpha의 A
B 정수부 :Alpha의 R
B 소수부 :Alpha의 G
A 정수부 :Alpha의 B
A 소수부 :Alpha의 A

2. 조명 연산
대상 -> 스펙큘라,명암
R 정수부 :NonAlpha의 조명 R
R 소수부 :NonAlpha의 조명 G
G 정수부 :NonAlpha의 조명 B
G 소수부 :NonAlpha의 조명 A
B 정수부 :Alpha의 조명 R
B 소수부 :Alpha의 조명 G
A 정수부 :Alpha의 조명 B
A 소수부 :Alpha의 조명 A

조명 연산한번이면 충분

3.블랜드
디코딩후 각자 조명과 오브젝트를 합치자

인코딩, 디코딩으로 준비하자


순서
논알파 -> 알파 -> 쉐이드 -> 블랜드
1. 논알파 -> 알파 단계에서 vAlphaCheck를 곱해주어 논알파는 RG에 1,1 알파는 BA에 1,1을 넣어주어 연산해주자

2.쉐이드 단계에서 디코딩하여 한방에 계산 후 알파와 논알파를 합치는거까지 해주자

3. 블랜드 단계에서 알베도만 디코딩하고 그냥 더하면 될듯


알파는 뎁스가 저장중이니 안넣어줘도 될듯?
기본적으로 색상을 겹치기 위해 전부 알파블랜딩을 해야겠다
*/
////////////////////////////////////////////////////////////

float2 EncodeColor(float4 fColor)
{
	//modf(x,i) : return이 실수부, i가 정수부
	int iClamp;
	int iRed;
	modf(fColor.r*100.f,iRed);
	iRed = clamp(iRed, 0.f, 9999.f);
	float fGreen =modf(fColor.g*0.01f, iClamp);
	fGreen = clamp(fGreen, 0.f,0.9999f);
	int iBlue;
	modf(fColor.b*100.f, iBlue);
	iBlue = clamp(iBlue, 0.f, 9999.f);
	float fAlpha = modf(fColor.a*0.01f, iClamp);
	fAlpha = clamp(fAlpha, 0.f, 0.9999f);
	return float2(iRed + fGreen,iBlue + fAlpha);
}

float4 DecodeColor(float2 fColor)
{
	float4 fDecodedColor = (float4)0;
	fDecodedColor.g = modf(fColor.x, fDecodedColor.r);
	fDecodedColor.a = modf(fColor.y, fDecodedColor.b);
	return fDecodedColor;
}