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

	//���� ���
	void		Input_Fog(CFog* pFog);
	//���� ��ϵ� ���� �ʱ�ȭ
	void		Reset_Fog();
	//�������� ���� �� �Ŵ����� �ʱ�ȭ
	void		Reset_Manager();
protected:
	//Depth Fog�� �Ŵ������� ��������
	CFog*				m_pDepthFog;
	CFog*				m_pHeightFog;

	list<CFog*>			m_listFog;
private:
	virtual void Free(void);
};

END