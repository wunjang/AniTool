

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


// VertexShader : 
// 1. ������ ��ġ������ ���� ��ȯ(���������) �� ����Ѵ�.
// 2. ���� ����ü�� ����� �����Ӱ� ������ �� �ִ�. 

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;
	
	// ���ý����̽��� �ִ� ������ ��ġ�� ���庯ȯ����� ���Ѵ�.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;	
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);
	Out.vProjPos = Out.vPosition;

	return Out;	
}

// ����γ��� ��ġ������ W������ ��ġ������ XYZW�� ������. (2�������� �����ϴ� ��������) In.ProjSpace
// ����Ʈ��ǥ�� ��ȯ�Ѵ�.(������ ���� ��ǥ : 0, 0 -> WinCX, WinCY)
// �����Ͷ���� ����. (�ȼ��� �����Ѵ�.)

struct PS_IN
{
	float4 vPosition : POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector	vDiffuse : COLOR0;
	vector	vNormal : COLOR1;
	vector	vDepth : COLOR2;
};

// �ȼ��� ���� �����Ѵ�.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// tex2D : �ؽ����� ������ �����ϰ��ִ� ���÷��κ��� uv��ǥ�� �ش��ϴ� ���� �����Ѵ�.
	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	// In.vNormal.xyzw (-1 ~ 1)
	// Out.vNormal.xyzw (0 ~ 1)
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);


	return Out;
}


technique Default_Technique
{
	// ���̴� ����� ĸ��ȭ. ((���)���) (����ŧ��)  (�׸���) (������Ʈ) 
	pass Default_Rendering
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();	
	}
}


