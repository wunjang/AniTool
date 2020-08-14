#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	 _vec3*				Get_VtxPos(void) const {	return m_pPos;	}
	_ulong				Get_VtxCntX(void) const { return m_dwCntX; }
	_ulong				Get_VtxCntZ(void) const { return m_dwCntZ; }

public:
	void		Copy_Index(ENGINE::INDEX32* pIndex, const _ulong& dwTriCnt);
	

public:
	HRESULT	Ready_Buffer(
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv,
		const _float& dwDetail);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	_uint				m_dwCntX;
	_uint				m_dwCntZ;

	_vec3*				m_pPos;
	_bool				m_bClone;


public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _ulong& dwCntX,
									const _ulong& dwCntZ, 
									const _ulong& dwVtxItv,
									const _float& fDetail);

	virtual CResources*		Clone(void);
	virtual void			Free(void);
};

END
#endif // TerrainTex_h__
