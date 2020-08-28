#include "Shader_BaseRender.hpp"
float4      tcLumOffsets[4];               
float4      tcDSOffsets[9];                

float2		g_vPixelSize;
float		g_fDeltaTime = 0.f;
float		g_fSmokeDiffuseFactor = 1.f;

/////////////////////////////////////
//GodRay
float g_fDecay = 0.96815f;
float g_fExposure = 0.2f;
float g_fDensity = 0.926f;
float g_fWeight = 0.58767f;
int		g_iGodRayCount = 0;
float2 g_vFocusArray[5];
/////////////////////////////////////
texture			g_BlendTexture;

sampler BlendSampler = sampler_state
{
	texture = g_BlendTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_LuminanceTexture;

sampler LuminanceSampler = sampler_state
{
	texture = g_LuminanceTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};
texture			g_BloomTexture;

sampler BloomSampler = sampler_state
{
	texture = g_BloomTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

};
float fBrightPassThreshold = 1.f;
float4 tcDownSampleOffsets[16];

float HBloomWeights[9];                
float HBloomOffsets[9];                

float VBloomWeights[9];                
float VBloomOffsets[9];                

struct PS_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_GreyScaleDownSample(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float average = 0.0f;
	float maximum = -1e20;
	float4 color = 0.0f;
	float3 WEIGHT = float3(0.299f, 0.587f, 0.114f);

	for (int i = 0; i < 4; i++)
	{
		color = tex2D(BlendSampler, abs(In.vTexUV + float2(tcLumOffsets[i].x, tcLumOffsets[i].y)));

		// There are a number of ways we can try and convert the RGB value into
		// a single luminance value:

		//그레이 벨류를 만드는 다양한 방법

		////1.평균 그레이스케일 value를 만든다
		//float GreyValue = dot( color.rgb, float3( 0.33f, 0.33f, 0.33f ) );
		//
		//// 2. 가중치에 따라 색상 변경:
		//float GreyValue = dot( color.rgb, WEIGHT );

		// 3. Take the maximum value of the incoming, same as computing the
		//    brightness/value for an HSV/HSB conversion:
		float GreyValue = max(color.r, max(color.g, color.b));

		// 4. Compute the luminance component as per the HSL colour space:
		//float GreyValue = 0.5f * ( max( color.r, max( color.g, color.b ) ) + min( color.r, min( color.g, color.b ) ) );

		// 5. Use the magnitude of the colour
		//float GreyValue = length( color.rgb );

		maximum = max(maximum, GreyValue);
		average += (0.25f * log(1e-5 + GreyValue)); //1e-5 necessary to stop the singularity at GreyValue=0
	}

	average = exp(average);
	Out.vColor = float4(average, maximum, 0.0f, 1.0f);
	// Output the luminance to the render target
	return Out;
}
PS_OUT PS_DownSample(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	// Compute the average of the 10 necessary samples
	float4 color = 0.0f;
	float maximum = -1e20;
	float average = 0.0f;
	
	for (int i = 0; i < 9; i++)
	{
		color = tex2D(LuminanceSampler,abs( In.vTexUV + float2(tcDSOffsets[i].x, tcDSOffsets[i].y)));
		average += color.r;
		maximum = max(maximum, color.g);
	}
	
	//샘플링된 수 만큼 나눈다
	average /= 9.0f;
	
	// Return the new average luminance
	Out.vColor = float4(average, maximum, 0.0f, 1.0f);
	return Out;
}

technique Luminance
{
	pass GreyScaleDownSample
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_GreyScaleDownSample();
	}
	pass DownSample
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_DownSample();
	}
}

PS_OUT PS_BrightDownSample(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;


	for (int i = 0; i < 16; i++)
	{
		Out.vColor += tex2D(BaseSampler, In.vTexUV + float2(tcDownSampleOffsets[i].x, tcDownSampleOffsets[i].y));
	}

	Out.vColor *= (1.0f / 16.0f);

	return Out;
}

	PS_OUT PS_HorizontalBlur(PS_IN In)
	{
		PS_OUT Out = (PS_OUT)0;

		float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

		for (int i = 0; i < 9; i++)
		{
			color += (tex2D(BaseSampler, In.vTexUV + float2(HBloomOffsets[i], 0.0f)) * HBloomWeights[i]);
		}

		Out.vColor = vector(color.rgb, 1.f);

		return Out;
	}

	PS_OUT PS_VerticalBlur(PS_IN In)
	{
		PS_OUT Out = (PS_OUT)0;

		float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

		for (int i = 0; i < 9; i++)
		{
			color += (tex2D(BaseSampler, In.vTexUV + float2(0.f, VBloomOffsets[i])) * VBloomWeights[i]);
		}

		Out.vColor = vector(color.rgb, 1.f);

		return Out;
	}

	PS_OUT PS_BrightPass(PS_IN In)
	{
		PS_OUT Out = (PS_OUT)0;
		float4 averageBright = { 0.0f, 0.0f, 0.0f, 0.0f };

		// load in and combine the 4 samples from the source HDR texture
		for (int i = 0; i < 4; i++)
		{
			averageBright +=saturate(tex2D(BaseSampler, In.vTexUV + float2(tcDownSampleOffsets[i].x, tcDownSampleOffsets[i].y)));
		}
		averageBright *= 0.25f;
		float luminance = max(averageBright.r, max(averageBright.g, averageBright.b));
		if (luminance < fBrightPassThreshold)
			averageBright = float4(0.0f, 0.0f, 0.0f, 1.0f);

		float4 averageGlow = { 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 4; i++)
		{
			averageGlow += tex2D(BloomSampler, In.vTexUV + float2(tcDownSampleOffsets[i].x, tcDownSampleOffsets[i].y));
		}
		averageGlow *= 0.25f;
		// Determine the brightness of this particular pixel. As with the luminance calculations
		// there are 4 possible variations on this calculation:

		// 1. Do a very simple mathematical average:
		//float luminance = dot( average.rgb, float3( 0.33f, 0.33f, 0.33f ) );

		// 2. Perform a more accurately weighted average:
		//float luminance = dot( average.rgb, float3( 0.299f, 0.587f, 0.114f ) );

		// 3. Take the maximum value of the incoming, same as computing the
		//    brightness/value for an HSV/HSB conversion:

		// 4. Compute the luminance component as per the HSL colour space:
		//float luminance = 0.5f * ( max( average.r, max( average.g, average.b ) ) + min( average.r, min( average.g, average.b ) ) );

		// 5. Use the magnitude of the colour
		//float luminance = length( average.rgb );

		// Determine whether this pixel passes the test...
		Out.vColor = averageBright + averageGlow;
		// Write the colour to the bright-pass render target
		return Out;
	}
