#ifndef Line_h__
#define Line_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(ENGINE)

class ENGINE_DLL CLine : public CBase
{
public:
	enum POINT		{ POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE	{ COMPARE_LEFT, COMPARE_RIGHT };

private:
	explicit CLine(void);
	virtual ~CLine(void);

public:
	HRESULT			    Ready_Line(const _vec2* pPointA, const _vec2* pPointB);
	bool				Compare(const _vec2* pEndPos);

	_vec2				Get_Nomal() { return m_vNormal; }
private:
	_vec2				m_vPoint[POINT_END];
	_vec2				m_vDirection;
	_vec2				m_vNormal;

public:
	static CLine*		Create(const _vec2* pPointA, const _vec2* pPointB);
	virtual void		Free(void);

};

END
#endif // Line_h__
