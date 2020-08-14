#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

#include "Resources.h"
#include "TriCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "Dynamic_RcTex.h"
BEGIN(ENGINE)

class ENGINE_DLL CResourcesMgr : public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr(void);
	virtual ~CResourcesMgr(void);

public:
	HRESULT	Reserve_ContainerSize(const _ushort& wSize);
	
	HRESULT	Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ushort& wContainerIdx,
							const _tchar* pBufferTag,
							BUFFERID eID, 
							const _ulong& dwCntX = 1, 
							const _ulong& dwCntZ = 1, 
							const _ulong& dwVtxItv = 1,
							const _float& fDetail = 1);

	HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								const _tchar* pTextureTag,
								TEXTURETYPE eType,
								const _tchar* pPath,
								const _uint& iCnt = 1);

	HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								const _tchar* pMeshTag,
								MESHTYPE eType,
								const _tchar* pFilePath,
								const _tchar* pFileName);


	void		Render_Texture(const _ushort& wContainerIdx, 
								const _tchar* pTextureTag,
								const _uint& iIndex = 0);
		
	void		Render_Buffer(const _ushort& wContainerIdx,
								const _tchar* pBufferTag);

	CResources*		Clone(const _ushort& wContainerIdx, 
						  const _tchar* pResourceTag);

	bool			CheckResouce(const _ushort& wContainerIdx,
								const _tchar* pResourceTag);

	const D3DXFRAME*		Get_RootFrame(const _ushort& wContainerIdx,
													const _tchar* pResourceTag);

private:
	CResources*	Find_Resources(const _ushort& wContainerIdx,
							   const _tchar* pResourcesTag);

private:
	map<const _tchar*, CResources*>*		m_pmapResources = nullptr;
	_ushort									m_wContainerSize = 0;

public:
	virtual void Free(void);

};

END
#endif // ResourcesMgr_h__
