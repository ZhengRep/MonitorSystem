#pragma once

#include "winhdr.h"
#include "Singleton.h"

/*
* Loader of resources from resource files linked with application.
* Singleton without support of "lazy" initialization
*/

class ResourceLoader: public Singleton<ResourceLoader>
{
public:
	ResourceLoader(HINSTANCE appInst);
	virtual ~ResourceLoader();
	HICON loadStandartIcon(const TCHAR* iconName);
	HICON loadIcon(const TCHAR* iconName);
	//return ture if string is loaded, false is otherwise
	bool loadString(UINT id, StringStorage* string);
	HACCEL loadAccelerator(UINT id);
	HCURSOR loadStandardCursor(const TCHAR* id);
	HCURSOR loadCursor(UINT id);

protected:
	//Application instance
	HINSTANCE m_appInstance;

};

