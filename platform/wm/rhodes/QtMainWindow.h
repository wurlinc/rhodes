#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QWebInspector>

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

private:
    Ui::QtMainWindow *ui;
    QWebInspector *wi;

private slots:
    void on_webView_urlChanged(QUrl );
    void on_webView_loadFinished(bool );
    void on_webView_loadStarted();
    void on_actionExit_triggered();
};

#endif // QTMAINWINDOW_H
