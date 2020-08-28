#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(ENGINE)
class CTexture;
class ENGINE_DLL CDynamicMesh : public CMesh
{


private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	const D3DXFRAME_DERIVED*				Get_FrameByName(const char* pFrameName);
	D3DXFRAME*								Get_RootFrame();
	CAniCtrl*								Get_AniCtrl() { return m_pAniCtrl; }
	_bool									Is_AnimationSetFinish(_double dfAdjTime);
	_bool									Is_AnimationSetFinishFromRatio(_double dfAdjTime);


public:
	HRESULT									Ready_Meshes(wstring wstrFilePath, wstring wstrFileName, _bool bIsPlayer = false);
	void									Render_Meshes(void);

	void									Render_Meshes(LPD3DXEFFECT& pEffect, SUBSET::RENDER eRenderSel);
	void									Render_Meshe_All(LPD3DXEFFECT& pEffect);



	void									Set_AnimationSet(const _uint& iIndex);
	void									Set_CoreciveAnimationSet(const _uint& iIndex);

	vector<D3DXMESHCONTAINER_DERIVED*>*		Get_MeshContainer() { return &m_vecMeshContainer; }
	void									Set_MeshContainer(vector<D3DXMESHCONTAINER_DERIVED*>* pMeshContainer) { m_vecMeshContainer = *pMeshContainer; }
	void									Set_RootFrame(D3DXFRAME* pRootFrame) { m_pRootFrame = pRootFrame; }


	void									Play_AnimationSet(const _float& fTimeDelta);
	_bool									GetCheckHardwareSkinning() { return m_bCheckHardwareSkinning; }
	HRESULT									Set_CustomTexture(string strSubSetName, TEXTURE::SHADER eTexType, CTexture* pTexture, _uint iTexIndex = 0);


private:
	void									Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void									SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	CHierarchyLoader*						m_pLoader;
	CAniCtrl*								m_pAniCtrl;
	D3DXFRAME*								m_pRootFrame;
	vector<D3DXMESHCONTAINER_DERIVED*>		m_vecMeshContainer;

	_bool									m_bCheckHardwareSkinning;

public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFilePath, wstring wstrFileName);
	virtual CComponent*			Clone(void * pArg);
	virtual void				Free(void);



};

END
#endif // DynamicMesh_h__

// 뼈 정보를 담아두는 구조체

//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;					// 뼈의 이름을 저장하는 문자열(아스키 코드)
//	D3DXMATRIX              TransformationMatrix;	// 최초 로드 시점에 뼈가 지닌 상태 행렬 값
//
//	LPD3DXMESHCONTAINER     pMeshContainer;			// 실제적으로 스태틱 메쉬
//
//	struct _D3DXFRAME       *pFrameSibling;			// 형제 뼈대의 첫 번째 주소
//	struct _D3DXFRAME       *pFrameFirstChild;		// 자식 뼈대의 첫 번째 주소
//} D3DXFRAME, *LPD3DXFRAME;

// 메쉬를 구성하는 각종 요소를 보관하는 구조체

//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;			// 메쉬 컨테이너의 이름을 보관하는 문자열(사실상 거의 사용하지 않음)
//
//	D3DXMESHDATA            MeshData;		// 생성하고자 하는 메쉬의 옵션을 결정하는 구조체
//
//	LPD3DXMATERIAL          pMaterials;		// 재질 정보를 담당하는 구조체 포인터(재질에 대한 색상 정보를 저장하기 위한 변수, 텍스쳐 이름)
//	LPD3DXEFFECTINSTANCE    pEffects;		// 메쉬 컨테이너가 보관하고 있는 독특한 이펙트 정보를 저장하는 포인터(하지만 우리는 사용할 수 없다)
//	DWORD                   NumMaterials;	// 재질의 개수
//	DWORD                  *pAdjacency;		// 인접한 정보의 첫번째 주소를 보관하기 위한 포인터
//
//	LPD3DXSKININFO          pSkinInfo;		// 스키닝 애니메이션에 관련된 각종 기능을 제공하는 컴 객체
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// 다음 메쉬 컨테이너의 주소값을 저장하는 포인터(그러나 우리가 직접 메쉬 컨테이너의 다음 정보를 참조하거나 탐색하는 일은 거의 없다)
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;

// 메쉬의 타입을 결정하는 구조체
//typedef struct _D3DXMESHDATA
//{
//	D3DXMESHDATATYPE Type;			// 어떤 메쉬를 사용할 것인지 결정하는 열거체
//
//	// current mesh data interface
//	union
//	{
//		LPD3DXMESH              pMesh;			// 우리는 보편적인 노말 메쉬를 사용한다
//		LPD3DXPMESH             pPMesh;
//		LPD3DXPATCHMESH         pPatchMesh;
//	};
//} D3DXMESHDATA, *LPD3DXMESHDATA;
