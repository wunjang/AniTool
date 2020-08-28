
float3 EncodeGamma(float3 vLinear)
{
	return pow(abs(vLinear), 1 / 2.2);
}
float3 EncodeLinear(float3 vGamma)
{
	return pow(abs(vGamma), 2.2);
}


static const float PI = 3.141592;
static const float Epsilon = 0.00001;


matrix				g_matView;
matrix				g_matProj;


 matrix		g_matProjInv;
 matrix		g_matViewInv;

texture2D	g_BaseTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};
texture		g_NormalTexture;

sampler		NormalSampler = sampler_state
{
	texture = g_NormalTexture;

};


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


texture2D	g_FogDensityTexture;

sampler		FogDensitySampler = sampler_state
{
	texture = g_FogDensityTexture;
};
/////////////////////////////////////////////////////
//B : 알파
texture2D	g_DepthTexture;
sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;

};

texture				g_NoiseTexture;

sampler		NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

textureCUBE g_IrradianceTexture;
samplerCUBE		IrradianceSampler = sampler_state
{
	texture = g_IrradianceTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};



texture3D				g_SpecularCube;

sampler		SpecularCubeSampler = sampler_state
{
	texture = g_SpecularCube;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

///////////////////////////////////////////////////////
// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r)*0.125f; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}
// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
//uint querySpecularTextureLevels()
//{
//	uint width, height, levels;
//	g_SpecularCube.GetDimensions(0, width, height, levels);
//	return levels;
//}

///////////////////////////////////////////////////////


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

vector	g_vLightDir;
vector	g_vLightDiffuse;
vector	g_vLightAmbient = {1.f,1.f,1.f,1.f};

vector	g_vMtrlDiffuse = (vector)1.f;
vector	g_vMtrlAmbient = (vector)(1.f,1.f,1.f,1.f);

vector	g_vCamPos;
float	g_fPower;


float	g_fRange;

vector g_vLightPos = (vector)(0.f,1000.f,0.f);
struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	vector		vShadow : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	//////////////////////////////////////////////////////////
	//PBR 실패
	PS_OUT		Out = (PS_OUT)0;
	
	vector	vRefractionDiffuse = tex2D(RefractionDiffuseSampler, In.vTexUV);
	vector	vRefraction = tex2D(RefractionSampler, In.vTexUV);


	vector		vAlbedo = tex2D(BaseSampler,saturate(In.vTexUV + vRefraction.xy)) + vRefractionDiffuse;
	vAlbedo.rgb = EncodeLinear(vAlbedo.rgb) * 1.2f;
	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	vector	vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fShadowDepth = vNormal.a;

	float		fRoughness = vDepth.a;
	float		fMetallic = vDepth.b;
	//최저 조명을 0에서 조정해주는 변수(하프 렘버트지만 하프는 아니게 바꿔놨다)
	float fHalfLambertLevel = 0.5f;

	//fRoughness = saturate(fRoughness) * -1.f + 1.f;
	//fMetallic = saturate(fMetallic) * -1.f + 1.f;
	//vNormal = normalize(vector(vNormal.xyz, 0.f));
	vNormal = vector(normalize(vNormal.xyz),0.f);
	//vNormal = normalize( vector(vNormal.xyz * 2.f - 1.f, 0.f));

	//vector vDirection = vector(g_vLightDir.xyz, 0.f);
	//vector vLight = saturate((dot(normalize(vDirection) * -1.f, vNormal) +1) * fHalfLambertLevel) * (g_vLightDiffuse * g_vMtrlDiffuse) /*+ (g_vLightAmbient * g_vMtrlAmbient)*/;
	////vLight = vLight *0.5f + 0.5f;
	//vLight.a = 1.f;

	//Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//Out.vShade.a = 1.f;
	

	float	fViewZ = vDepth.y * 1000.f;
	
	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;
	
	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);
	////////////////////////////////////////////////////////////////////////////////
	//내가 구현한 조명 쉐이딩(쿡토렌스 + 렘버트)
	//변수 선언
	vector vDirectionLight = (vector)0;
	
	float3 L = -normalize(g_vLightDir.xyz);				//빛의 방향
	float3 N = normalize(vNormal.xyz);					//노말법선
	float3 V = normalize(g_vCamPos - vPosition.xyz);	//시선
	float3 H = normalize(L + V);						//빛 + 시선 하프
	
	float NV = dot(N, V);
	float NH = dot(N, H);
	float VH = dot(V, H);
	float NL = dot(N, L);
	float LH = dot(L, H);

	//하프 렘버트 적용
	//메탈릭할수록 음영이 사라지고 원래의 색상만 남게된다
	//vDirectionLight = saturate((dot(normalize(L), N) + 1) * fHalfLambertLevel) * (g_vLightDiffuse * g_vMtrlDiffuse)+ (g_vLightAmbient * g_vMtrlAmbient);
	vDirectionLight = saturate((dot(normalize(L), N) ) ) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	/////////////////////////////
	/* A. 미세면 분포 함수 D를 구한다
		미세면의 각각의 각도에 관한 시선벡터의 반사 벡터가 광원 벡터로 향하는 비율을 나타낸다
		각 픽셀 단위의 노말을 미세면의 각도라 생각하면 됨
	*/
	//거칠기를 나타낸다
	const float m = fRoughness;
	float NH2 = NH*NH;
	float D = exp(-(1 - NH2) / (NH2*m*m)) / (4 * m*m*NH2*NH2);
	/////////////////////////////
	/* B. 기하감쇠율 G를 구한다
		미세면에 입사된 빛이 다른 미세면에 그림자를 만드는 효과라 한다
		결과가 1에 가까울 수록 본래 반사광을 표현
	*/
	float G = min(1,min(2*NH*NV/VH,2*NH*NL/VH));
	/////////////////////////////
	/* C. 프리넬 값을 구한다
		굴절률에 관한 공식이다
		가까우면 투과율이,멀리있으면 반사율이 높아진다
		쉬릭스 근사치를 사용한다
	*/
	float n = 20.f; // 복소굴절률의 실수부(?)
	float g = sqrt(n*n+LH*LH-1);
	float gpc = g + LH;
	float gnc = g - LH;
	float cgpc = LH * gpc - 1;
	float cgnc = LH*gnc + 1;
	float F = 0.5f * gnc*gnc*(1 + cgpc*cgpc / (cgnc*cgnc)) / (gpc*gpc);

	//float fFdielectric = 0.04f;
	//float3 F0 = lerp(float3(fFdielectric.xxx), float3(1.f, 1.f, 1.f), fMetallic);
	//float3 F = fresnelSchlick(F0, max(0.f, dot(Lh, Lo)));
	/////////////////////////////
	/////////////////////////////

	//림라이트
	//float fRimLight = 0.f;
	//fRimLight = smoothstep(0.5f, 1.f, 1 - dot(normalize(N), vCameraDir) - 0.f);

	float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(1.f, 1.f, 1.f), fMetallic);
	float3 diffuseBRDF = vAlbedo.xyz;
	Out.vSpecular =vector( max(0.001f, F*D*G / NV) * float3(1.f,1.f,1.f)  ,1.f) ;
	//Out.vSpecular = vector(float3(max(0.001f, F*D*G / NV) *fMetallic * vAlbedo.xyz), 1.f);
	float3 directLighting =/* diffuseBRDF **/ vDirectionLight.xyz *kd * diffuseBRDF + Out.vSpecular;
	Out.vSpecular = (vector)0;
	////////////////////////////////////////////////////////////////////////////////
	//PBR
	////float3	Lo = normalize(g_vCamPos.xyz - vPosition.xyz);
	////float3	Lo = normalize(float3(0.f,250.f,-10.f) - vPosition);
	//float3	Lo = normalize(vPosition-g_vCamPos);
	////float3	Lo = normalize(g_vCamPos);

	////float3	Lo = normalize(float3(0.f,0.f,-1.f));

	//float3 N = vNormal.xyz;
	//
	//float cosLo = max(0.f,(dot(N,Lo)));

	//float3 Lr = 2.f*cosLo * N - Lo;
	//
	//float fFdielectric = 0.04f;
	////float fFdielectric = 0.5f;

	//float3 F0 = lerp(float3(fFdielectric.xxx), float3(1.f,1.f,1.f), fMetallic);
	//float3 directLighting;
	//{
	//	float3 vLightDir = -normalize(g_vLightDir.xyz);

	//	float3 Lh = normalize(vLightDir + Lo);
	//	//Half Lambert 적용  (내적값을 0.5~1로 고정시켜 음영 최저치를 없앴다)
	//	//float cosLi = max(0.0, (dot(N, vLightDir)+1)*fHalfLambertLevel);
	//	//float cosLh = max(0.0, (dot(N, Lh)+1)*fHalfLambertLevel);
	//	float cosLi = max(0.0, (dot(N, vLightDir)));
	//	float cosLh = max(0.0, (dot(N, Lh)));

	//	float3 F = fresnelSchlick(F0, max(0.f, dot(Lh, Lo)));
	//	float D = ndfGGX(cosLh, fRoughness);
	//	float G = gaSchlickGGX(cosLi, cosLo, fRoughness);

	//	float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(1.f, 1.f, 1.f), fMetallic);
	//	float3 diffuseBRDF = kd * vAlbedo.xyz;
	//	//float3 diffuseBRDF = kd * 1.f;
	//	//float3 specularBRDF = (F * D * G) / max(0.001f, 4.0f * cosLi * cosLo);
	//	float3 specularBRDF = max(0.001f, (F * D * G) / 4.0f * cosLi * cosLo);

	//	float fRimLight = 0.f;
	//	float3 vCameraDir = normalize(g_vCamPos  - vPosition.xyz);

	//	fRimLight = smoothstep(0.5f, 1.f,1 -  dot(normalize(N), vCameraDir) - 0.f);


	//	vector vDirection = vector(g_vLightDir.xyz, 0.f);
	//	vector vCamLight = saturate((dot(normalize(Lo.xyz) * -1.f, vNormal.xyz)) ) /** (g_vLightDiffuse * g_vMtrlDiffuse) */ * (g_vLightAmbient * g_vMtrlAmbient);
	//	//vLight = vLight *0.5f + 0.5f;
	//	vCamLight.a = 1.f;
	//	//Directinal은 하프렘버트 사용
	//	vector vDirectionLight = saturate((dot(normalize(g_vLightDir) * -1.f, vNormal.xyz) +1) * fHalfLambertLevel) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);


	//	vector	vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);

	//	vector vSpecular = pow(saturate(dot(Lo, vReflect)), 10.f);


	//	//directLighting = (diffuseBRDF * 2.f +(specularBRDF )*5.f)* 1.f* cosLi;
	//	//directLighting = (diffuseBRDF * 1.f /*+ vSpecular*/)/** 1.f* cosLi*/ * (vCamLight.rgb + vDirectionLight) +(vAlbedo.xyz *(1- fRimLight));
	//	//directLighting = vLight.rgb + fRimLight;
	//	directLighting = (vAlbedo.xyz + vAlbedo.xyz *(1 - fRimLight))*(/*vCamLight.rgb +*/ vDirectionLight)/* + vSpecular*/;
	//	Out.vSpecular = vector(specularBRDF * 10.f,1.f);
	//}
	//////////////////////////////////////////////////////////////////
	//IBL 
	//float3 ambientLighting;
	//{
	//	// Sample diffuse irradiance at normal direction.
	//	//float3 irradiance = irradianceTexture.Sample(defaultSampler, N).rgb;
	//	//텍스처의 고유 색상값과 메탈릭의 파워가 강조되는거인듯
	//	float3 irradiance = texCUBE(IrradianceSampler, N).rgb ;
	//	irradiance.rgb = EncodeLinear(irradiance.rgb) ;

	//	//float3 vReflectAlb = tex2D(BaseSampler, N.xy).rgb;
	//
	//	// Calculate Fresnel term for ambient lighting.
	//	// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
	//	// use cosLo instead of angle with light's half-vector (cosLh above).
	//	// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
	//	float3 F = fresnelSchlick(F0, cosLo);
	//
	//	// Get diffuse contribution factor (as with direct lighting).
	//	//float3 kd = lerp(1.0 - F, 0.0, fMetallic);
	//	float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(1.f, 1.f, 1.f), fMetallic);

	//	// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
	//	float3 diffuseIBL = kd * vAlbedo.xyz * irradiance;
	//
	//	//ambientLighting = (diffuseIBL);


	//	// Sample pre-filtered specular reflection environment at correct mipmap level.
	//	//uint specularTextureLevels = querySpecularTextureLevels();mm
	//	//float3 specularIrradiance = specularTexture.SampleLevel(defaultSampler, Lr, roughness * specularTextureLevels).rgb;
	//	int width, height, levels;
	//	//int SpecularTexLevel = g_IrradianceTexture.GetDimensions(0, width, height, levels);

	//	//Lod 사용 버전(렉 이유일거같아서 일단 아닌거 밑에 있음)

	//	//Lod를 사용하면 반사가 너무 눈에 띄게 남는듯
	//	//float3 specularIrradiance = texCUBElod(IrradianceSampler,float4(Lr.xyz,fRoughness*6)).rgb;
	//	float3 specularIrradiance = texCUBE(IrradianceSampler, float3(Lr.xyz)).rgb;

	//	// Split-sum approximation factors for Cook-Torrance specular BRDF.
	//	//x -> 메탈릭한 물체의 반사 정도  y -> 일반 오브젝트들도 포함된 반사정도
	//	float2 specularBRDF = (1.f, 0.05f);

	//	// Total specular IBL contribution.
	//	//float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
	//	float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

	//	//float3 specularIBL = (vReflectAlb * specularBRDF.x + specularBRDF.y) * specularIrradiance;
	//	//러프리스하면 스펙큘러 강조
	//	//float fInvRough = saturate(fRoughness) * -0.5f + 0.5f;;
	//	//float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance * fInvRough;

	//	// Total ambient lighting contribution.
	//	ambientLighting = (diffuseIBL *1.f + specularIBL  * 0.25f );
	////Out.vSpecular = vector(specularIBL * 1.f,1.f);

	//}
	//////////////////////////////////////////////////////////////////


	Out.vShade = vector((directLighting * 1.f/* + ambientLighting * 1.f*/) /** 1.f * vLight*/, 1.f);
	//Out.vShade = vLight;
	//Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower);
	//Out.vSpecular *= 0.1f;
	//Out.vSpecular.a = 0.f;

	///////////////////////////////////////////////////////////////////////////////////////////
	//Calculate Shadow




	//Out.vShadow = float4(1.f,1.f,1.f,1.f);

	return Out;
}
PS_OUT		PS_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	vector	vRefractionDiffuse = tex2D(RefractionDiffuseSampler, In.vTexUV);
	vector	vRefraction = tex2D(RefractionSampler, In.vTexUV);


	vector		vAlbedo = tex2D(BaseSampler, saturate(In.vTexUV + vRefraction.xy)) + vRefractionDiffuse;
	vAlbedo.rgb = EncodeLinear(vAlbedo.rgb) * 1.2f;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	vNormal = vector(normalize(vNormal.xyz), 0.f);
	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	
	//vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 0.f));
	float		fRoughness = vDepth.a;
	float		fMetallic = vDepth.b;


	float	fViewZ = vDepth.y * 1000.f;
	
	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;
	
	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);
	
	float3 L = -normalize(vPosition - g_vLightPos);				//빛의 방향
	float3 N = normalize(vNormal.xyz);					//노말법선
	float3 V = normalize(g_vCamPos - vPosition.xyz);	//시선
	float3 H = normalize(L + V);						//빛 + 시선 하프

	float NV = dot(N, V);
	float NH = dot(N, H);
	float VH = dot(V, H);
	float NL = dot(N, L);
	float LH = dot(L, H);
	
	vector	vLightDir = vPosition - g_vLightPos;
	float	fDistance = length(vLightDir);
	
	float	fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);

	float fAngle = saturate(dot(L, N));
	//Out.vShade.xyz = (fAngle * (g_vLightDiffuse * g_vMtrlDiffuse)*vAlbedo.rgb + fAngle * (g_vLightAmbient.xyz * g_vMtrlAmbient.xyz) )*fAtt; // 음영의 강도를 줄이기 위해 0.5더하고 fAtt를 Saturate안으로 넣음
	Out.vShade.xyz = (fAngle * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient.xyz * g_vMtrlAmbient.xyz))*fAtt; // 음영의 강도를 줄이기 위해 0.5더하고 fAtt를 Saturate안으로 넣음

	Out.vShade.a = 1.f;

	//vector	vLook = normalize(g_vCamPos - vPosition);
	//vector	vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);
	//Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower) * fAtt ;
	//Out.vSpecular = (0.f, 0.f, 0.f, 0.f);
	//////////////////////////////////////////////////////////

	//const float m = fRoughness;
	//float NH2 = NH*NH;
	//float D = exp(-(1 - NH2) / (NH2*m*m)) / (4 * m*m*NH2*NH2);
	///////////////////////////////
	///* B. 기하감쇠율 G를 구한다
	//미세면에 입사된 빛이 다른 미세면에 그림자를 만드는 효과라 한다
	//결과가 1에 가까울 수록 본래 반사광을 표현
	//*/
	//float G = min(1, min(2 * NH*NV / VH, 2 * NH*NL / VH));
	///////////////////////////////
	///* C. 프리넬 값을 구한다
	//굴절률에 관한 공식이다
	//가까우면 투과율이,멀리있으면 반사율이 높아진다
	//쉬릭스 근사치를 사용한다
	//*/
	//float n = 20.f; // 복소굴절률의 실수부(?)
	//float g = sqrt(n*n + LH*LH - 1);
	//float gpc = g + LH;
	//float gnc = g - LH;
	//float cgpc = LH * gpc - 1;
	//float cgnc = LH*gnc + 1;
	//float F = 0.5f * gnc*gnc*(1 + cgpc*cgpc / (cgnc*cgnc)) / (gpc*gpc);
	///////////////////////////////
	///////////////////////////////
	//
	////림라이트
	////float fRimLight = 0.f;
	////fRimLight = smoothstep(0.5f, 1.f, 1 - dot(normalize(N), vCameraDir) - 0.f);
	//float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(1.f, 1.f, 1.f), fMetallic);
	//
	//Out.vSpecular = vector(max(0.001f, F*D*G / NV) *kd * fAtt, 1.f);
	//////////////////////////////////////////////////////////
	return Out;

	//PS_OUT		Out = (PS_OUT)0;
	//
	//vector		vAlbedo = tex2D(BaseSampler, In.vTexUV);
	//vAlbedo.rgb = EncodeLinear(vAlbedo.rgb);
	//vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	//float		fRoughness = vNormal.a;
	//float		fMetallic = vAlbedo.a;
	//
	//vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 1.f));
	//
	////Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	////Out.vShade.a = 1.f;
	//
	////vector	vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);
	//
	//vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	//
	//float	fViewZ = vDepth.a * 1000.f;
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
	////float3	Lo = normalize(g_vCamPos.xyz - vPosition.xyz);
	//vector	vLightDir = -(vPositmion - g_vLightPos);
	//
	//float3	Lo = normalize(g_vCamPos.xyz - g_vLightPos);
	//
	//float3 N = vNormal.xyz;
	//
	//float cosLo = max(0.f, dot(N, Lo));
	//float3 Lr = 2.f*cosLo * N - Lo;
	//
	//float fFdielectric = 0.04f;
	//
	//float3 F0 = lerp(float3(fFdielectric.xxx), vAlbedo.xyz, fMetallic);
	//
	//float3 Lh = normalize(vLightDir + Lo);
	//
	//float3 L = vLightDir.xyz;
	//
	//float cosLi = max(0.0, dot(N, vLightDir));
	//float cosLh = max(0.0, dot(N, Lh));
	//float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
	//float D = ndfGGX(cosLh, fRoughness);
	//float G = gaSchlickGGX(cosLi, cosLo, fRoughness);
	//
	//float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), fMetallic);
	//
	//float3 diffuseBRDF = kd * vAlbedo.xyz;
	//
	//float3 specularBRDF = (F * D * G) / max(0.00001f, 4.0f * cosLi * cosLo);
	//float3 directLighting = (diffuseBRDF + specularBRDF)* g_fRange * cosLi;
	//
	//float3 ambientLighting;
	//{
	//	// Sample diffuse irradiance at normal direction.
	//	//float3 irradiance = irradianceTexture.Sample(defaultSampler, N).rgb;
	//	//텍스처의 고유 색상값과 메탈릭의 파워가 강조되는거인듯
	//	float3 irradiance = (0.5f, 0.5f, 0.5f);
	//
	//
	//	// Calculate Fresnel term for ambient lighting.
	//	// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
	//	// use cosLo instead of angle with light's half-vector (cosLh above).
	//	// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
	//	float3 F = fresnelSchlick(F0, cosLo);
	//
	//	// Get diffuse contribution factor (as with direct lighting).
	//	float3 kd = lerp(1.0 - F, 0.0, fMetallic);
	//
	//	// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
	//	float3 diffuseIBL = kd * vAlbedo.xyz * irradiance;
	//
	//	// Sample pre-filtered specular reflection environment at correct mipmap level.
	//	//uint specularTextureLevels = querySpecularTextureLevels();
	//	//float3 specularIrradiance = specularTexture.SampleLevel(defaultSampler, Lr, roughness * specularTextureLevels).rgb;
	//	float3 specularIrradiance = (0.3f, 0.3f, 0.3f);
	//
	//	// Split-sum approximation factors for Cook-Torrance specular BRDF.
	//	float2 specularBRDF = (0.3f, 0.3f, 0.3f);
	//
	//	// Total specular IBL contribution.
	//	float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
	//
	//	// Total ambient lighting contribution.
	//	ambientLighting = (diffuseIBL + specularIBL);
	//}
	//
	//Out.vShade = vector((directLighting + ambientLighting), 1.f);
	//Out.vSpecular = vector(ambientLighting, 1.f);
	////Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower);
	////Out.vSpecular *= 0.1f;
	////Out.vSpecular.a = 0.f;

	return Out;
}

