#pragma once
#include "Component.h"
#include "Engine_Defines.h"

BEGIN(ENGINE)
class ENGINE_DLL CShader :
	public CComponent
{
protected:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader(void);


public:
	LPD3DXEFFECT		Get_EffectHandle(void) { return m_pEffect; }

public:
	virtual HRESULT				Ready_Shader(wstring wstrFilePath);

protected:

	LPD3DXEFFECT				m_pEffect;
	LPD3DXBUFFER				m_pErrMsg;

public:
	static  CShader*		Create(LPDIRECT3DDEVICE9	pGraphicDev, wstring wstrFilePath);
	virtual CComponent*		Clone(void* pArg);
	virtual void			Free(void);
};

END