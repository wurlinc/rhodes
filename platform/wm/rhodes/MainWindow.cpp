// MainWindow.cpp: Defines main window for this application.

#include "stdafx.h"
#include "MainWindow.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");

using namespace rho;
using namespace rho::common;

extern "C" void rho_geoimpl_turngpsoff();

int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;

CMainWindow::CMainWindow()
{
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::Navigate2(BSTR URL) {
    String cleared_url = convertToStringA(OLE2CT(URL));
    if (!cleared_url.empty()) {
        StringW cw = convertToStringW(cleared_url);
        //cw.c_str();
    }
}

// **************************************************************************
//
// WM_xxx handlers
//
// **************************************************************************

LRESULT CMainWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = S_OK;

    int xScreenSize = GetSystemMetrics(SM_CXSCREEN);
    int yScreenSize = GetSystemMetrics(SM_CYSCREEN);

    LOG(INFO)  + "Screen size: x=" + xScreenSize + ";y=" + yScreenSize;

    RECT rcMainWindow = { 0,0,320,470 };

    LOGCONF().setLogView(&m_logView);

    rcMainWindow.left = getIniInt(_T("main_view_left"),0);
    rcMainWindow.top = getIniInt(_T("main_view_top"),0);
    if ( rcMainWindow.left < 0 || rcMainWindow.left > xScreenSize )
        rcMainWindow.left = 0;
    if ( rcMainWindow.top < 0 || rcMainWindow.top > yScreenSize )
        rcMainWindow.top = 0;

    int width = RHOCONF().getInt("client_area_width");
    if (width <= 0) 
        width = rcMainWindow.right;
    rcMainWindow.right = rcMainWindow.left+width;
    int height = RHOCONF().getInt("client_area_height");
    if (height <= 0) 
        height = rcMainWindow.bottom;
    rcMainWindow.bottom = rcMainWindow.top+height;

    // TODO: create window & menubar & webview

    if ( !RHOCONF().getBool("wm_show_statusbar") )
        {} // TODO: show/hide statusbar

    m_screenWidth = rcMainWindow.right - rcMainWindow.left;
    m_screenHeight = rcMainWindow.bottom - rcMainWindow.top;

    // TODO: MoveWindow(&rcMainWindow);


    RHO_ASSERT(SUCCEEDED(hr));

    rho_rhodesapp_callUiCreatedCallback();

    return SUCCEEDED(hr) ? 0 : -1;
}

//HWND CMainWindow::getWebViewHWND() {
//	return NULL;
//}

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

LRESULT CMainWindow::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    // USES_CONVERSION;
    // LOG(TRACE) + "Seting browser client area size to: " + (int)LOWORD(lParam) + " x " + (int)(HIWORD(lParam)-m_menuBarHeight-m_toolbar.getHeight());
    // m_browser.MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam)-m_menuBarHeight-m_toolbar.getHeight());
    // if (m_menuBar.m_hWnd) {
    //    m_menuBar.MoveWindow(0, HIWORD(lParam)-m_menuBarHeight, LOWORD(lParam), m_menuBarHeight);
    //}
    //if ( m_toolbar.m_hWnd )
    //    m_toolbar.MoveWindow(0, HIWORD(lParam)-m_menuBarHeight-m_toolbar.getHeight(), LOWORD(lParam), m_toolbar.getHeight());

    // TODO: put everything in place

    return 0;
}

LRESULT CMainWindow::OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    int fActive = LOWORD(wParam);
    rho_rhodesapp_callAppActiveCallback(fActive);
    if (!fActive)
        rho_geoimpl_turngpsoff();
    return 0;
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
    // TODO: m_spIWebBrowser2->GoBack();
    return 0;
}

LRESULT CMainWindow::OnNavigateForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: m_spIWebBrowser2->GoForward();
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
    // TODO: if (m_spIWebBrowser2) m_spIWebBrowser2->Refresh();
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
