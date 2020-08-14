#ifndef ENGINE_STRUCT_H__
#define ENGINE_STRUCT_H__

namespace ENGINE
{
	typedef	struct tagVertexColor
	{		
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;


	typedef	struct tagVertexNormalTexture
	{
		_vec3			vPosition;
		_vec3			vNormal;
		_vec2		 	vTexUV;
	}VTXNORTEX;

	const _ulong		FVF_NORTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec2		 	vTexUV;
	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;




	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


	////////////////////////////랜더타겟//////////////////////

	//둘중하나 나중에 빼자
	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;


	typedef struct tagViewPortVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;
	}VTXVIEWPORT;

	const _ulong		FVF_VIEWPORT = D3DFVF_XYZRHW | D3DFVF_TEX1;

	
	/////////////////////////////////////////////////////////////////

	typedef struct tagPathInfo
	{
		wstring wstrPath = L"";
		wstring wstrObjKey = L"";
		wstring wstrStateKey = L"";
		int		iCount = 0;
	}IMG_PATH_INFO;







	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;

	typedef	 struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix			CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;
		LPDIRECT3DTEXTURE9*		ppNormalTexture;
		LPDIRECT3DTEXTURE9*		ppRoughnessTexture;
		LPDIRECT3DTEXTURE9*		ppEmissiveTexture;
		LPDIRECT3DTEXTURE9*		ppMetallicTexture;

		LPD3DXMESH				pOriMesh;	// 최초의 상태를 가지고 있는 메시 객체(좌표 불변)

		_ulong					dwNumBones; // 뼈의 개수

		_matrix**				ppFrameCombinedTransformationMatrix;
		// 뼈들이 지니고 있는 상태 행렬의 주소값을 보관하기 위한 이중 포인터

		_matrix*				pFrameOffsetMatrix; // 애니메이션 정보를 포함한 상태의 행렬
		_matrix*				pRenderingMatrix;	// 최종적으로 렌더링을 수행하기 위한 완성 상태의 행렬

		//하드웨어 스키닝 시 필요한 객체
		LPD3DXBUFFER			pBoneCombinationBuffer;
		_int					iSubMeshNum; //한 메쉬 객체 내의 몇번째 서브메쉬인지 저장


	}D3DXMESHCONTAINER_DERIVED;

	typedef	struct	tagVetexPoint
	{
		_vec3		vPos;
	}VTXPTTEX;

	typedef	struct	tagParticleMatrix
	{
		_vec2		vTexUV;
		_vec2		vTexUVSize;

		_vec4		vPos;
		_vec4		vColor;
		//_vec4		vRight;
		//_vec4		vUp;
		//_vec4		vLook;

	}VTXPARTICLEMATRIX;

	typedef	struct	tagVertexMatrix
	{
		_vec4		vRight;
		_vec4		vUp;
		_vec4		vLook;
		_vec4		vPos;

	}VTXMATRIX;



	typedef struct tagSubSetScope
	{
		tagSubSetScope()
			: m_dwStart(0)
			, m_dwEnd(0)
		{
		}
		tagSubSetScope(_uint dwStart, _uint dwEnd)
			: m_dwStart(dwStart)
			, m_dwEnd(dwEnd)
		{
		}

		_uint	m_dwStart;
		_uint	m_dwEnd;

	}_SubSet;


	typedef struct tagWorldInfo
	{
		tagWorldInfo()
			: vPos(_vec3(0.f, 0.f, 0.f))
			, vScale(_vec3(1.f, 1.f, 1.f))
			, vAngle(_vec3(0.f, 0.f, 0.f))
		{}

		_vec3 vPos;
		_vec3 vScale;
		_vec3 vAngle;
	}_WorldInfo;
	class CGameObject;
	typedef struct tagColMsg
	{
		tagColMsg()
			: eID(OBJ_ID_END)
			, iMsg(0)
			, pColObj(nullptr)

		{
		}
		tagColMsg(ENGINE::OBJ_ID eID, _int iMsg, ENGINE::CGameObject* pColObj)
			: eID(eID)
			, iMsg(iMsg)
			, pColObj(pColObj)
		{
		}
		ENGINE::OBJ_ID			eID;
		_int					iMsg;
		ENGINE::CGameObject*	pColObj;
	}ColMsg;
