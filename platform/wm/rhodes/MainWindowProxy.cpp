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
    if (qtMainWindow) delete (QtMainWindow*)qtMainWindow;
    if (qtApplication) delete (QApplication*)qtApplication;
}

void CMainWindowProxy::navigate(const wchar_t* url)
{
	((QtMainWindow*)qtMainWindow)->navigate(QUrl(QString::fromWCharArray(url)));
}

void CMainWindowProxy::setCallback(IMainWindowCallback* callback)
{
	((QtMainWindow*)qtMainWindow)->setCallback(callback);
}

void* CMainWindowProxy::init(IMainWindowCallback* callback, const wchar_t* title)
{
    int argc = 0;
    qtApplication = (void*)new QApplication(argc, 0);
	qtMainWindow = (void*)new QtMainWindow();
    ((QtMainWindow*)qtMainWindow)->setWindowTitle(QString::fromWCharArray(title));
	((QtMainWindow*)qtMainWindow)->setCallback(callback);
    ((QtMainWindow*)qtMainWindow)->show();

	return (void*)((QtMainWindow*)qtMainWindow)->winId();
}

void CMainWindowProxy::messageLoop(void)
{
	qApp->exec();
}
