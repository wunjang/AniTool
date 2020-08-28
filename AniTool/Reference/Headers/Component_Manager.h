#pragma once

// 1. 원형객체들을 보관한다.
// 2. 원형객체를 찾아서 복제한 후, 리턴한다.

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"

//#include "Mesh_Static.h"
#include "ParticleBuffer.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Circle.h"
#include "VIBuffer_Terrain.h"
#include "DynamicMesh.h"
#include "StaticMesh.h"

BEGIN(ENGINE)

class ENGINE_DLL CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager();
public:
	HRESULT Reserve_Component_Manager(_uint iNumScenes);
	HRESULT Add_Prototype(_uint iSceneID, const wstring wstrPrototypeTag, CComponent* pPrototype);
	HRESULT Add_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
						_uint iSceneID,
						const wstring wstrPrototypeTag,
						wstring wstrPath,
						TEXTURE::TYPE tTexType = TEXTURE::TYPE::TYPE_GENERAL,
						const _uint& iCnt = 1);
	CComponent* Clone_Component(_uint iSceneID, const wstring wstrPrototypeTag, void* pArg);
	HRESULT Clear_Component_Manager(_uint iSceneID);

	//Mesh 관련
	HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ushort& wContainerIdx,
		wstring wstrMeshTag,
		MESHTYPE eType,
		wstring wstrFilePath,
		wstring wstrFileName);

	HRESULT		Ready_AllMesh(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath);
	HRESULT		Ready_AllTexture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath);
	HRESULT		Ready_AllCubeTexture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath);
private:
	_uint		m_iNumScenes = 0;
private:
	map<const wstring, CComponent*>*		m_pPrototypes = nullptr;
	typedef map<const wstring, CComponent*>	PROTOTYPES;
private:
	CComponent* Find_Component(_uint iSceneID, const wstring wstrPrototypeTag);
public:
	virtual void Free();
};


END