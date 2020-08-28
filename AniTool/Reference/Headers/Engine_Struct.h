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


	////////////////////////////����Ÿ��//////////////////////

	//�����ϳ� ���߿� ����
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
	class CTexture;
	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;
		LPDIRECT3DTEXTURE9*		ppNormalTexture;
		LPDIRECT3DTEXTURE9*		ppRoughnessTexture;
		LPDIRECT3DTEXTURE9*		ppEmissiveTexture;
		LPDIRECT3DTEXTURE9*		ppMetallicTexture;

		//vector<CTexture*>			m_vecOriginTexture[TEXTURE::SHADER::SHADER_END];
		//vector<CTexture*>			m_vecTexture[TEXTURE::SHADER::SHADER_END];

		LPD3DXMESH				pOriMesh;	// ������ ���¸� ������ �ִ� �޽� ��ü(��ǥ �Һ�)

		_ulong					dwNumBones; // ���� ����

		_matrix**				ppFrameCombinedTransformationMatrix;
		// ������ ���ϰ� �ִ� ���� ����� �ּҰ��� �����ϱ� ���� ���� ������

		_matrix*				pFrameOffsetMatrix; // �ִϸ��̼� ������ ������ ������ ���
		_matrix*				pRenderingMatrix;	// ���������� �������� �����ϱ� ���� �ϼ� ������ ���

		//�ϵ���� ��Ű�� �� �ʿ��� ��ü
		LPD3DXBUFFER			pBoneCombinationBuffer;
		_int					iSubMeshNum; //�� �޽� ��ü ���� ���° ����޽����� ����

		_bool					bCustomTextureArr[ENGINE::TEXTURE::SHADER_END];//�޽��� �ؽ�ó�� �����Ϳ� �ٲ���ٸ� Ʈ��

	}D3DXMESHCONTAINER_DERIVED;

	typedef	struct	tagVetexPoint
	{
		_vec3		vPos;

		_vec2		vTexUV;
		_vec2		vTexUVSize;

		_vec4		vColor;

		_float		fSize;
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
	namespace FOG
	{
		enum FOG_TYPE {FOG_DEPTH,FOG_NORMAL,FOG_HEIGHT,FOG_END};
		typedef struct tagFogInfo
		{
			//��
			_vec4 vDensity;
			_vec4 vColor;
			_float fRange;

			FOG_TYPE eType;

			tagFogInfo()
			{
				vDensity = {0.33f,100.f,0.f,0.f};
				vColor = {1.f,1.f,1.f,1.f};
				fRange = 1.f;
				eType = FOG_NORMAL;
			}

		}FOG_INFO;
	}
	class CGameObject;
	class CCollider;
	typedef struct tagColMsg
	{
		tagColMsg()
			: iMyColliderNum(0)
			, eID(OBJ_ID_END)
			, iMsg(0)
			, pOtherCollObj(nullptr)
			, pOtherCollider(nullptr)
		{
		}
		tagColMsg(_int iMyColliderNum, ENGINE::OBJ_ID eID, _int iMsg, ENGINE::CGameObject* pOtherColObj, ENGINE::CCollider* pOtherCollider)
			: iMyColliderNum(iMyColliderNum)
			, eID(eID)
			, iMsg(iMsg)
			, pOtherCollObj(pOtherColObj)
			, pOtherCollider(pOtherCollider)
		{
		}
		_uint					iMyColliderNum;//���� �浹�� �浹ü��ȣ
		ENGINE::OBJ_ID			eID;//�浹�� ����� ���̵�
		_uint					iMsg;//�浹�� ����� �޼���
		ENGINE::CGameObject*	pOtherCollObj;//�浹�� ����� ������
		ENGINE::CCollider*		pOtherCollider;//�浹�� ����� �浹ü ������
	}ColMsg;

	typedef struct tagColCellInfo
	{
		D3DXPLANE tPlane;
		_vec3 vNormal;
	}COL_CELL_INFO;
///////////////////////////////////////////////////
//TextureEffect State
#define TEX_EFFECTOP_USE_DISTORTION_ALBEDO 0x00000001
#define TEX_EFFECTOP_USE_DISTORTION_NORMAL 0x00000002
#define TEX_EFFECTOP_USE_MASK 0x00000004
#define TEX_EFFECTOP_USE_NORMAL 0x00000008
#define TEX_EFFECTOP_USE_DISOLVE 0x00000010
#define TEX_EFFECTOP_USE_TEXANIM 0x00000020
#define TEX_EFFECTOP_USE_MOTIONBLUR 0x00000040
#define TEX_EFFECTOP_USE_AMBIENT 0x00000080
#define TEX_EFFECTOP_USE_LUT 0x00000100
#define TEX_EFFECTOP_USE_ALPHA 0x00000200
#define TEX_EFFECTOP_USE_CULL_NONE 0x00000400
#define TEX_EFFECTOP_USE_POSTPROCESS 0x00000800
#define TEX_EFFECTOP_USE_BILLBOARD 0x00001000
///////////////////////////////////////////////////
//EFFECT
	typedef struct tagTextureEffectInfo
	{
		DWORD dwTextureState = 0;
		//////////////////////////////////////
		//Distortion
		wstring wstrDistortionTex = L"";
		_float fDistortionLevel=0.f;
		
		_vec2 vDistortionSpeed = { 0.f ,0.f};
		//Mask
		wstring wstrMaskTex = L"";
		_bool bUseMask = false;
		_vec4 vMaskLevel= {1.f,1.f,1.f,1.f };
		//Albedo
		wstring wstrAlbedoTex = L"";
		_vec4 vDiffuseLevel = {1.f,1.f,1.f,1.f};
		//motion blur
		wstring wstrNormalTex = L"";
		//Disolve
		wstring wstrDisolveTex = L"";
		_vec4 vDisolveLevel = {0.f,0.f,0.f,0.f};
		//motion blur
		wstring wstrMotionBlurTex = L"";
		_float fBlurLevel = 0.f;
		//Smoke
		_vec4 vSmokeLevel = {0.f,0.f,0.f,1.f};
		//Bloom
		_vec4 vBloomLevel = { 0.f,0.f,0.f,0.f };
		//ambient
		_vec4 vAmbient = {0.f,0.f,0.f,0.f };
		//LUT
		wstring wstrLUTTex = L"";
		_float fLUTTime = 0.f;
		_float fLUTSpeed = 1.f;

		_int iRenderPass = 0;

		_int iMaxIndexX = 1;
		_int iMaxIndexY = 1;
		_vec2 vUVStart = {0.f,0.f};
		_vec2 vUVSize = {1.f,1.f};
		//UV�� �帣�� ����
		_vec2 vUVMove = {0.f,0.f};
		//UV�� �����ŭ �ٲ��
		_float fTexAnimChangeFre = 1.f;
		/////////////////////////////////////////////////
		//���ʲ��� �� ����Ʈ ��ü���� ����,��������� �Ѵ�
		////UV �ִϸ��̼� ����
		////TexAnimChange �ÿ� UVSize�� ���ؼ� ���� �ִϸ��̼����� �Ѿ�� �����
		//_vec2 vUVIndex = { 0.f,0.f };
		//_float fCurTexTimer = 0.f;
		////�帣�� UV�� ���� ����
		//_vec2 vCurUV = { 0.f,0.f };
	}TEX_EFFECT_INFO;

#define PARTICLE_EFFECTOP_USE_GRAVITIY 0x00000001
#define PARTICLE_EFFECTOP_USE_MOTIONBLUR 0x00000002
#define PARTICLE_EFFECTOP_USE_RANDOMIZE_TEXANIM 0x00000004
#define PARTICLE_EFFECTOP_USE_POSTPROCESS 0x00000008
	//��ƼŬ ��ü�� ����Ǵ� ����
	typedef struct tagParticleEffectInfo
	{
		/*velocity
			accel
			gravitiy ��� ����
			Create Amount
			Max Create
			create frequency
			particle Motion blur(�̰� ���� ����)
			Create Pos*/
		DWORD dwParticleState = 0;

		///////////////////////////////////////////////////////////
		//��ƼŬ �̵� ����
		//�̵�
		_vec3 vVelocityMax = { 1.f,1.f,1.f };
		_vec3 vVelocityMin = { -1.f,-1.f,-1.f };
		//ũ��
		_vec3 vDefaultScale = { 1.f,1.f,1.f };
		_vec3 vScaling = {0.f,0.f,0.f};
		//���� ��ġ
		_vec3 vPosMax = { 0.f,0.f,0.f };
		_vec3 vPosMin = { 0.f,0.f,0.f };
		//����
		_vec4 vColorMax = { 1.f,1.f,1.f,1.f };
		_vec4 vColorMin = { 1.f,1.f,1.f,1.f };
		//���� �ٲ�� ��
		_vec4 vColorFadeMax = { 0.f,0.f,0.f,0.f };
		_vec4 vColorFadeMin = { 0.f,0.f,0.f,0.f };
		//Velocity�� ���ӵǴ� ��
		_float fAccel = 1.f;
		//Life
		_float fMinLife= 1.f;
		_float fMaxLife = 0.f;
		//�߷� ���� ��
		_vec3 vGravitiyLevel = {0.f,0.f,0.f};
		///////////////////////////////////////////////////////////
		//�ѹ��� �����Ǵ� ��
		_int iCreateAmount = 0;
		_float fCreateFre = 1.f;

	}PARTICLE_EFFECT_INFO;

	//�� ��ƼŬ�� �����ӿ� ���� ����
	typedef struct tagParticleAttribute
	{
		_vec3 vVelocity = { 0.f,0.f,0.f };
		_vec3 vScale = { 1.f,1.f,1.f };
		_vec3 vPos = { 0.f,0.f,0.f };
		_vec4 vColor = { 1.f,1.f,1.f,1.f };
		_vec4 vColorFade = { 0.f,0.f,0.f,0.f };
		//�߷� ���� �ð�(1�� �ִ��߷��� �ǰ� ����)
		_float fGravitiyTimer = 0.f;

		_float fLife = 1.f;
		///////////////////////////////////////////////////////////
		//UV �ִϸ��̼� ����
		//TexAnimChange �ÿ� UVSize�� ���ؼ� ���� �ִϸ��̼����� �Ѿ�� �����
		_vec2 vUVIndex = { 0.f,0.f };
		_int iUVIndexX = 0;
		_int iUVIndexY = 0;

		_float fCurTexTimer = 0.f;
		//�帣�� UV�� ���� ����
		_vec2 vCurUV = { 0.f,0.f };

		_float fViewZ = 1.f;
	}PARTICLE_ATT;
	typedef struct tagMeshEffectAttribute
	{
		//Velocity�� ���ӵǴ� ��
		_float fAccel = 1.f;
		//�̵�
		_vec3 vVelocity = { 0.f,0.f,0.f };
		//ũ��
		_vec3 vScaling = { 0.f,0.f,0.f };
		//ȸ��(Z�� ����)
		_vec3 vRotation = { 0.f,0.f,0.f };
		//����
		_vec4 vColor = { 1.f,1.f,1.f,1.f };
		_vec4 vColorFade = { 0.f,0.f,0.f,0.f };
		//�߷� ���� ��
		_vec3 vGravitiyLevel = { 0.f,0.f,0.f };
		//�߷� ���� �ð�(1�� �ִ��߷��� �ǰ� ����)
		_float fGravitiyTimer = 0.f;

		//UV �ִϸ��̼� ����
		//TexAnimChange �ÿ� UVSize�� ���ؼ� ���� �ִϸ��̼����� �Ѿ�� �����
		_int iCurUVIndexX = 1;
		_int iCurUVIndexY = 1;

		_float fCurTexTimer = 0.f;
		_vec2 vCurUV = { 0.f,0.f };
		
		_vec2 vCurDistortion = {0.f,0.f};
		//�� ������ ���� ��ġ�� ���󰣴�
		_vec3* pLockPos = nullptr;
	}EFFECTMESH_ATT;

	typedef struct tagScreenEffectAttribute
	{
		//Projection���� ����
		_vec2 vImagePos = {0.f,0.f};
		//Projection���� ũ��
		_vec2 vSize = {0.1f,0.1f};
		//����
		_vec4 vColor = { 1.f,1.f,1.f,1.f };
		_vec4 vColorFade = { 0.f,0.f,0.f,0.f };

		_int iCurUVIndexX = 0;
		_int iCurUVIndexY = 0;

		_float fCurTexTimer = 0.f;
		_vec2 vCurUV = { 0.f,0.f };
	}SCREEN_ATT;
	typedef struct tagIntVector3
	{
		int x;
		int y;
		int z;
	}_IntVec3;

	typedef struct tagIntVector2
	{
		int x;
		int z;
	}_IntVec2;


	typedef struct GodRayInfo
	{
		_vec3 vPos = {0.f,0.f,0.f};
		//Decay�� �ر� -> ���跮�ε�?
		float fDecay = 0.96815f;
		//���ⷮ, �Ƹ� �� ���� �ε�
		float fExposure = 0.2f;
		//�е�, �ִ� ����?
		float fDensity = 0.926f;
		//����ġ
		float fWeight = 0.58767f;
	}GOD_RAY_INFO;


	typedef struct UITexture1
	{
		UITexture1() {}
		UITexture1(float fSizeX, float fSizeY, float fx, float fy,float Rot, bool IsSecondFont,wstring FirstFont,wstring SecondFont, D3DXVECTOR2 FirstFontPos, D3DXVECTOR2 SecondFontPos, D3DXCOLOR Color)
			:fSizeX(fSizeX), fSizeY(fSizeY),fx(fx),fy(fy),Rot(Rot), IsSecondFont(IsSecondFont), FirstFont(FirstFont), SecondFont(SecondFont),FirstFontPos(FirstFontPos), SecondFontPos(SecondFontPos),Color(Color)
			{
		}
		
		UITexture1(float fSizeX, float fSizeY, float fx, float fy, float Rot)
			:fSizeX(fSizeX), fSizeY(fSizeY), fx(fx), fy(fy), Rot(Rot)
		{
			IsSecondFont=false;

			 FirstFont=L"";
			SecondFont=L"";

			FirstFontPos = {0.f,0.f};
			SecondFontPos = {0.f,0.f};

			Color = {0.f,0.f,0.f,0.f};

		}


		float fSizeX;
		float fSizeY;
		float fx;
		float fy;
		float Rot;

		bool  IsSecondFont;

		wstring FirstFont;
		wstring SecondFont;

		D3DXVECTOR2 FirstFontPos;
		D3DXVECTOR2 SecondFontPos;

		D3DXCOLOR Color;

	}UITexture;



}

#endif // ENGINE_Struct_h__
