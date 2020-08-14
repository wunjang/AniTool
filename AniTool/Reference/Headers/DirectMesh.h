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
//	LPD3DXBUFFER				m_pAdjacency;	// 이웃하는 정점(폴리곤) 정보를 보관하기 위한 주소값(포인터)
//	LPD3DXBUFFER				m_pSubsetBuffer; // 서브셋 정보를 저장하기 위한 포인터
//
//	D3DXMATERIAL*				m_pSubsets;		// 재질 정보를 보관하기 위한 구조체 포인터
//	_ulong						m_dwSubsetCnt;	// 서브셋 개수 == 텍스쳐 개수 == 재질의 개수
//
//	LPDIRECT3DTEXTURE9*			m_ppTexture;	// 여러 개의 텍스쳐 정보를 보관하기 위해 이중포인터
//
//
//
//	LPD3DXMESH					m_pOriMesh;		// x파일에서 로드하여 생성한 최초의 메쉬를 대표하는 컴 객체
//	LPD3DXMESH					m_pMesh;		// 만약 노말정보가 없는 메쉬의 경우 노말 값을 삽입하여 새로 생성해주는 메쉬 컴 객체
//	_ulong						m_dwNumVtx;		// 버텍스의 개수
//	_ulong						m_dwStride;		// 정점의 포지션과 포지션 간의 메모리 간격을 저장하기 위한 변수
//	_vec3*						m_pVtxPos;		// 메쉬 정점의 정보 중 모든 포지션 값을 얻어와서 저장하기 위한 포인터
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
