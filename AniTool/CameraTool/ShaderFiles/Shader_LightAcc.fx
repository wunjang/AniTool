
vector		g_vLightDir;
vector		g_vLightPos;
float		g_fRange;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector		g_vCamPosition;
matrix		g_matProjInv;
matrix		g_matViewInv;

texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;	
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

struct PS_IN
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vShade : COLOR0;
	vector vSpecular : COLOR1;
};

PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector	vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector	vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	float	fViewZ = vDepthInfo.g * 300.0f;

	// (0 -> -1), (1 -> 1)
	vector	vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	float	fShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal));
	Out.vShade = (g_vLightDiffuse * g_vMtrlDiffuse) * (fShade + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade.a = 1.f;

	vector	vReflect = reflect(normalize(g_vLightDir), vNormal);

	vector	vWorldPos;

	// 로컬 * 월드 * 뷰 * 투영
	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthInfo.r * fViewZ;
	vWorldPos.w = 1.0f * fViewZ;

	// 로컬 * 월드 * 뷰
	vWorldPos = mul(vWorldPos, g_matProjInv);

	// 로컬 * 월드
	vWorldPos = mul(vWorldPos, g_matViewInv);

	vector	vLook = vWorldPos - g_vCamPosition;

	float	fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, vReflect)), 30.0f);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
	Out.vSpecular.a = 0.f;

	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector	vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector	vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	float	fViewZ = vDepthInfo.g * 300.0f;

	// (0 -> -1), (1 -> 1)
	vector	vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	vector	vWorldPos;

	// 로컬 * 월드 * 뷰 * 투영
	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthInfo.r * fViewZ;
	vWorldPos.w = 1.0f * fViewZ;

	// 로컬 * 월드 * 뷰
	vWorldPos = mul(vWorldPos, g_matProjInv);

	// 로컬 * 월드
	vWorldPos = mul(vWorldPos, g_matViewInv);

	vector	vLightDir = vWorldPos - g_vLightPos;

	float	fAtt = 0.f;
	float	fLength = length(vLightDir);

	fAtt = saturate((g_fRange - fLength) / g_fRange);	

	float	fShade = saturate(dot(normalize(vLightDir) * -1.f, vNormal)) * fAtt;
	Out.vShade = (g_vLightDiffuse * g_vMtrlDiffuse) * (fShade + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade.a = 1.f;

	vector	vReflect = reflect(normalize(vLightDir), vNormal);

	vector	vLook = vWorldPos - g_vCamPosition;

	float	fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, vReflect)), 30.0f) * fAtt;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
	Out.vSpecular.a = 0.f;

	return Out;
}


technique Default_Technique
{
	pass Directional
	{
		ZWriteEnable = false;

		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Point
	{
		ZWriteEnable = false;

		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}
}