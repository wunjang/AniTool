#pragma once
#include "Shader.h"
BEGIN(ENGINE)
class ENGINE_DLL CLuminance :
	public CShader
{
private:
	explicit CLuminance(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLuminance(const CLuminance& rhs);
	virtual ~CLuminance(void);

public:
	virtual HRESULT				Ready_Shader(wstring wstrFilePath);

protected:
	vector<LPDIRECT3DPIXELSHADER9> m_vecPS;          
	vector<LPD3DXCONSTANTTABLE> m_vecConstantTable;
public:
	static CShader*			Create(LPDIRECT3DDEVICE9	pGraphicDev, wstring wstrFilePath);
	virtual CComponent*		Clone(void* pArg)override;
private:
	virtual void			Free(void) override;
};
END