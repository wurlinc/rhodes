/********************************************************************************
** Form generated from reading UI file 'QtMainWindow.ui'
**
** Created: Thu 14. Apr 17:52:49 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMAINWINDOW_H
#define UI_QTMAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_QtMainWindow
{
public:
    QAction *actionExit;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWebView *webView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QtMainWindow)
    {
        if (QtMainWindow->objectName().isEmpty())
            QtMainWindow->setObjectName(QString::fromUtf8("QtMainWindow"));
        QtMainWindow->resize(400, 600);
        QtMainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionExit = new QAction(QtMainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(QtMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        webView = new QWebView(centralwidget);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl("about:blank"));

        verticalLayout->addWidget(webView);

        QtMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(QtMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 400, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        QtMainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(QtMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtMainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(QtMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolBar->sizePolicy().hasHeightForWidth());
        toolBar->setSizePolicy(sizePolicy);
        toolBar->setMinimumSize(QSize(0, 0));
        toolBar->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::BottomToolBarArea);
        toolBar->setIconSize(QSize(48, 48));
        toolBar->setFloatable(false);
        QtMainWindow->addToolBar(Qt::BottomToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(QtMainWindow);

        QMetaObject::connectSlotsByName(QtMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QtMainWindow)
    {
        QtMainWindow->setWindowTitle(QApplication::translate("QtMainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("QtMainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("QtMainWindow", "File", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("QtMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtMainWindow: public Ui_QtMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMAINWINDOW_H