float4 g_vSSAO_Param = (1.f,1.f,1.f,1.f);
float3 g_vViewDimensions = (1.f,1.f,1000.f);

float4 PS_SSAO(PS_IN In) : COLOR0
{
	float4	Out = (0.f,0.f,0.f,0.f);

const float3 avKernel[8] = 
{ normalize(float3(1.f, 1.f, 1.f))*0.125f,
	normalize(float3(-1.f, -1.f, -1.f))*0.250f,
	normalize(float3(-1.f, -1.f, 1.f))*0.375f,
	normalize(float3(-1.f, 1.f, -1.f))*0.500f,
	normalize(float3(-1.f, 1.f, 1.f))*0.625f,
	normalize(float3(1.f, -1.f, -1.f))*0.750f,
	normalize(float3(1.f, -1.f, 1.f))*0.875f,
	normalize(float3(1.f, 1.f, -1.f))*1.000f };

	
	/*float3 Random = tex2D(NormalSampler, In.vTexUV.xy * 10.f).xyz;
	Random = Random * 2.f - 1.f;*/
	vector	vDepth = tex2D(DepthSampler, In.vTexUV);

	float	fViewZ = vDepth.y * 1000.f;

	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	//depth값을 뷰상태의 포지션으로 바꿔준다
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	float fRadius = g_vSSAO_Param.y;


	//가상의 커널 구체를 만들어 차폐테스트를 할 수 있게 해준다
	float3 vKernelScale = float3(fRadius / vPosition.z, fRadius / vPosition.z, fRadius /1000.f);

	float fOcclusion = 0.f;
	for (int j = 1; j < 3; j++)
	{
		float3 Random = tex2D(NoiseSampler, In.vTexUV.xy * (7.f+(float)j) ).xyz;
		Random = Random * 2.f - 1.f;

		for (int i = 0; i < 8; i++)
		{
			float3 vRotatedKernel = reflect(avKernel[i], Random) * vKernelScale;
			//float3 vRotatedKernel = avKernel[i] * vKernelScale;
			float fSampleDepth = tex2D(DepthSampler, In.vTexUV.xy + vRotatedKernel.xy).y;

			float fDelta = max(fSampleDepth - vDepth.y + vRotatedKernel.z, 0);
			float fRange = abs(fDelta) / (vKernelScale.z * g_vSSAO_Param.z);
			fOcclusion += lerp(fDelta * g_vSSAO_Param.w,g_vSSAO_Param.x,saturate(fRange));
		}
	}

	Out = fOcclusion / (2.f*8.f);
	Out = lerp(0.1f,0.6f,saturate(Out.x));

	return Out;
}


