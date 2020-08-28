#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(ENGINE)
class CLayer;
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene(void);

public:
	CGameObject*			Get_GameObject(
		LAYER_TYPE eType,
		const  OBJ_ID eObjectID,
		_int iIndex = 0);

	_int		Get_GameObjectCnt(
		LAYER_TYPE eType,
		const  OBJ_ID eObjectID
	);

	CComponent*			Get_Component(
		LAYER_TYPE eType,
		const  OBJ_ID eObjectID,
		const COMPONENT::TAG eComponentTag,
		COMPONENT::ID eID);

	HRESULT Add_GameObject(LAYER_TYPE eLayerType,
		const  OBJ_ID eObjectID,
		CGameObject* pObject);

	CLayer* Get_Layer(LAYER_TYPE eType);

	

public:
	virtual HRESULT		Ready_Scene(void);
	virtual HRESULT		LateInitialize(void);
	virtual HRESULT		Enter_Scene(void)PURE;


	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual _int		LateUpdate_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(const _float& fTimeDelta)PURE;
	virtual	HRESULT		Ready_Light(void)PURE;
protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;

	//typedef unordered_map<LAYER_TYPE, CLayer*>	MAP_LAYER;
	typedef map<LAYER_TYPE, CLayer*>	MAP_LAYER;
	MAP_LAYER m_Layers;

public:
	virtual			void Free(void);

};


END
#endif // Scene_h__
