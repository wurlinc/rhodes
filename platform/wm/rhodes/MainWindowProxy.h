#pragma once

#include <string>

class CMainWindowProxy
{
public:
	CMainWindowProxy(void);
	~CMainWindowProxy(void);
	void* init(const wchar_t* title);
	void messageLoop(void);
	void navigate(const wchar_t* url);
private:
	void* qtMainWindow;
	void* qtApplication;
};
