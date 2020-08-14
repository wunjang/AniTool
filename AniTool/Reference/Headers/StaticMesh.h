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

	//�� �ؽ�ó�� ä��� �뵵��
	void		Set_NormalTexture(LPDIRECT3DTEXTURE9 pTexture,_int iIndex);
	void		Set_AlbedoTexture(LPDIRECT3DTEXTURE9 pTexture);
private:
	
	LPD3DXBUFFER				m_pAdjacency;	// �̿��ϴ� ����(������) ������ �����ϱ� ���� �ּҰ�(������)
	LPD3DXBUFFER				m_pSubsetBuffer; // ����� ������ �����ϱ� ���� ������

	D3DXMATERIAL*				m_pSubsets;		// ���� ������ �����ϱ� ���� ����ü ������
	_ulong						m_dwSubsetCnt;	// ����� ���� == �ؽ��� ���� == ������ ����

	LPDIRECT3DTEXTURE9*			m_ppTexture;	// ���� ���� �ؽ��� ������ �����ϱ� ���� ����������
	LPDIRECT3DTEXTURE9*			m_ppNormalTexture;
	LPDIRECT3DTEXTURE9*			m_ppEmissiveTexture;
	LPDIRECT3DTEXTURE9*			m_ppRoughnessTexture;
	LPDIRECT3DTEXTURE9*			m_ppMetallicTexture;

	LPD3DXMESH					m_pOriMesh;		// x���Ͽ��� �ε��Ͽ� ������ ������ �޽��� ��ǥ�ϴ� �� ��ü
	LPD3DXMESH					m_pMesh;		// ���� �븻������ ���� �޽��� ��� �븻 ���� �����Ͽ� ���� �������ִ� �޽� �� ��ü
	_ulong						m_dwNumVtx;		// ���ؽ��� ����
	_ulong						m_dwStride;		// ������ �����ǰ� ������ ���� �޸� ������ �����ϱ� ���� ����
	_vec3*						m_pVtxPos;		// �޽� ������ ���� �� ��� ������ ���� ���ͼ� �����ϱ� ���� ������


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
