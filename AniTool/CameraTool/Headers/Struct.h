#pragma once

struct CAMERAACTION
{
	CAMERAACTION(void)
		: strName(L"NewAction")
		, bIsFollow(false), eOBJ_ID(0), eObj_Idx(0)
		, vMoveTo({ 0.f, }), vRotateTo({ 0.f, }), fViewAngleTo(60.f)
		, fLength(1.f), fDistance(1.f)
	{
		
	}
	CString		strName;

	_bool		bIsFollow;
	_int		eOBJ_ID;
	_int		eObj_Idx;

	_vec3		vMoveTo;
	_vec3		vRotateTo;
	_float		fViewAngleTo;

	_float		fLength;
	_float		fDistance;
};