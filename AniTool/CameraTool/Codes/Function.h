#pragma once

#ifndef Function_h__
#define Function_h__

#include "Engine_Defines.h"
#include "Define.h"

class CFunction
{
public:
	static void FindFileByExtention(const CString& wstrExt, const CString& wstrRoot, map<CString, CString>* filePathMap);
};

#endif // !Function_h__
