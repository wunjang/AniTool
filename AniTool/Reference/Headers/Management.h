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

	HRESULT LateInitialize(void);



	CScene*		Get_Scene();
public:
	HRESULT		SetUp_Scene(CScene* pScene);
	HRESULT		Release_Scene();

	_int		Update_Scene(const _float& fTimeDelta);
	_int		LateUpdate_Scene(const _float& fTimeDelta);
	void		Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev, const _float& fTimeDelta);

private:
	CScene*							m_pScene = nullptr;

	list<CGameObject*>	m_LateInitList;
public:
	virtual void	Free(void);

};

END
#endif // Management_h__
