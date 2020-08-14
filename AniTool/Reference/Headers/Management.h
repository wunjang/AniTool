#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Scene.h"

BEGIN(ENGINE)

class ENGINE_DLL CManagement : public CBase 
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement(void);
	virtual		~CManagement(void);
public:
	typedef struct tagLateAddObj
	{
		tagLateAddObj()
		{
		}
		tagLateAddObj(LAYER_TYPE eInputLayerType, const  OBJ_ID eInputObjectID, CGameObject* pInputObject)
			: eLayerType(eInputLayerType)
			, eObjectID(eInputObjectID)
			, pObject(pInputObject)
		{
		}
		LAYER_TYPE		eLayerType;
		OBJ_ID			eObjectID;
		CGameObject*	pObject;

	}LateAddObj;
public:
	CGameObject*			Get_GameObject(
		LAYER_TYPE eType,
		const  OBJ_ID eObjectID,
		_int iIndex = 0
	);

	_int		Get_GameObjectCnt(
		LAYER_TYPE eType,
		const  OBJ_ID eObjectID
	);

	CComponent*			Get_Component(
		LAYER_TYPE eType,
		const  OBJ_ID eObjectID,
		const COMPONENT::TAG eComponentTag,
		COMPONENT::ID eID);

	HRESULT Add_GameObject(
		LAYER_TYPE eLayerType,
		const  OBJ_ID eObjectID,
		CGameObject* pObject);

	HRESULT LateAdd_GameObject(
		LAYER_TYPE eLayerType,
		const  OBJ_ID eObjectID,
		CGameObject* pObject);


public:
	HRESULT		Ready_Shader(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT		SetUp_Scene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	_int		LateUpdate_Scene(const _float& fTimeDelta);
	void		Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);

private:
	CScene*							m_pScene = nullptr;

	list<LateAddObj>	m_AddObjList;
public:
	virtual void	Free(void);

};

END
#endif // Management_h__
