#pragma once
#include "Engine_Defines.h"
#include "Base.h"
#include "Fog.h"
BEGIN(ENGINE)
class ENGINE_DLL CFogMgr :
	public CBase
{
	DECLARE_SINGLETON(CFogMgr)

protected:
	explicit CFogMgr();
	virtual ~CFogMgr();

public:
	HRESULT		Ready_DepthFog(LPDIRECT3DDEVICE9 pGraphicDev,ENGINE::FOG::FOG_INFO tInfo);
	
	HRESULT		Ready_Fog(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::FOG::FOG_INFO tInfo,_vec3 vPos, _matrix* pParent = nullptr);
public:
	void		Update_Fog(const _float& fTimeDelta);
	void		Render_Fog(LPD3DXEFFECT& pEffect);

	//포그 등록
	void		Input_Fog(CFog* pFog);
	//현재 등록된 포그 초기화
	void		Reset_Fog();
	//스테이지 시작 시 매니저를 초기화
	void		Reset_Manager();
protected:
	//Depth Fog는 매니저에서 관리하자
	CFog*				m_pDepthFog;
	CFog*				m_pHeightFog;

	list<CFog*>			m_listFog;
private:
	virtual void Free(void);
};

END