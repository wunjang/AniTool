 matrix				g_matWorld;		// ������̺� : ���� �����͸� ����ϴ� ��� ���ڿ�
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
//A �� ����ŧ�� ���� ������ ����

texture		g_NormalTexture;
sampler		NormalSampler = sampler_state
{
	texture = g_NormalTexture;

};
/////////////////////////////////////////////////////
//B : ����
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
Tequnic ������ ���� �ϴ°� ������
��� -> �˺���,�븻,�̹̽ú�
R ������ :NonAlpha�� R
R �Ҽ��� :NonAlpha�� G
G ������ :NonAlpha�� B
G �Ҽ��� :NonAlpha�� A
B ������ :Alpha�� R
B �Ҽ��� :Alpha�� G
A ������ :Alpha�� B
A �Ҽ��� :Alpha�� A

2. ���� ����
��� -> ����ŧ��,���
R ������ :NonAlpha�� ���� R
R �Ҽ��� :NonAlpha�� ���� G
G ������ :NonAlpha�� ���� B
G �Ҽ��� :NonAlpha�� ���� A
B ������ :Alpha�� ���� R
B �Ҽ��� :Alpha�� ���� G
A ������ :Alpha�� ���� B
A �Ҽ��� :Alpha�� ���� A

���� �����ѹ��̸� ���

3.����
���ڵ��� ���� ����� ������Ʈ�� ��ġ��

���ڵ�, ���ڵ����� �غ�����


����
����� -> ���� -> ���̵� -> ����
1. ����� -> ���� �ܰ迡�� vAlphaCheck�� �����־� ����Ĵ� RG�� 1,1 ���Ĵ� BA�� 1,1�� �־��־� ����������

2.���̵� �ܰ迡�� ���ڵ��Ͽ� �ѹ濡 ��� �� ���Ŀ� ����ĸ� ��ġ�°ű��� ������

3. ���� �ܰ迡�� �˺����� ���ڵ��ϰ� �׳� ���ϸ� �ɵ�


���Ĵ� ������ �������̴� �ȳ־��൵ �ɵ�?
�⺻������ ������ ��ġ�� ���� ���� ���ĺ����� �ؾ߰ڴ�
*/
////////////////////////////////////////////////////////////

float2 EncodeColor(float4 fColor)
{
	//modf(x,i) : return�� �Ǽ���, i�� ������
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