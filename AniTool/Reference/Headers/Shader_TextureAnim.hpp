float2 g_vUVSpeed = float2(0.f,0.f);
float2 g_vUVStart = float2(0.f,0.f);
float2 g_vUVSize = float2(1.f, 1.f);

float2 g_vDistortionTime = {1.f,1.f};

int g_iUVIndexX = 1;
int g_iUVIndexY = 1;

float4 g_vDiffuseColor = float4(1.f,1.f,1.f,1.f);

float g_fTime = 0.f;

float2 g_vUVOffset = {1.f,1.f};

float g_fDistiortionTag = 1.f;
float4 g_vMaskTag = float4(1.f, 1.f, 1.f,1.f);
//마스크 사용하면 1, 사용 안하면 0
float g_fUsingMaskTag = 1.f;

float4 g_vGodRayTag = float4(0.f, 0.f, 0.f, 1.f);
float4 g_vBloomLevel = float4(0.f, 0.f, 0.f, 1.f);

float4 g_vAmbient = float4(1.f, 1.f, 1.f, 1.f);


float g_fLUT_Time = 0.f;
texture2D	g_LUTTexture;
sampler		LUTSampler = sampler_state
{
	texture = g_LUTTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};