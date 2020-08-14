//#ifndef DirectMesh_h__
//#define DirectMesh_h__
//
//#include "Mesh.h"
//
//BEGIN(ENGINE)
//
//class ENGINE_DLL CDirectMesh : public CMesh
//{
//private:
//	explicit CDirectMesh(LPDIRECT3DDEVICE9 pGraphicDev);
//	explicit CDirectMesh(const CDirectMesh& rhs);
//	virtual ~CDirectMesh(void);
//
//public:
//	_ulong	Get_NumVtx(void) const { return m_dwNumVtx; }
//	_ulong	Get_Stride(void) { return m_dwStride; }
//	const _vec3*		Get_VtxPos(void) const { return m_pVtxPos; }
//	const LPD3DXMESH    Get_Mesh(void)const { return m_pMesh; }
//
//public:
//	HRESULT		Ready_Meshes(const DIRECT_MESH eMeshType, const _tchar* pFileName,float fMin=0,float fMax=0,float fRadious=0,float);
//	void		Render_Meshes(void);
//
//private:
//	
//	LPD3DXBUFFER				m_pAdjacency;	// �̿��ϴ� ����(������) ������ �����ϱ� ���� �ּҰ�(������)
//	LPD3DXBUFFER				m_pSubsetBuffer; // ����� ������ �����ϱ� ���� ������
//
//	D3DXMATERIAL*				m_pSubsets;		// ���� ������ �����ϱ� ���� ����ü ������
//	_ulong						m_dwSubsetCnt;	// ����� ���� == �ؽ��� ���� == ������ ����
//
//	LPDIRECT3DTEXTURE9*			m_ppTexture;	// ���� ���� �ؽ��� ������ �����ϱ� ���� ����������
//
//
//
//	LPD3DXMESH					m_pOriMesh;		// x���Ͽ��� �ε��Ͽ� ������ ������ �޽��� ��ǥ�ϴ� �� ��ü
//	LPD3DXMESH					m_pMesh;		// ���� �븻������ ���� �޽��� ��� �븻 ���� �����Ͽ� ���� �������ִ� �޽� �� ��ü
//	_ulong						m_dwNumVtx;		// ���ؽ��� ����
//	_ulong						m_dwStride;		// ������ �����ǰ� ������ ���� �޸� ������ �����ϱ� ���� ����
//	_vec3*						m_pVtxPos;		// �޽� ������ ���� �� ��� ������ ���� ���ͼ� �����ϱ� ���� ������
//
//
//public:
//	static	CDirectMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
//										const _tchar* pFilePath,
//										const _tchar* pFileName);
//public:
//	virtual CResources*			Clone(void);
//private:
//	virtual void				Free(void);
//};
//
//END
//#endif // StaticMesh_h__