technique Blur
{
	pass BrightPass
	{

			alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_BrightPass();
	}
	pass DownSample
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_BrightDownSample();
	}

	pass HorizontalBlur
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_HorizontalBlur();
	}

	pass VerticalBlur
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_VerticalBlur();
	}
}

PS_OUT PS_ComputeSmoke(PS_IN In)
	{
		PS_OUT Out = (PS_OUT)0;

		Out.vColor = tex2D(BaseSampler, In.vTexUV);

		float4 vRight	= tex2D(BaseSampler, float2(In.vTexUV.x + g_vPixelSize.x, In.vTexUV.y));
		float4 vLeft	= tex2D(BaseSampler, float2(In.vTexUV.x - g_vPixelSize.x, In.vTexUV.y));
		float4 vUp		= tex2D(BaseSampler, float2(In.vTexUV.x, In.vTexUV.y + g_vPixelSize.y));
		float4 vDown	= tex2D(BaseSampler, float2(In.vTexUV.x, In.vTexUV.y - g_vPixelSize.y));

		//Out.vColor.rgb += g_fSmokeDiffuseFactor * g_fDeltaTime * 
		//	(
		//		vRight.rgb +
		//		vLeft.rgb +
		//		vUp.rgb +
		//		vDown.rgb -
		//		4.f * Out.vColor.rgb
		//	);

		float fFactor = g_fSmokeDiffuseFactor * g_fDeltaTime * (vRight.r + vLeft.r + vUp.r + vDown.r - 4.f * Out.vColor.r);
		float minimum = 0.003;
		if (fFactor >= -minimum && fFactor < 0.0) fFactor = -minimum;

		Out.vColor.rgb += fFactor;
		return Out;
	}

technique Smoke
{
		pass ComputeSmoke
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = InvSrcAlpha;

	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_ComputeSmoke();
	}
}
PS_OUT PS_ComputeGodRay(PS_IN In)
	{
		//https://medium.com/community-play-3d/god-rays-whats-that-5a67f26aeac2

		PS_OUT Out = (PS_OUT)0;

		int NUM_SAMPLE = 120;
		if (g_iGodRayCount == 0)
			return Out;

		for (int iFocus = 0; iFocus < g_iGodRayCount; iFocus++)
		{
			float2 vTex = In.vTexUV;
			float2 vDeltaTex = vTex - g_vFocusArray[iFocus];
			vDeltaTex *= 1.f / float(NUM_SAMPLE) * g_fDensity;
			float fIlluminationDecay = 1.f;

			float4 vColor = tex2D(BaseSampler, vTex)*0.4f;
			for (int iSamples = 0; iSamples < NUM_SAMPLE; iSamples++)
			{
				vTex -= vDeltaTex;
				float4 vSample = tex2D(BaseSampler, vTex) * 0.4f;
				vSample *= fIlluminationDecay * g_fWeight;

				vColor += vSample;
				fIlluminationDecay *= g_fDecay;
			}
			Out.vColor += vColor;
		}
		//Out.vColor /= g_iGodRayCount;
		return Out;
	}
technique GodRay
{
		pass Compute_GodRay
	{
		alphablendenable = false;
	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_ComputeGodRay();
	}
}

PS_OUT PS_MOTIONBLUR(PS_IN In)
	{
		PS_OUT		Out = (PS_OUT)0;

		int iNumBlurSample = 10;

		vector		vVelocity = tex2D(VelocitySampler, In.vTexUV);
		vVelocity.xy *= 0.01f;

		int icnt = 1;
		float4 BColor;

		Out.vColor = tex2D(BlendSampler, In.vTexUV);
		for (int i = icnt; i < iNumBlurSample; i++)
		{
			BColor = tex2D(BlendSampler, In.vTexUV + vVelocity.xy * (float)i);

			//if (vVelocity.a < BColor.a + 0.04f)
			//{
			++icnt;
			Out.vColor += BColor;
			//}
		}

		Out.vColor /= (float)(icnt);
		return Out;
	}
technique Motion_Blur
	{
		pass Compute_MotionBlur
	{
		zwriteEnable = false;
	alphablendenable = false;
	srcblend = srcalpha;
	destblend = One;

	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_MOTIONBLUR();
	}
	}
//void main()
//{
//	//float4 c0 = Sample();
//	//
//	//vec3 color = vec3(c0.r, c0.g, c0.b);
//	//color = Uncharted2ToneMapping(color);
//	//
//	//SetOutput(float4(color[0], color[1], color[2], c0.a));
//}
/////////////////////////////////////////////////////////////////

