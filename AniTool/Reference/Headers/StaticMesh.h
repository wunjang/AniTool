#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(ENGINE)
class CTexture;
class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	_ulong				Get_NumVtx(void) const { return m_dwNumVtx; }
	_ulong				Get_Stride(void) { return m_dwStride; }
	const _vec3*		Get_VtxPos(void) const { return m_pVtxPos; }
	const LPD3DXMESH    Get_Mesh(void)const { return m_pMesh; }
	const LPD3DXBUFFER&		Get_Adjacency() { return m_pAdjacency; }

	void CreateSubTexture(const _tchar* pFilePath, _int iExtension, wstring wstrCreateTextureKey, LPDIRECT3DTEXTURE9* ppTexture);

public:
	HRESULT		Ready_Meshes(wstring wstrFilePath, wstring wstrFileName);

	void		Render_Meshe_All(LPD3DXEFFECT& pEffect);
	void		Render_Meshes(LPD3DXEFFECT& pEffect, SUBSET::RENDER eRenderSel);
	void		Render_Subset(LPD3DXEFFECT& pEffect, _int iSubsetNum,_bool bCheckSet_OutSide = false);

	void		Render_Meshes_Set_OutSide(LPD3DXEFFECT& pEffect, SUBSET::RENDER eRenderSel);
	void		Render_PostProcess(LPD3DXEFFECT& pEffect);

	_bool		Set_Texture(wstring wstrFileName);

	HRESULT		Set_CustomTexture(_uint iSubSetIndex, TEXTURE::SHADER eTexType, CTexture* pTexture, _uint iTexIndex = 0);
	vector<CTexture*>*	Get_Textrue(TEXTURE::SHADER eType) { return &m_vecTexture[eType]; }


private:
	
	LPD3DXBUFFER				m_pAdjacency;	// 이웃하는 정점(폴리곤) 정보를 보관하기 위한 주소값(포인터)
	LPD3DXBUFFER				m_pSubsetBuffer; // 서브셋 정보를 저장하기 위한 포인터
	D3DXMATERIAL*				m_pSubsets;		// 재질 정보를 보관하기 위한 구조체 포인터

	_ulong						m_dwSubsetCnt;	// 서브셋 개수 == 텍스쳐 개수 == 재질의 개수

	vector<CTexture*>			m_vecOriginTexture[TEXTURE::SHADER::SHADER_END];
	vector<CTexture*>			m_vecTexture[TEXTURE::SHADER::SHADER_END];


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
