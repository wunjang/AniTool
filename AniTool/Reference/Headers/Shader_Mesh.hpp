#include "Shader_BaseRender.hpp"
#include "Shader_TextureAnim.hpp"
float4 g_RimLightColor;
float4 g_BloomPower;
float g_fAlpha = 0.f;
//////////////////////////////////////////////
//R : Albedo
//G : Normal
//B : ��������
//A : 1�� �־���
float4 g_vDistiortionTag= float4(0.f,0.f,0.f,1.f);
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


	float3 m_T : TEXCOORD2;
	float3 m_B : TEXCOORD3;
	float3 m_N : TEXCOORD4;

	float		fRimLight : TEXCOORD5;
};


VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.m_N = normalize(worldNormal);  //< ���� ���������� �븻�� ���ϱ�

	float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	Out.m_T = normalize(worldTangent);  //< ���� ���������� ź��Ʈ�� ���ϱ�

	float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.m_B = normalize(worldBinormal);  //< ���� ���������� ���̳븻�� ���ϱ�

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	float3 vCameraDir = normalize(g_matView[3].xyz - (mul(vector(In.vPosition.xyz, 1.f), matWV), 1.f));

	Out.fRimLight = smoothstep(0.8f, 1.f, 1 - max(0, dot(mul(worldNormal, (float3x3)g_matView), vCameraDir) - 0.3f));
	return Out;
}

struct	PS_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;

	float3 m_T : TEXCOORD2;
	float3 m_B : TEXCOORD3;
	float3 m_N : TEXCOORD4;

	float		fRimLight : TEXCOORD5;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
	vector		vEmissive : COLOR3;
};

// �ȼ� ���̴�

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = tex2D(BaseSampler,In.vTexUV);
	//Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D �ؽ�ó�κ��� UV���� �����ϴ� �Լ� ,��ȯŸ���� VECTOR Ÿ��
	Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	//Out.vColor.rgb = DecodeRGB16(Out.vColor);
	Out.vEmissive = tex2D(EmissiveSampler, In.vTexUV);
	Out.vEmissive.rgb = EncodeLinear(Out.vEmissive.rgb);
	Out.vNormal = vector(In.m_N, 1.f);
	//����ŧ�� ���� ����
	Out.vNormal.a = 0.f;

	float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
	float fMetallic =  tex2D(MetallicSampler, In.vTexUV).r;
	//Out.vNormal = tex2D(NormalSampler, In.vTexUV);
	//Out.vColor.a =  fMetallic;
	//Out.vNormal.a = fRoughness;


	//Out.vNormal = mul(Out.vNormal, g_matView);
	//Out.vNormal = normalize(Out.vNormal * 2 - 1); //< rgb�� xyz �������� ��ȯ
												  //< �������� -> ����������� ��ȯ
												  //float3x3 TBN = float3x3(normalize(In.m_T), normalize(In.m_B), normalize(In.m_N)); //< ��ȯ��� ����� : ����->����
												  //TBN = transpose(TBN); //< ������� ���ؼ� ����->����
												  //float3 worldNormal = mul(TBN, Out.vNormal); //< ������ ����
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.z, g_fAlpha, In.vProjPos.w * 0.001f);
	Out.vColor.a = 1.f;
	return Out;
}

float g_fSpeed=10.f;
float g_fWaveHeight=100.f;
float g_fWaveFrequency=0.5f;


VS_OUT	VS_WATER_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	//float cosTime = cos(g_fTime * g_fSpeed + In.vTexUV.x * g_fWaveFrequency)*g_fWaveHeight;
	//float cosTime = cos(g_fTime * g_fSpeed + In.vTexUV.x * g_fWaveFrequency);

	//In.vPosition.y += cosTime;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.m_N = normalize(worldNormal);  //< ���� ���������� �븻�� ���ϱ�

	float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	Out.m_T = normalize(worldTangent);  //< ���� ���������� ź��Ʈ�� ���ϱ�

	float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.m_B = normalize(worldBinormal);  //< ���� ���������� ���̳븻�� ���ϱ�

	Out.vProjPos = Out.vPosition;

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	return Out;
}

