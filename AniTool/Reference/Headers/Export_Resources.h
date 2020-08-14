#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourcesMgr.h"

BEGIN(ENGINE)

// ResourcesMgr
inline HRESULT	Reserve_ContainerSize(const _ushort& wSize);

inline HRESULT	Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pBufferTag,
	BUFFERID eID,
	const _ulong& dwCntX = 1,
	const _ulong& dwCntZ = 1,
	const _ulong& dwVtxItv = 1,
	const _float& fDetail = 1);

inline HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pTextureTag,
	TEXTURETYPE eType,
	const _tchar* pPath,
	const _uint& iCnt = 1);

inline HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pMeshTag,
	MESHTYPE eType,
	const _tchar* pFilePath,
	const _tchar* pFileName);

inline void		Render_Texture(const _ushort& wContainerIdx,
	const _tchar* pTextureTag,
	const _uint& iIndex = 0);

inline void		Render_Buffer(const _ushort& wContainerIdx,
	const _tchar* pBufferTag);

inline CResources*		Clone(const _ushort& wContainerIdx,
	const _tchar* pResourceTag);

inline bool		CheckResouce(const _ushort& wContainerIdx,
							const _tchar* pResourceTag);

inline const D3DXFRAME*		Get_RootFrame(const _ushort& wContainerIdx,
									const _tchar* pResourceTag);

// Release Resources
inline void Release_Resources();

#include "Export_Resources.inl"

END

#endif // Export_Resources_h__
