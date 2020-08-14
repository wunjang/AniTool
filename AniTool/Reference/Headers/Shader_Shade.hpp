#include "Shader_BaseRender.hpp"
static const float PI = 3.141592;
static const float Epsilon = 0.00001;

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
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
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
vector	g_vLightAmbient;

vector	g_vMtrlDiffuse = (vector)1.f;
vector	g_vMtrlAmbient = (vector)1.f;

vector	g_vCamPos;
float	g_fPower;

matrix	g_matProjInv;
matrix	g_matViewInv;

vector	g_vLightPos;
float	g_fRange;
struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	
	vNormal = vector(vNormal.xyz * 2.f - 1.f, vNormal.a);
	
	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal) + 0.5f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient) + 0.3f;//그림자 음영을 줄이기 위해 0.5더하고 saturate해줌
	Out.vShade.a = 1.f;
	
	vector	vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);
	
	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	
	float	fViewZ = vDepth.a * 1000.f;
	
	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;
	
	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);
	
	vector	vLook = normalize(g_vCamPos - vPosition);
	
	Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower) * vNormal.a;
	//Out.vSpecular *= 0.1f;
	Out.vSpecular.a = 1.f;

	//////////////////////////////////////////////////////////
	//PBR 실패
	//PS_OUT		Out = (PS_OUT)0;
	//
	//vector		vAlbedo = tex2D(BaseSampler, In.vTexUV);
	//vAlbedo.rgb = EncodeLinear(vAlbedo.rgb);
	//vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	//float		fRoughness = vNormal.a;
	//float		fMetallic = vAlbedo.a;
	//
	//vNormal = normalize( vector(vNormal.xyz * 2.f - 1.f, 1.f));
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
	//vPosition = float4(80.f, 35.f, 35.f,1.f);
	//float3	Lo = normalize(g_vCamPos.xyz - vPosition);

	//float3 N = vNormal.xyz;
	//
	//float cosLo = max(0.f,dot(N,Lo));
	//float3 Lr = 2.f*cosLo * N - Lo;
	//
	//float fFdielectric = 0.04f;
	//
	//float3 F0 = lerp(float3(fFdielectric.xxx), vAlbedo.xyz, fMetallic);
	//
	//float3 vLightDir = normalize(g_vLightDir.xyz);

	//float3 Lh = normalize(vLightDir + Lo);
	//
	//float3 L = g_vLightDir.xyz;
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
	//float3 directLighting = (diffuseBRDF + specularBRDF)* 1000.f * cosLi;
	//
	//float3 ambientLighting;
	//{
	//	// Sample diffuse irradiance at normal direction.
	//	//float3 irradiance = irradianceTexture.Sample(defaultSampler, N).rgb;
	//	//텍스처의 고유 색상값과 메탈릭의 파워가 강조되는거인듯
	//	float3 irradiance = (1.f, 1.f, 1.f);
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
	//	float3 specularIrradiance = (1.f, 1.f, 1.f);
	//
	//	// Split-sum approximation factors for Cook-Torrance specular BRDF.
	//	float2 specularBRDF = (0.3f, 0.3f, 0.3f);
	//
	//	// Total specular IBL contribution.
	//	float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
	//
	//	// Total ambient lighting contribution.
	//	ambientLighting = ( diffuseIBL + specularIBL);
	//}
	//
	//Out.vShade = vector((directLighting+ambientLighting), 1.f);
	//Out.vSpecular = vector(ambientLighting,1.f);
	////Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower);
	////Out.vSpecular *= 0.1f;
	////Out.vSpecular.a = 0.f;

	return Out;
}
PS_OUT		PS_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	
	vNormal = vector(vNormal.xyz * 2.f - 1.f, vNormal.a);
	

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	
	float	fViewZ = vDepth.a * 1000.f;
	
	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환
	
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;
	
	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);
	
	
	vector	vLook = normalize(g_vCamPos - vPosition);
	vector	vLightDir = vPosition - g_vLightPos;
	vector	vReflect = reflect(normalize(vLook), vNormal);
	float	fDistance = length(vLightDir);
	
	float	fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);
	
	Out.vShade = saturate(dot(normalize(vLightDir) * -1.f, vNormal)/* + 0.5f*/) * ((g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient /*+ 0.5f*/))*fAtt + 0.3f; // 음영의 강도를 줄이기 위해 0.5더하고 fAtt를 Saturate안으로 넣음
	Out.vShade.a = 1.f;
	
	Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower) * fAtt * vNormal.a;;
	//Out.vSpecular = (0.f, 0.f, 0.f, 0.f);
	Out.vSpecular.a = 0.f;
	
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
	//vector	vLightDir = -(vPosition - g_vLightPos);
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

	float	fViewZ = vDepth.a * 1000.f;

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

		for (int i = 0; i < 4; i++)
		{
			float3 vRotatedKernel = reflect(avKernel[i], Random) * vKernelScale;
			//float3 vRotatedKernel = avKernel[i] * vKernelScale;
			float fSampleDepth = tex2D(DepthSampler, In.vTexUV.xy + vRotatedKernel.xy).a;

			float fDelta = max(fSampleDepth - vDepth.a + vRotatedKernel.z, 0);
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
	srcblend = srcalpha;
	destblend = invsrcalpha;

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
	srcblend = srcalpha;
	destblend = invsrcalpha;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_SSAO();
	}

};