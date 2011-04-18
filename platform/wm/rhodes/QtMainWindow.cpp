#pragma warning(disable:4996)
#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"
#include <QResizeEvent>
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "rubyext/WebView.h"
#undef null

QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    wi(new QWebInspector()),
	cb(NULL)
{
	ui->setupUi(this);
	this->ui->webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	this->move(0,0);
	this->ui->toolBar->hide();

#ifdef MAINWINDOW_SHOW_WEBINSPECTOR
	// connecting WebInspector
    wi->setWindowTitle("Web Inspector");
    wi->setPage(ui->webView->page());
    wi->move(416, this->geometry().y());
    wi->resize(850, 600);
    wi->show();
#endif
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
    wi->close();
    QMainWindow::closeEvent(ce);
}

void QtMainWindow::resizeEvent(QResizeEvent *event)
{
    if (cb)
        cb->updateSizeProperties(event->size().width(), event->size().height()); // ui->webView
}

void QtMainWindow::on_actionExit_triggered()
{
    close();
}

void QtMainWindow::on_webView_loadStarted()
{
    //ui->statusBar->showMessage("Loading...");
	if (cb) cb->logEvent("WebView: loading...");
}

void QtMainWindow::on_webView_loadFinished(bool ok)
{
    //ui->statusBar->showMessage((ok?"Loaded ":"Failed ")+ui->webView->url().toString());
	if (cb) cb->logEvent((ok?"WebView: loaded ":"WebView: failed "));
}

void QtMainWindow::on_webView_urlChanged(QUrl url)
{
    // url.toString()
    if (cb) cb->logEvent("WebView: URL changed");
}

void QtMainWindow::navigate(QUrl url)
{
	ui->webView->setUrl(url);
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
}

void QtMainWindow::toolbarShow()
{
	ui->toolBar->show();
}

void QtMainWindow::toolbarHide()
{
	ui->toolBar->hide();
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
		rho::String strAction = action->data().toString().toStdString();
		if ( strcasecmp(strAction.c_str(), "forward") == 0 )
			rho_webview_navigate_forward();
		else
		    RHODESAPP().loadUrl(strAction);
	}
}

void QtMainWindow::toolbarAddAction(const QIcon & icon, const QString & text, const char* action)
{
	QAction* qAction = new QAction(icon, text, ui->toolBar);
	qAction->setData(QVariant(action));
	QObject::connect(qAction, SIGNAL(triggered(bool)), this, SLOT(on_toolbarAction_triggered(bool)) );
	ui->toolBar->addAction(qAction);
}

void QtMainWindow::toolbarAddSeparator()
{
	ui->toolBar->addSeparator();
}
