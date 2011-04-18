#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QWebInspector>
#include <QAction>
#include "MainWindowCallback.h"
#include "common/IRhoThreadImpl.h"

namespace Ui {
    class QtMainWindow;
}

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);
	void setCallback(IMainWindowCallback* callback);
	// webview
    void navigate(QUrl url);
	void GoBack(void);
	void GoForward(void);
	void Refresh(void);
    // toolbar
	void toolbarRemoveAllButtons(void);
    void toolbarShow(void);
    void toolbarHide(void);
	int toolbarGetHeight(void);
    void toolbarAddAction(const QString & text);
    void toolbarAddAction(const QIcon & icon, const QString & text, const char* action);
	void toolbarAddSeparator(void);
private:
    Ui::QtMainWindow *ui;
    QWebInspector *wi;
    IMainWindowCallback* cb;

private slots:
    void on_webView_urlChanged(QUrl );
    void on_webView_loadFinished(bool);
    void on_webView_loadStarted();
    void on_actionExit_triggered();
	void on_toolbarAction_triggered(bool);
protected:
	void resizeEvent(QResizeEvent *);
};

#endif // QTMAINWINDOW_H
