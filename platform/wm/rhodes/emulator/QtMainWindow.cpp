#pragma warning(disable:4996)
#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"
#include "ExternalWebView.h"
#include <QResizeEvent>
#include <QWebFrame>
#include <QWebSettings>
#include <QWebSecurityOrigin>
#include "ext/rho/rhoruby.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "rubyext/WebView.h"
#undef null

extern "C" {
	extern VALUE rb_thread_main(void);
	extern VALUE rb_thread_wakeup(VALUE thread);
}

QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    wi(new QWebInspector()),
    cb(NULL)
{
    ui->setupUi(this);

    QWebSettings* qs = QWebSettings::globalSettings(); //this->ui->webView->settings();
    qs->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    qs->setAttribute(QWebSettings::LocalStorageEnabled, true);
    qs->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    qs->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    qs->setOfflineStorageDefaultQuota(1024*1024*1024);

    rho::String rs_dir = RHODESAPP().getRhoRootPath()+RHO_EMULATOR_DIR;
    qs->setOfflineWebApplicationCachePath(rs_dir.c_str());
    qs->setLocalStoragePath(rs_dir.c_str());
    qs->setOfflineStoragePath(rs_dir.c_str());

    this->ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    this->ui->webView->page()->mainFrame()->securityOrigin().setDatabaseQuota(1024*1024*1024);

    this->move(0,0);
    this->ui->toolBar->hide();
    this->ui->toolBarRight->hide();

    // connecting WebInspector
    wi->setWindowTitle("Web Inspector");
    wi->setPage(ui->webView->page());
    wi->move(416, this->geometry().y());
    wi->resize(850, 600);
    wi->show();
}

QtMainWindow::~QtMainWindow()
{
    delete wi;
    delete ui;
}

void QtMainWindow::setCallback(IMainWindowCallback* callback)
{
    cb = callback;
}

void QtMainWindow::hideEvent(QHideEvent *)
{
    if (cb) cb->onActivate(0);
}

void QtMainWindow::showEvent(QShowEvent *)
{
    if (cb) cb->onActivate(1);
}

void QtMainWindow::closeEvent(QCloseEvent *ce)
{
    rb_thread_wakeup(rb_thread_main());
    if (cb) cb->onWindowClose();
	wi->close();
    QMainWindow::closeEvent(ce);
}

void QtMainWindow::resizeEvent(QResizeEvent *event)
{
    if (cb)
        cb->updateSizeProperties(event->size().width(), event->size().height()); // ui->webView
}

void QtMainWindow::on_actionBack_triggered()
{
    ui->webView->back();
}

void QtMainWindow::on_webView_linkClicked(const QUrl& url)
{
    QString sUrl = url.toString();
    if (sUrl.contains("rho_open_target=_blank")) {
        if (cb) cb->logEvent("WebView: open external browser");
        ExternalWebView* externalWebView = new ExternalWebView();
        externalWebView->navigate(QUrl(sUrl.remove("rho_open_target=_blank")));
        externalWebView->show();
        externalWebView->activateWindow();
    } else {
        sUrl.remove(QRegExp("#+$"));
        if (sUrl.compare(ui->webView->url().toString())!=0)
            ui->webView->load(QUrl(sUrl));
    }
}

void QtMainWindow::on_webView_loadStarted()
{
    if (cb) cb->logEvent("WebView: loading...");
}

void QtMainWindow::on_webView_loadFinished(bool ok)
{
    if (cb) cb->logEvent((ok?"WebView: loaded ":"WebView: failed ")); // +ui->webView->url().toString()
}

void QtMainWindow::on_webView_urlChanged(QUrl url)
{
    if (cb) cb->logEvent("WebView: URL changed"); // url.toString()
}

void QtMainWindow::on_menuMain_aboutToShow()
{
    if (cb) cb->createCustomMenu();
}

void QtMainWindow::navigate(QString url)
{
    if (url.startsWith("javascript:", Qt::CaseInsensitive)) {
        url.remove(0,11);
        ui->webView->stop();
        ui->webView->page()->mainFrame()->evaluateJavaScript(url);
    } else
        ui->webView->load(QUrl(url));
}

void QtMainWindow::GoBack(void)
{
    ui->webView->back();
}

void QtMainWindow::GoForward(void)
{
    ui->webView->forward();
}

void QtMainWindow::Refresh(void)
{
    ui->webView->reload();
}

void QtMainWindow::toolbarRemoveAllButtons()
{
    ui->toolBar->clear();
    ui->toolBarRight->clear();
}

void QtMainWindow::toolbarShow()
{
    ui->toolBar->show();
    ui->toolBarRight->show();
}

void QtMainWindow::toolbarHide()
{
    ui->toolBar->hide();
    ui->toolBarRight->hide();
}

int QtMainWindow::toolbarGetHeight()
{
    return ui->toolBar->height();
}

void QtMainWindow::toolbarAddAction(const QString & text)
{
    ui->toolBar->addAction(text);
}

void QtMainWindow::on_toolbarAction_triggered(bool checked)
{
    QObject* sender = QObject::sender();
    QAction* action;
    if (sender && (action = dynamic_cast<QAction*>(sender))) {
		rho::String* strAction = new rho::String(action->data().toString().toStdString());
        if ( strcasecmp(strAction->c_str(), "forward") == 0 )
            rho_webview_navigate_forward();
        else
            RHODESAPP().loadUrl(*strAction);
    }
}

void QtMainWindow::toolbarAddAction(const QIcon & icon, const QString & text, const char* action, bool rightAlign)
{
    QAction* qAction = new QAction(icon, text, ui->toolBar);
    qAction->setData(QVariant(action));
    QObject::connect(qAction, SIGNAL(triggered(bool)), this, SLOT(on_toolbarAction_triggered(bool)) );
    if (rightAlign)
        ui->toolBarRight->insertAction( (ui->toolBarRight->actions().size() > 0 ? ui->toolBarRight->actions().last() : 0), qAction);
    else
        ui->toolBar->addAction(qAction);
}

void QtMainWindow::toolbarAddSeparator()
{
    ui->toolBar->addSeparator();
}

void QtMainWindow::setToolbarStyle(bool border, QString background)
{
	QString style = "";
	if (!border) style += "border:0px";
	if (background.length()>0) {
        if (style.length()>0) style += ";";
        style += "background:"+background;
    }
	if (style.length()>0) {
		style = "QToolBar{"+style+"}";
	    ui->toolBar->setStyleSheet(style);
        ui->toolBarRight->setStyleSheet(style);
	}
}

void QtMainWindow::menuAddAction(const QString & text, int item)
{
    QAction* qAction = new QAction(text, ui->toolBar);
    qAction->setData(QVariant(item));
    QObject::connect(qAction, SIGNAL(triggered(bool)), this, SLOT(on_menuAction_triggered(bool)) );
    ui->menuMain->addAction(qAction);
}

void QtMainWindow::menuClear(void)
{
    ui->menuMain->clear();
}

void QtMainWindow::menuAddSeparator()
{
    ui->menuMain->addSeparator();
}

void QtMainWindow::on_menuAction_triggered(bool checked)
{
    QObject* sender = QObject::sender();
    QAction* action;
    if (sender && (action = dynamic_cast<QAction*>(sender)) && cb)
        cb->onCustomMenuItemCommand(action->data().toInt());
}
