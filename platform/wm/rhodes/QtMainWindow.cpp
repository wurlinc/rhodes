#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"
#include <QResizeEvent>

QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    wi(new QWebInspector()),
	cb(NULL)
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

void QtMainWindow::setCallback(IMainWindowCallback* callback)
{
	cb = callback;
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
