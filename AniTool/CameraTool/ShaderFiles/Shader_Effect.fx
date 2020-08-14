

// ������Ʈ���̺�.(��������� ����.)
matrix			g_matWorld, g_matView, g_matProj;

texture			g_DiffuseTexture; // �ȼ����� �ݻ��ؾ��ϴ� ���� ������ �޸�. 

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


// VertexShader : 
// 1. ������ ��ġ������ ���� ��ȯ(���������) �� ����Ѵ�.
// 2. ���� ����ü�� ����� �����Ӱ� ������ �� �ִ�. 

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;
	
	// ���ý����̽��� �ִ� ������ ��ġ�� ���庯ȯ����� ���Ѵ�.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;	
	Out.vProjPos = Out.vPosition;

	return Out;	
}

// ����γ��� ��ġ������ W������ ��ġ������ XYZW�� ������. (2�������� �����ϴ� ��������) In.ProjSpace
// ����Ʈ��ǥ�� ��ȯ�Ѵ�.(������ ���� ��ǥ : 0, 0 -> WinCX, WinCY)
// �����Ͷ���� ����. (�ȼ��� �����Ѵ�.)

struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector	vColor : COLOR;
};

// �ȼ��� ���� �����Ѵ�.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// tex2D : �ؽ����� ������ �����ϰ��ִ� ���÷��κ��� uv��ǥ�� �ش��ϴ� ���� �����Ѵ�.
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	float2	vProjPos = (In.vProjPos.xy / In.vProjPos.w);

	float2	vTexUV;
	vTexUV.x = vProjPos.x * 0.5f + 0.5f;
	vTexUV.y = vProjPos.y * -0.5f + 0.5f;

	vector	vDepthInfo = tex2D(DepthSampler, vTexUV);

	float	fViewZ = In.vProjPos.w;
	float	fOldViewZ = vDepthInfo.y * 300.f;

	float	fAlpha = saturate(fOldViewZ - fViewZ);

	Out.vColor.a = Out.vColor.a * fAlpha;

	return Out;
}


technique Default_Technique
{
	// ���̴� ����� ĸ��ȭ. ((���)���) (����ŧ��)  (�׸���) (������Ʈ) 
	pass Default_Rendering
	{
		// RenderState
		Lighting = false;
		AlphaBlendEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();	
	}

	pass AlphaBlending
	{
		// RenderState
		Lighting = false;
		AlphaTestEnable = false;
		AlphaBlendEnable = TRUE;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	// ���̴� ����� ĸ��ȭ. (���ĺ���) (�ְ�)
	pass AlphaTest
	{
		// RenderState
		Lighting = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 100;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}


