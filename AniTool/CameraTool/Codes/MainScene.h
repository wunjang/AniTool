#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"
#include "Define.h"



class CMainScene : public ENGINE::CScene
{
private:
	explicit CMainScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMainScene(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;
	
	static HRESULT		Load_MapData(LPDIRECT3DDEVICE9 pGraphicDev, const ENGINE::LAYER_TYPE eLayerType, wstring wstrFilePath);

private:
	HRESULT		Ready_Prototype(void);
	HRESULT		Ready_Environment_Layer(const ENGINE::LAYER_TYPE eLayerType);

	HRESULT		Ready_GameLogic_Layer(const ENGINE::LAYER_TYPE eLayerType);
	HRESULT		Ready_UI_Layer(const ENGINE::LAYER_TYPE eLayerType);
	HRESULT		Ready_LightInfo(void);






public:
	static CMainScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};


#endif // Stage_h__
