#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Light.h"
#include "GodRay.h"
BEGIN(ENGINE)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr(void);
	virtual ~CLightMgr(void);

public:
	const D3DLIGHT9*		Get_Light(const _uint& iIndex);

	HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex, _matrix* pParent = nullptr);
	HRESULT		Ready_PointLight(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, _matrix* pParent = nullptr);
	void		Update_Light(const _float& fTimeDelta);
	void		Render_Light(LPD3DXEFFECT& pEffect);

	_uint		GetNextIndex();
	void		ResetLight();

	void		SetLightCam(LPD3DXEFFECT& pEffect, _int iLightNum, _vec3 vCamPos);
	void		SetObjectLightCam(LPD3DXEFFECT& pEffect, _int iLightNum, _vec3 vCamPos);

	//�������� ������ �����ش�
	//�ִ� ������ �������ְ� �Ѿ�� ���, �� ������ �ʱ�ȭ ������
	void		Set_GodRay_Focus(_vec3 vPos);

	void		Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev);

	void		Set_GodRay_RenderInfo(LPD3DXEFFECT& pEffect);

	void		InputLight(CLight* pLight);
private:
	list<CLight*>			m_LightList;

	list<CLight*>			m_ListDynamicPointLight;

	CGodRay*				m_pGodRay;

	_vec3					m_vLightDir;
	_vec3					m_vLightPos;
	_vec3					m_vLightAt;
	_matrix					m_matLightView;
	_matrix					m_matLightProj;

private:
	virtual void Free(void);

};

END
#endif // LightMgr_h__
