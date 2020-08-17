#pragma once
struct CAMERAACTION
{
	CAMERAACTION(void)
		: strName(L"NewAction")
		, bIsFollow(false), eOBJ_ID(0), eObj_Idx(0)
		, vMoveTo({ 0.f, 0.f, 0.f }), vRotateTo({ 0.f, 0.f, 0.f }), fViewAngleTo(45.f)
		, fLength(1.f), fDistance(1.f)
		, EffectOption(0)
	{}

	CString		strName;

	_bool		bIsFollow;
	_int		eOBJ_ID;
	_int		eObj_Idx;

	_vec3		vMoveTo;
	_vec3		vRotateTo;
	_float		fViewAngleTo;

	_float		fLength;
	_float		fDistance;

	_int		EffectOption;
};

struct CAMERAACTION_SAVE
{
	CAMERAACTION_SAVE(void)
		: szName(L"")
		, bIsFollow(false), eOBJ_ID(0), eObj_Idx(0)
		, vMoveTo({ 0.f, 0.f, 0.f }), vRotateTo({ 0.f, 0.f, 0.f }), fViewAngleTo(45.f)
		, fLength(1.f), fDist(1.f)
		, EffectOption(0)
	{}
	CAMERAACTION_SAVE(const CAMERAACTION& rAction)
		: bIsFollow(rAction.bIsFollow), eOBJ_ID(rAction.eOBJ_ID), eObj_Idx(rAction.eObj_Idx)
		, vMoveTo(rAction.vMoveTo), vRotateTo(rAction.vRotateTo), fViewAngleTo(rAction.fViewAngleTo)
		, fLength(rAction.fLength), fDist(rAction.fDistance)
		, EffectOption(rAction.EffectOption)
	{
		lstrcpy(szName, rAction.strName.GetString());
	}
	_tchar		szName[64];

	_bool		bIsFollow;
	_int		eOBJ_ID;
	_int		eObj_Idx;

	_vec3		vMoveTo;
	_vec3		vRotateTo;
	_float		fViewAngleTo;

	_float		fLength;
	_float		fDist;

	_int		EffectOption;
};