static float g_fMetallic = 0.f;
static float g_fRouphness = 0.f;
// �ȼ� ���̴�
struct PS_WATER_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
};
PS_WATER_OUT PS_WATER_MAIN(PS_IN In)
{
	//***************************���� ������ ������*******************************//


	PS_WATER_OUT		Out = (PS_WATER_OUT)0;
	float fDistortion = tex2D(DistortionSampler,In.vTexUV ).r * (g_fDistiortionPower);

	Out.vColor = float4(0.f, 0.05f, 0.1f,1.f);
	//Out.vColor = tex2Dlod(BaseSampler, float4(In.vTexUV,0, g_iMipLevel));		// 2D �ؽ�ó�κ��� UV���� �����ϴ� �Լ� ,��ȯŸ���� VECTOR Ÿ��
	Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	//Out.vColor.rgb = DecodeRGB16(Out.vColor);
	//Out.vNormal = vector(In.m_N, 1.f);
	//float fRoughness = tex2D(RoughnessSampler, In.vTexUV).r;
	//float fMetallic =  tex2D(MetallicSampler, In.vTexUV).r;
	Out.vNormal = tex2D(NormalSampler, In.vTexUV + fDistortion + float2(g_fTime * g_vUVSpeed));
	Out.vNormal = vector(Out.vNormal.xyz * 2.f - 1.f, Out.vNormal.a)*3.f;
	//�ȼ���ġ�� ã�´�
	vector vPosition = In.vProjPos;
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * vPosition.w;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * vPosition.w;


	vector vCamPos = g_matView[3].xyzw;
	//vector vCamPos = vector(50.f,45.f,40.f,1.f);
	vector	vLook = normalize(vCamPos - vPosition);

	vector	vReflect = reflect(normalize(vLook), Out.vNormal);

	vector vLight = (pow(abs(saturate(dot(vLook, vReflect))), 5.f)  * 1.f);
	//Out.vColor.a = (vLight.r + vLight.g + vLight.b + vLight.a) *0.25f;
	Out.vColor.rgb *= vLight.rgb ;
	Out.vColor.a = vLight.r;
	//Out.vColor.a = saturate(dot(normalize(vLook) * -1.f, Out.vNormal)).r;
	//Out.vColor.a = 1.f;
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

// �ȼ� ���̴�

PS_2D_OUT PS_2D_MAIN(PS_2D_IN In)
{
	//2D�� ������

	PS_2D_OUT		Out = (PS_2D_OUT)0;
	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.rgb = EncodeLinear(Out.vColor.rgb);
	Out.vColor.a = 1.f;
	return Out;
}


struct PS_REMAIN_OUT
{
	vector		vBloom: COLOR0;
	vector		vDistortion : COLOR1;

};
PS_REMAIN_OUT PS_POSTPROCESS(PS_IN In)
{
	PS_REMAIN_OUT		Out = (PS_REMAIN_OUT)0;


	Out.vBloom = float4(tex2D(BaseSampler, In.vTexUV) * g_BloomPower);
	Out.vDistortion = float4(tex2D(DistortionSampler, In.vTexUV) * g_vDistiortionTag);
	Out.vDistortion.a = 1.f;
	//vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	//float	fViewZ = vDepth.a * 1000.f;
	//
	//vector vPosition; // �ؽ��� ��ǥ���� ���� ��ǥ�� ��ȯ
	//				  //depth���� ������� ���������� �ٲ��ش�
	//vPosition.z = vDepth.x * fViewZ;
	//vPosition.w = fViewZ;
	//
	//if (fViewZ > In.vProjPos.w-1000.f)
	//{
	//	Out.vBloom = float4(tex2D(BaseSampler, In.vTexUV) * g_BloomPower);
	//	Out.vDistortion = float4(tex2D(DistortionSampler, In.vTexUV) * g_fDistiortionPower);
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	vColor = tex2D(BaseSampler, abs(In.vTexUV + float2(g_vOffset[i].x, g_vOffset[i].y)));
	//	Out.vBloom += vColor;
	//}
	//Out.vBloom /= 4.f;
	//Out.vBloom *= g_BloomPower;
	return Out;
}

technique		Default_Device
{
	// ����� ĸ��ȭ
	pass	DefualtRander
	{
		alphablendenable = false;
		//cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
		
	}

	pass	PostProcess
	{
		alphablendenable = false;
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
		//cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();

	}

		pass	AdditiveBlend
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = One;
	//cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();

	}

		pass	Render_Water
	{
		alphablendenable = true;
	//srcblend = srcalpha;
	//destblend = invsrcalpha;
	srcblend = srcalpha;
	destblend = one;
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
};

extern float4x4 MatrixPalette[60];

//�� ������ ������ ��ġ�� �ִ� �� ��
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
	Out.m_N = normalize(worldNormal);  //< ���� ���������� �븻�� ���ϱ�

	float3 worldTangent = mul(In.m_Tangent, (float3x3)g_matWorld);
	Out.m_T = normalize(worldTangent);  //< ���� ���������� ź��Ʈ�� ���ϱ�

	float3 worldBinormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.m_B = normalize(worldBinormal);  //< ���� ���������� ���̳븻�� ���ϱ�

	Out.vPosition = vector(mul(p, matWVP));
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	float3 vCameraDir = normalize(g_matView[3].xyz - (mul(p, matWV), 1.f));

	Out.fRimLight = smoothstep(0.8f, 1.f, 1 - max(0, dot(mul(worldNormal, (float3x3)g_matView), vCameraDir) - 0.3f));
	return Out;
}

technique		Hardware_Skin
{
	// ����� ĸ��ȭ
	pass	DefualtRander
	{
		vertexshader = compile vs_3_0 VS_HARDWARE_SKIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};