///////////////////////////////////////////////////
//TextureEffect State
#define TEX_EFFECTOP_USE_DISTORTION_ALBEDO 0x00000001
#define TEX_EFFECTOP_USE_DISTORTION_NORMAL 0x00000002
#define TEX_EFFECTOP_USE_MASK 0x00000004
#define TEX_EFFECTOP_USE_NORMAL 0x00000008
#define TEX_EFFECTOP_USE_DISOLVE 0x00000010
#define TEX_EFFECTOP_USE_TEXANIM 0x00000020
#define TEX_EFFECTOP_USE_MOTIONBLUR 0x00000040
///////////////////////////////////////////////////
//EFFECT
	typedef struct tagTextureEffectInfo
	{
		DWORD dwTextureState = 0;
		//////////////////////////////////////
		//Distortion
		wstring wstrDistortionTex = L"";
		_float fDistortionLevel=0.f;
		//Mask
		wstring wstrMaskTex = L"";
		_bool bUseMask = false;
		_vec4 vMaskLevel= {0.f,0.f,0.f,0.f};
		//Albedo
		wstring wstrAlbedoTex = L"";
		_vec4 vDiffuseLevel = {1.f,1.f,1.f,1.f};
		//Disolve
		wstring wstrDisolveTex = L"";
		_vec4 vDisolveLevel = {0.f,0.f,0.f,0.f};
		//motion blur
		wstring wstrMotionBlurTex = L"";
		_float fBlurLevel = 0.f;

		_int iMaxIndexX = 0;
		_int iMaxIndexY = 0;
		_vec2 vUVStart = {0.f,0.f};
		_vec2 vUVSize = {1.f,1.f};
		//UV가 흐르는 느낌
		_vec2 vUVMove = {0.f,0.f};
		//UV가 사이즈만큼 바뀐다
		_float fTexAnimChangeFre = 1.f;
		/////////////////////////////////////////////////
		//이쪽꺼는 각 이펙트 객체들이 생성,관리해줘야 한다
		////UV 애니메이션 상태
		////TexAnimChange 시에 UVSize와 곱해서 다음 애니메이션으로 넘어가게 만들거
		//_vec2 vUVIndex = { 0.f,0.f };
		//_float fCurTexTimer = 0.f;
		////흐르는 UV에 관한 상태
		//_vec2 vCurUV = { 0.f,0.f };
	}TEX_EFFECT_INFO;

#define PARTICLE_EFFECTOP_USE_GRAVITIY 0x00000001
#define PARTICLE_EFFECTOP_USE_MOTIONBLUR 0x00000002
	//파티클 전체에 적용되는 정보
	typedef struct tagParticleEffectInfo
	{
		/*velocity
			accel
			gravitiy 사용 여부
			Create Amount
			Max Create
			create frequency
			particle Motion blur(이게 과연 될지)
			Create Pos*/
		DWORD dwParticleState = 0;

		///////////////////////////////////////////////////////////
		//파티클 이동 관련
		//이동
		_vec3 vVelocityMax = { 1.f,1.f,1.f };
		_vec3 vVelocityMin = { -1.f,-1.f,-1.f };
		//생성 위치
		_vec3 vPosMax = { 0.f,0.f,0.f };
		_vec3 vPosMin = { 0.f,0.f,0.f };
		//색상
		_vec4 vColorMax = { 1.f,1.f,1.f,1.f };
		_vec4 vColorMin = { 0.f,0.f,0.f,0.f };
		//색상 바뀌는 양
		_vec4 vColorFadeMax = { 1.f,1.f,1.f,1.f };
		_vec4 vColorFadeMin = { 0.f,0.f,0.f,0.f };
		//Velocity에 가속되는 값
		_float fAccel = 1.f;
		//Life
		_float fMinLife= 1.f;
		_float fMaxLife = 0.f;
		//중력 적용 값
		_vec3 vGravitiyLevel = {0.f,0.f,0.f};
		///////////////////////////////////////////////////////////
		//한번에 생성되는 양
		_int iCreateAmount = 0;
		_float fCreateFre = 1.f;

		_int iRenderPass = 0;
	}PARTICLE_EFFECT_INFO;

	//한 파티클의 움직임에 관한 정보
	typedef struct tagParticleAttribute
	{
		_vec3 vVelocity = { 0.f,0.f,0.f };
		_vec3 vPos = { 0.f,0.f,0.f };
		_vec4 vColor = { 1.f,1.f,1.f,1.f };
		_vec4 vColorFade = { 0.f,0.f,0.f,0.f };
		//중력 노출 시간(1이 최대중력이 되게 하자)
		_float fGravitiyTimer = 0.f;

		_float fLife = 1.f;
		///////////////////////////////////////////////////////////
		//UV 애니메이션 상태
		//TexAnimChange 시에 UVSize와 곱해서 다음 애니메이션으로 넘어가게 만들거
		_vec2 vUVIndex = { 0.f,0.f };
		_int iUVIndexX = 0;
		_int iUVIndexY = 0;

		_float fCurTexTimer = 0.f;
		//흐르는 UV에 관한 상태
		_vec2 vCurUV = { 0.f,0.f };
	}PARTICLE_ATT;
}
#endif // ENGINE_Struct_h__
