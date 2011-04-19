// MainWindow.cpp: Defines main window for this application.

#include "stdafx.h"
#include "MainWindowQt.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "common/RhoFilePath.h"
#include "AppManager.h"

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");

using namespace rho;
using namespace rho::common;

extern "C" void rho_geoimpl_turngpsoff();

int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;

CMainWindow::CMainWindow() { }

CMainWindow::~CMainWindow() { }

void CMainWindow::updateSizeProperties(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
}

void CMainWindow::logEvent(const ::std::string& message)
{
    LOG(INFO) + message;
}

void CMainWindow::Navigate2(BSTR URL) {
    String cleared_url = convertToStringA(OLE2CT(URL));
    if (!cleared_url.empty()) {
        StringW cw = convertToStringW(cleared_url);
		m_mainWindowProxy.navigate(cw.c_str());
    }
}

HWND CMainWindow::Initialize(const wchar_t* title)
{
    HWND hWnd = (HWND)m_mainWindowProxy.init(this, title);
	SubclassWindow(hWnd);
    //rho_rhodesapp_callAppActiveCallback(1);
    rho_rhodesapp_callUiCreatedCallback();
	return hWnd;
}

void CMainWindow::MessageLoop(void)
{
	m_mainWindowProxy.messageLoop();
}

void CMainWindow::createCustomMenu(void)
{
	RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);
	//m_mainWindowProxy.createCustomMenu();
#ifdef ENABLE_DYNAMIC_RHOBUNDLE
    String strIndexPage = CFilePath::join(RHODESAPP().getStartUrl(),"index_erb.iseq");
    if ( RHODESAPP().getCurrentUrl().compare(RHODESAPP().getStartUrl()) == 0 ||
         RHODESAPP().getCurrentUrl().compare(strIndexPage) == 0 )
        m_arAppMenuItems.addElement(CAppMenuItem("Reload RhoBundle","reload_rhobundle"));
#endif //ENABLE_DYNAMIC_RHOBUNDLE

	//update UI with custom menu items
	m_mainWindowProxy.menuClear();
    for ( unsigned int i = 0; i < m_arAppMenuItems.size(); i++)
    {
        CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
        if (oItem.m_eType == CAppMenuItem::emtSeparator) 
			m_mainWindowProxy.menuAddSeparator();
		else
        {
			m_mainWindowProxy.menuAddAction((oItem.m_eType == CAppMenuItem::emtClose ? "Exit" : oItem.m_strLabel.c_str()), i);
        }
    }
}

void CMainWindow::onCustomMenuItemCommand(int nItemPos)
{	
    if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return;

	CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
    if ( oMenuItem.m_eType == CAppMenuItem::emtUrl )
    {
        if ( oMenuItem.m_strLink == "reload_rhobundle" )
        {
        #ifdef ENABLE_DYNAMIC_RHOBUNDLE
	        if ( RHODESAPP().getRhobundleReloadUrl().length()>0 ) {
		        CAppManager::ReloadRhoBundle(m_hWnd,RHODESAPP().getRhobundleReloadUrl().c_str(), NULL);
	        } else {
		        MessageBox(_T("Path to the bundle is not defined."),_T("Information"), MB_OK | MB_ICONINFORMATION );
	        }
        #endif
            return;
        }
    }

    oMenuItem.processCommand();
}


// **************************************************************************
//
// WM_xxx handlers
//
// **************************************************************************

void CMainWindow::performOnUiThread(rho::common::IRhoRunnable* pTask)
{
	PostMessage(WM_EXECUTE_RUNNABLE, 0, (LPARAM)pTask);
}
LRESULT CMainWindow::OnExecuteRunnable(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) 
{
    rho::common::IRhoRunnable* pTask = (rho::common::IRhoRunnable*)lParam;
	if (pTask)
    {
		pTask->runObject();
        delete pTask;
    }
	return 0;
}

LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    rho_rhodesapp_callUiDestroyedCallback();

    if(m_logView.IsWindow()) {
        m_logView.DestroyWindow();
    }
    LOGCONF().setLogView(NULL);

    PostQuitMessage(0);

    bHandled = FALSE; // Allow ATL's default processing (e.g. NULLing m_hWnd)
    return 0;
}

void CMainWindow::onActivate(int active)
{
    rho_rhodesapp_callAppActiveCallback(active);
    if (!active)
        rho_geoimpl_turngpsoff();
}


// **************************************************************************
//
// WM_COMMAND handlers
//
// **************************************************************************

LRESULT CMainWindow::OnExitCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    SendMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainWindow::OnNavigateBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_mainWindowProxy.GoBack();
    return 0;
}

LRESULT CMainWindow::OnNavigateForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_mainWindowProxy.GoForward();
    return 0;
}

LRESULT CMainWindow::OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    RHODESAPP().navigateBack();
    return 0;
}

LRESULT CMainWindow::OnLogCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if ( !m_logView.IsWindow() ) {
        LoadLibrary(_T("riched20.dll"));
        m_logView.Create(NULL);
    }
    m_logView.ShowWindow(SW_SHOWNORMAL);
    return 0;
}

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_mainWindowProxy.Refresh();
    return 0;
}

LRESULT CMainWindow::OnNavigateCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    LPTSTR wcurl = (LPTSTR)hWndCtl;
    if (wcurl) {
        Navigate2(wcurl);
        free(wcurl);
    }
    return 0;
}

LRESULT CMainWindow::OnExecuteCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	RhoNativeViewRunnable* command = (RhoNativeViewRunnable*)wParam;
	if (command != NULL) {
		command->run();
	}
	return 0;
}	
