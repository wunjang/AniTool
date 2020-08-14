#include "Export_Resources.h"

// ResourcesMgr
HRESULT	Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
 }

HRESULT	Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pBufferTag,
	BUFFERID eID,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv,
	const _float& fDetail
	)
{
	return CResourcesMgr::GetInstance()->Ready_Buffers(pGraphicDev, wContainerIdx, pBufferTag, eID, dwCntX, dwCntZ, dwVtxItv, fDetail);
}

HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pTextureTag,
	TEXTURETYPE eType,
	const _tchar* pPath,
	const _uint& iCnt)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}

HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pMeshTag,
	MESHTYPE eType,
	const _tchar* pFilePath,
	const _tchar* pFileName)
{
	return CResourcesMgr::GetInstance()->Ready_Meshes(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}

void		Render_Texture(const _ushort& wContainerIdx, const _tchar* pTextureTag, const _uint& iIndex)
{
	CResourcesMgr::GetInstance()->Render_Texture(wContainerIdx, pTextureTag, iIndex);
 }

void		Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, pBufferTag);
 }

CResources*		Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	return CResourcesMgr::GetInstance()->Clone(wContainerIdx, pResourceTag);
}

inline bool CheckResouce(const _ushort & wContainerIdx, const _tchar * pResourceTag)
{
	return CResourcesMgr::GetInstance()->CheckResouce(wContainerIdx, pResourceTag);
}

inline const D3DXFRAME * Get_RootFrame(const _ushort & wContainerIdx, const _tchar * pResourceTag)
{
	return CResourcesMgr::GetInstance()->Get_RootFrame(wContainerIdx, pResourceTag);
}

// Release Resources
void Release_Resources()
{
	CResourcesMgr::GetInstance()->DestroyInstance();
}