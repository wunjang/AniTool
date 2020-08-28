#include "stdafx.h"
#include "Function.h"

void CFunction::FindFileByExtention(const CString & strExt, const CString & strRoot, map<CString, CString> * filePathMap)
{
	CFileFind find;
	BOOL bContinue = FALSE;
	
	bContinue = find.FindFile(strRoot + L"\\*.*");
	while (bContinue)
	{
		bContinue = find.FindNextFile();
		if (find.IsDots() || find.IsSystem())
		{
			continue;
		}
		if (find.IsDirectory())
		{
			FindFileByExtention(strExt, find.GetFilePath(), filePathMap);
		}
		else if (find.GetFileName().Find(strExt) > 0)
		{
			filePathMap->emplace(find.GetFileTitle(), find.GetFilePath());
		}
	}
}
