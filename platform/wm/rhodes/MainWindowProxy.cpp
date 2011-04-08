#include "MainWindowProxy.h"
#include <QString>
#include <QApplication>
#include "QtMainWindow.h"

CMainWindowProxy::CMainWindowProxy(void):
    qtApplication(NULL),
    qtMainWindow(NULL)
{
}

CMainWindowProxy::~CMainWindowProxy(void)
{
    if (qtApplication) delete (QApplication*)qtApplication;
    if (qtMainWindow) delete (QtMainWindow*)qtMainWindow;
}

void CMainWindowProxy::navigate(const wchar_t* url)
{
	((QtMainWindow*)qtMainWindow)->navigate(QUrl(QString::fromWCharArray(url)));
}

void* CMainWindowProxy::init(const wchar_t* title)
{
    int argc = 0;
    qtApplication = (void*)new QApplication(argc, 0);
	qtMainWindow = (void*)new QtMainWindow();

    ((QtMainWindow*)qtMainWindow)->setWindowTitle(QString::fromWCharArray(title));
    ((QtMainWindow*)qtMainWindow)->show();

	return (void*)((QtMainWindow*)qtMainWindow)->winId();
}