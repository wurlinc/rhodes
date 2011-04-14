#pragma once

#include <string>
#include "common/rhoparams.h"
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
	void GoBack(void);
	void GoForward(void);
	void Refresh(void);
	// toolbar proxy
	bool isStarted();
	int getHeight();
    void createToolbar(rho_param *p);
    void removeToolbar();
	void removeAllButtons();
private:
	void* qtMainWindow;
	void* qtApplication;
};