technique		Default_Device
{
	// 기능의 캡슐화
	pass Directional
	{
		alphablendenable = true;
	srcblend = one;
	destblend = one;

	zwriteenable = false;
	vertexshader = NULL;
	pixelshader = compile ps_3_0 PS_MAIN();
	}
	pass	Point
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;
		
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_POINT();
	}

		pass SSAO
	{
		alphablendenable = true;
	srcblend = one;
	destblend = one;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_SSAO();
	}

};
/////////////////////////////////////////////////////////////////////////////////
//fog Constant
float g_fDensity = 0.35f;
float g_fEndPos = 30.f;
float g_fFogHeight = 3.f;
float4 g_vFogColor = float4(0.1f, 0.1f, 0.1f,1.f);

/////////////////////////////////////////////////////////////////////////////////
float4 PS_DEPTH_FOG(PS_IN In) : COLOR0
{
	float4 vDepth = tex2D(DepthSampler,In.vTexUV);

	float fDensity = g_fDensity;
	float fEndPos = g_fEndPos;

	float	fViewZ = vDepth.y * 1000.f;
	
	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;
	
	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	float3 vCamToPos = vPosition.xyz - g_vCamPos;
	//float fDist = (vCamToPos.x*vCamToPos.x + vCamToPos.y*vCamToPos.y + vCamToPos.z*vCamToPos.z);
	float fDist = distance(g_vCamPos, vPosition.xyz);
	fDist = fDist / fEndPos * 4;
	//float f = exp(-(fDist * fDensity) * (fDist * fDensity));
	float f =-1/ exp( (fDist * fDensity));

	return vector(g_vFogColor.xyz,f);
}
float4 PS_NORMAL_FOG(PS_IN In) : COLOR0
{
	return float4(0.f,0.f,0.f,0.f);
}
float4 PS_HEIGHT_FOG(PS_IN In) : COLOR0
{
	float4 vDepth = tex2D(DepthSampler,In.vTexUV);

	float fDensity = 0.f;
	float fEndPos = g_fEndPos;
	float fFogRange = g_fFogHeight;

	float	fViewZ = vDepth.y * 1000.f;
	
	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;
	
	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);
	
	float fDeltaD = distance(g_vCamPos, vPosition.xyz) / fEndPos * 2.f;

	float fDeltaY, fDensityIntegral;
	//카메라가 포그 최고점보다 높으면
	if (g_vCamPos.y >= g_fFogHeight)
	{
		//픽셀위치가 포그 최고점보다 낮으면
		if (vPosition.y < g_fFogHeight)
		{
			fDeltaY = (g_fFogHeight - vPosition.y) / g_fFogHeight * 2;
			fDensityIntegral = abs(fDeltaY*fDeltaY*g_fDensity);
		}
		else
		{
			fDeltaY = 0.f;
			fDensityIntegral = 0.f;
		}
	}
	//카메라가 포그 아래에 있을 경우
	else
	{
		if (g_vCamPos.y < g_fFogHeight)
		{
			//픽셀위치가 포그 최고점보다 낮으면
			if (vPosition.y < g_fFogHeight)
			{
				float fDeltaA = (g_fFogHeight - g_vCamPos.y) / g_fFogHeight * 2;
				float fDeltaB = (g_fFogHeight - vPosition.y) / g_fFogHeight * 2;

				fDeltaY = abs(fDeltaA-fDeltaB);
				fDensityIntegral = abs((fDeltaA*fDeltaA*g_fDensity) -(fDeltaB*fDeltaB*g_fDensity));
			}
			else
			{
				fDeltaY = abs(g_fFogHeight - g_vCamPos.y)/fFogRange*2;
				fDensityIntegral = abs(fDeltaY*fDeltaY*g_fDensity);
			}
		}

	}

	if (fDeltaY != 0.f)
	{
		fDensity = sqrt(1.f + (fDeltaD / fDeltaY)*(fDeltaD / fDeltaY)) * fDensityIntegral;
	}
	else
		fDensity = 0.f;

	float f = -1/ exp(fDensity);
	
	return vector(g_vFogColor.xyz,f);
}
float4 PS_CALCULATE_FOG(PS_IN In) : COLOR0
{
	return float4(0.f,0.f,0.f,0.f);
}

technique		Fog
{
		pass Depth
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;
		
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_DEPTH_FOG();

	}
		pass Normal
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_NORMAL_FOG();
	}
		pass Height
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_HEIGHT_FOG();
	}
		pass Calculate
	{
		alphablendenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_CALCULATE_FOG();

	}
}
