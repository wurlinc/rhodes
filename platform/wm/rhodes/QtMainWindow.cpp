#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"

QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    wi(new QWebInspector())
{
	ui->setupUi(this);
	this->ui->webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	this->move(0,0);

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

void QtMainWindow::closeEvent(QCloseEvent *ce)
{
    wi->close();
    QMainWindow::closeEvent(ce);
}

void QtMainWindow::on_actionExit_triggered()
{
    close();
}

void QtMainWindow::on_webView_loadStarted()
{
    ui->statusBar->showMessage("Loading...");
}

void QtMainWindow::on_webView_loadFinished(bool ok)
{
    ui->statusBar->showMessage((ok?"Loaded ":"Failed ")+ui->webView->url().toString());
}

void QtMainWindow::on_webView_urlChanged(QUrl url)
{
    // url.toString()
}

void QtMainWindow::navigate(QUrl url)
{
	ui->webView->setUrl(url);
}
