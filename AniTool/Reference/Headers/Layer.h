#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(ENGINE)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	CGameObject*			Get_GameObject(
		const  OBJ_ID eObjectID,
		int iIndex = 0);

	_int			Get_GameObjectCnt(
		const  OBJ_ID eObjectID
	);
	
	CComponent*			Get_Component(
		const  OBJ_ID eObjectID,
		const COMPONENT::TAG eComponentTag,
		COMPONENT::ID eID,
		_int iIndex = 0);

public:
	HRESULT				Add_GameObject(
		const OBJ_ID eObjectID,
		CGameObject* pObject);


	HRESULT				Ready_Layer(void);
	HRESULT				LateInitialize(void);

	_int				Update_Layer(const _float& fTimeDelta);
	_int				LateUpdate_Scene(const _float& fTimeDelta);

	HRESULT				Export(OBJ_ID eID, CGameObject* pObj);
private:
	typedef list<CGameObject*>					OBJECT_LIST;
	typedef map<OBJ_ID, OBJECT_LIST>	MAP_OBJECT;
	MAP_OBJECT	m_GameObjects;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);

};

END
#endif // Layer_h__
