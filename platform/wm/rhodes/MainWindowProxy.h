#pragma once

#include <string>
#include "MainWindowCallback.h"

class CMainWindowProxy
{
public:
	CMainWindowProxy(void);
	~CMainWindowProxy(void);
	void* init(IMainWindowCallback* callback, const wchar_t* title);
	void setCallback(IMainWindowCallback* callback);
	void messageLoop(void);
	void navigate(const wchar_t* url);
private:
	void* qtMainWindow;
	void* qtApplication;
};
