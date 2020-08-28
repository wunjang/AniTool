#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER	Get_AniCtrl(void) { return m_pAniCtrl; }
	_bool						Is_AnimationSetFinish(double dbAdjTime = 0.25);
	_bool						Is_AnimationSetFinishFromRatio(double dbAdjTime = 0.9);

public:
	HRESULT			Ready_AniCtrl(void);

	void			Set_AnimationSet(const _uint& iIndex);
	void			Set_CoreciveAniSet(const _uint& iIndex);

	void			Play_AnimationSet(const _float& fTimeDelta);

	_double			Get_AniPeriod()					{ return m_dbPeriod; }
	_uint			Get_CurrentAnimationSet()		{ return m_iOldAniIdx; }

	void			Set_Weight(_float fWeight)		{ m_fCurWeight = fWeight; m_fNextWeight = 1.f - fWeight; }
	void			Set_CurWeight(_float fWeight)	{ m_fCurWeight = fWeight;}
	void			Set_NextWeight(_float fWeight)	{ m_fNextWeight = fWeight; }
	void SetTrackPosition(_double dbPosition);
private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;
	_uint							m_iCurrentTrack;
	_uint							m_iNewTrack;
	_float							m_fAccTime;
	_uint							m_iOldAniIdx;

	_double							m_dbPeriod;

	_float							m_fCurWeight = 0.1f;
	_float							m_fNextWeight = 0.9f;
public:
	static CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*		Create(const CAniCtrl& rhs);
	virtual void			Free(void);
};

END
#endif // AniCtrl_h__
