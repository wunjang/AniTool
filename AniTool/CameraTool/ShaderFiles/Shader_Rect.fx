

// 컨스턴트테이블.(상수데이터 집합.)
matrix			g_matWorld, g_matView, g_matProj;

texture			g_DiffuseTexture; // 픽셀들이 반사해야하는 색을 저장한 메모리. 

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


// VertexShader : 
// 1. 정점의 위치벡터의 공간 변환(월드뷰투영) 을 담당한다.
// 2. 정점 구조체의 멤버를 자유롭게 변형할 수 있다. 

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
	
	// 로컬스페이스에 있는 정점의 위치에 월드변환행렬을 곱한다.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;	

	return Out;	
}

// 결과로나온 위치벡터의 W값으로 위치벡터의 XYZW를 나눈다. (2차원으로 변형하는 원근투영) In.ProjSpace
// 뷰포트좌표로 변환한다.(윈도우 상의 좌표 : 0, 0 -> WinCX, WinCY)
// 래스터라이즈를 수행. (픽셀을 생성한다.)

struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR;
};

// 픽셀의 색을 결정한다.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// tex2D : 텍스쳐의 정보를 포함하고있는 샘플러로부터 uv좌표에 해당하는 색을 리턴한다.
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	return Out;
}


technique Default_Technique
{
	// 셰이더 기능의 캡슐화. ((노멀)명암) (스펙큘러)  (그림자) (림라이트) 
	pass Default_Rendering
	{
		// RenderState
		Lighting = false;
		AlphaBlendEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();	
	}

	// 셰이더 기능의 캡슐화. (알파블렌딩) (왜곡)
	pass AlphaBlending
	{
		// RenderState
		Lighting = false;
		AlphaBlendEnable = TRUE;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}


