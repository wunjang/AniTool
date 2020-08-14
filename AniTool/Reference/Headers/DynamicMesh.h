#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(ENGINE)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	const D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
	const D3DXFRAME*				Get_RootFrame();
	CAniCtrl*						Get_AniCtrl() { return m_pAniCtrl; }
	_bool							Is_AnimationSetFinish(_double dfAdjTime);
	_bool							Is_AnimationSetFinishFromRatio(_double dfAdjTime);


public:
	HRESULT		Ready_Meshes(wstring wstrFilePath, wstring wstrFileName, _bool bIsPlayer = false);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);

	void		Set_AnimationSet(const _uint& iIndex);
	void		Play_AnimationSet(const _float& fTimeDelta);

	_bool		GetCheckHardwareSkinning() { return m_bCheckHardwareSkinning; }
private:
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);
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

// �� ������ ��Ƶδ� ����ü

//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;					// ���� �̸��� �����ϴ� ���ڿ�(�ƽ�Ű �ڵ�)
//	D3DXMATRIX              TransformationMatrix;	// ���� �ε� ������ ���� ���� ���� ��� ��
//
//	LPD3DXMESHCONTAINER     pMeshContainer;			// ���������� ����ƽ �޽�
//
//	struct _D3DXFRAME       *pFrameSibling;			// ���� ������ ù ��° �ּ�
//	struct _D3DXFRAME       *pFrameFirstChild;		// �ڽ� ������ ù ��° �ּ�
//} D3DXFRAME, *LPD3DXFRAME;

// �޽��� �����ϴ� ���� ��Ҹ� �����ϴ� ����ü

//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;			// �޽� �����̳��� �̸��� �����ϴ� ���ڿ�(��ǻ� ���� ������� ����)
//
//	D3DXMESHDATA            MeshData;		// �����ϰ��� �ϴ� �޽��� �ɼ��� �����ϴ� ����ü
//
//	LPD3DXMATERIAL          pMaterials;		// ���� ������ ����ϴ� ����ü ������(������ ���� ���� ������ �����ϱ� ���� ����, �ؽ��� �̸�)
//	LPD3DXEFFECTINSTANCE    pEffects;		// �޽� �����̳ʰ� �����ϰ� �ִ� ��Ư�� ����Ʈ ������ �����ϴ� ������(������ �츮�� ����� �� ����)
//	DWORD                   NumMaterials;	// ������ ����
//	DWORD                  *pAdjacency;		// ������ ������ ù��° �ּҸ� �����ϱ� ���� ������
//
//	LPD3DXSKININFO          pSkinInfo;		// ��Ű�� �ִϸ��̼ǿ� ���õ� ���� ����� �����ϴ� �� ��ü
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// ���� �޽� �����̳��� �ּҰ��� �����ϴ� ������(�׷��� �츮�� ���� �޽� �����̳��� ���� ������ �����ϰų� Ž���ϴ� ���� ���� ����)
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;

// �޽��� Ÿ���� �����ϴ� ����ü
//typedef struct _D3DXMESHDATA
//{
//	D3DXMESHDATATYPE Type;			// � �޽��� ����� ������ �����ϴ� ����ü
//
//	// current mesh data interface
//	union
//	{
//		LPD3DXMESH              pMesh;			// �츮�� �������� �븻 �޽��� ����Ѵ�
//		LPD3DXPMESH             pPMesh;
//		LPD3DXPATCHMESH         pPatchMesh;
//	};
//} D3DXMESHDATA, *LPD3DXMESHDATA;
