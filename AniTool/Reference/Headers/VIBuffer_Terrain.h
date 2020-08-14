#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class CTransform;
class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain(void) = default;
public:
	const _vec3* Get_VtxPos(void) const {	
		return m_pPosition;	}
	_float Get_Height(CTransform* pTransform);
public:
	HRESULT	Ready_Component_Prototype(_ulong dwCntX, _ulong dwCntZ, _ulong dwVtxItv);	
	HRESULT	Ready_Component_Prototype(const _tchar* pFilePath, _ulong dwVtxItv);
	virtual HRESULT Ready_Component(void* pArg);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fh;
	BITMAPINFOHEADER	m_ih;
	_vec3*				m_pPosition = nullptr;
	_ulong				m_dwInterval = 0;
	_ulong				m_dwVtxCntX = 0;
	_ulong				m_dwVtxCntZ = 0;

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwCntX, _ulong dwCntZ, _ulong dwVtxItv);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, _ulong dwVtxItv);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void);

	
};

END
#endif // TerrainTex_h__
