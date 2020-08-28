#pragma once

#include "Component.h"

BEGIN(ENGINE)

class CShader;

class ENGINE_DLL CTexture final : public CComponent
{

private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	HRESULT SetUp_OnGraphicDev(_uint iStage, _uint iIndex = 0);
	HRESULT SetUp_OnShader(D3DXHANDLE hHandle, CShader* pShader, _uint iIndex = 0);
	HRESULT SetUp_OnShader(D3DXHANDLE hHandle, LPD3DXEFFECT & pEffect, _uint iIndex = 0);

public:
	virtual HRESULT Ready_Component_Prototype(TEXTURE::TYPE eType, const wstring pFileName, const _uint& iCnt);
	virtual HRESULT Ready_Component(void* pArg);		

	LPDIRECT3DTEXTURE9 Get_Texture(_int iIndex = 0);
	wstring			Get_FileName() { return m_wstrFileName; }
	_bool			Get_IsNullTex() { return m_bIsNullTex; }
	vector<IDirect3DBaseTexture9*>*		Get_TextureToTal() { return &m_vecTexture; }

private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;
private:
	wstring			m_wstrFileName;
	_bool			m_bIsNullTex;
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE::TYPE eType, const _tchar* pFileName, const _uint& iCnt = 1);
	virtual CComponent* Clone(void* pArg = nullptr);
protected:
	virtual void Free();
};

END