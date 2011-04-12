#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QWebInspector>
#include "MainWindowCallback.h"

namespace Ui {
    class QtMainWindow;
}

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();
    void navigate(QUrl url);
    virtual void closeEvent(QCloseEvent *);
	void setCallback(IMainWindowCallback* callback);

private:
    Ui::QtMainWindow *ui;
    QWebInspector *wi;
    IMainWindowCallback* cb;

private slots:
    void on_webView_urlChanged(QUrl );
    void on_webView_loadFinished(bool );
    void on_webView_loadStarted();
    void on_actionExit_triggered();
protected:
	void resizeEvent(QResizeEvent *);
};

#endif // QTMAINWINDOW_H
