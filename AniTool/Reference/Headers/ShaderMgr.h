#pragma once
#include "Engine_Defines.h"
#include "Base.h"
BEGIN(ENGINE)
class CShader;
class ENGINE_DLL CShaderMgr : public CBase
{
	DECLARE_SINGLETON(CShaderMgr)
public:
	enum ShaderType { ShaderType_Rect,ShaderType_Mesh, ShaderType_Shade, ShaderType_Particle,ShaderType_Blend, ShaderType_PostProcess, ShaderType_End};

private:
	explicit CShaderMgr();
	virtual ~CShaderMgr();

public:
	HRESULT InitManager(LPDIRECT3DDEVICE9 & pGraphicDev);

	CShader*	Get_Shader(ShaderType eType);
	void		Reset_Shader(ShaderType eType);
protected:

protected:
	map<ShaderType, CShader*> m_mapShader;


protected:
	// CBase을(를) 통해 상속됨
	virtual void Free(void) override;
};

END