#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(ENGINE)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	_ulong	Get_NumVtx(void) const { return m_dwNumVtx; }
	_ulong	Get_Stride(void) { return m_dwStride; }
	const _vec3*		Get_VtxPos(void) const { return m_pVtxPos; }
	const LPD3DXMESH    Get_Mesh(void)const { return m_pMesh; }
	_bool Find_Alpha(const char* pFileName);

	void CreateSubTexture(const _tchar* pFilePath, _int iExtension, wstring wstrCreateTextureKey, LPDIRECT3DTEXTURE9* ppTexture);

public:
	HRESULT		Ready_Meshes(wstring wstrFilePath, wstring wstrFileName);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);

	void		Render_Meshes_Set_OutSide(LPD3DXEFFECT& pEffect);

	void		Render_PostProcess(LPD3DXEFFECT& pEffect);

	//빈 텍스처를 채우는 용도다
	void		Set_NormalTexture(LPDIRECT3DTEXTURE9 pTexture,_int iIndex);
	void		Set_AlbedoTexture(LPDIRECT3DTEXTURE9 pTexture);
private:
	
	LPD3DXBUFFER				m_pAdjacency;	// 이웃하는 정점(폴리곤) 정보를 보관하기 위한 주소값(포인터)
	LPD3DXBUFFER				m_pSubsetBuffer; // 서브셋 정보를 저장하기 위한 포인터

	D3DXMATERIAL*				m_pSubsets;		// 재질 정보를 보관하기 위한 구조체 포인터
	_ulong						m_dwSubsetCnt;	// 서브셋 개수 == 텍스쳐 개수 == 재질의 개수

	LPDIRECT3DTEXTURE9*			m_ppTexture;	// 여러 개의 텍스쳐 정보를 보관하기 위해 이중포인터
	LPDIRECT3DTEXTURE9*			m_ppNormalTexture;
	LPDIRECT3DTEXTURE9*			m_ppEmissiveTexture;
	LPDIRECT3DTEXTURE9*			m_ppRoughnessTexture;
	LPDIRECT3DTEXTURE9*			m_ppMetallicTexture;

	LPD3DXMESH					m_pOriMesh;		// x파일에서 로드하여 생성한 최초의 메쉬를 대표하는 컴 객체
	LPD3DXMESH					m_pMesh;		// 만약 노말정보가 없는 메쉬의 경우 노말 값을 삽입하여 새로 생성해주는 메쉬 컴 객체
	_ulong						m_dwNumVtx;		// 버텍스의 개수
	_ulong						m_dwStride;		// 정점의 포지션과 포지션 간의 메모리 간격을 저장하기 위한 변수
	_vec3*						m_pVtxPos;		// 메쉬 정점의 정보 중 모든 포지션 값을 얻어와서 저장하기 위한 포인터


public:
	static	CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										wstring wstrFilePath,
										wstring wstrFileName);
public:
	virtual CComponent*			Clone(void * pArg);
private:
	virtual void				Free(void);
};

END
#endif // StaticMesh_h__
