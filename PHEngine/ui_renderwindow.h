/********************************************************************************
** Form generated from reading UI file 'renderwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERWINDOW_H
#define UI_RENDERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "renderwidget.h"

QT_BEGIN_NAMESPACE

class Ui_RenderWindow
{
public:
    QAction *actioncreatProject;
    QAction *actionopenProject;
    QAction *Render_action;
    QAction *Clear_action;
    QAction *actionWirefame;
    QWidget *centralwidget;
    RenderWidget *openGLWidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menuLUA;
    QStatusBar *statusbar;
    QToolBar *toolBarL;
    QToolBar *toolBar_2;
    QToolBar *toolBar;

    void setupUi(QMainWindow *RenderWindow)
    {
        if (RenderWindow->objectName().isEmpty())
            RenderWindow->setObjectName(QString::fromUtf8("RenderWindow"));
        RenderWindow->resize(1417, 732);
        actioncreatProject = new QAction(RenderWindow);
        actioncreatProject->setObjectName(QString::fromUtf8("actioncreatProject"));
        actionopenProject = new QAction(RenderWindow);
        actionopenProject->setObjectName(QString::fromUtf8("actionopenProject"));
        Render_action = new QAction(RenderWindow);
        Render_action->setObjectName(QString::fromUtf8("Render_action"));
        Clear_action = new QAction(RenderWindow);
        Clear_action->setObjectName(QString::fromUtf8("Clear_action"));
        actionWirefame = new QAction(RenderWindow);
        actionWirefame->setObjectName(QString::fromUtf8("actionWirefame"));
        actionWirefame->setCheckable(true);
        centralwidget = new QWidget(RenderWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        openGLWidget = new RenderWidget(centralwidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(-10, 0, 1271, 671));
        RenderWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RenderWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1417, 31));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menuLUA = new QMenu(menubar);
        menuLUA->setObjectName(QString::fromUtf8("menuLUA"));
        RenderWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RenderWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RenderWindow->setStatusBar(statusbar);
        toolBarL = new QToolBar(RenderWindow);
        toolBarL->setObjectName(QString::fromUtf8("toolBarL"));
        RenderWindow->addToolBar(Qt::LeftToolBarArea, toolBarL);
        toolBar_2 = new QToolBar(RenderWindow);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        RenderWindow->addToolBar(Qt::RightToolBarArea, toolBar_2);
        toolBar = new QToolBar(RenderWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        RenderWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menuLUA->menuAction());
        menu->addAction(actioncreatProject);
        menu->addAction(actionopenProject);
        toolBarL->addAction(actioncreatProject);
        toolBar_2->addAction(actionopenProject);
        toolBar->addAction(Render_action);
        toolBar->addSeparator();
        toolBar->addAction(Clear_action);
        toolBar->addAction(actionWirefame);

        retranslateUi(RenderWindow);

        QMetaObject::connectSlotsByName(RenderWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RenderWindow)
    {
        RenderWindow->setWindowTitle(QCoreApplication::translate("RenderWindow", "RenderWindow", nullptr));
        actioncreatProject->setText(QCoreApplication::translate("RenderWindow", "\345\210\233\345\273\272\351\241\271\347\233\256", nullptr));
        actionopenProject->setText(QCoreApplication::translate("RenderWindow", "\346\211\223\345\274\200\351\241\271\347\233\256", nullptr));
        Render_action->setText(QCoreApplication::translate("RenderWindow", "\346\270\262\346\237\223", nullptr));
        Clear_action->setText(QCoreApplication::translate("RenderWindow", "\346\270\205\347\251\272", nullptr));
        actionWirefame->setText(QCoreApplication::translate("RenderWindow", "\347\272\277\346\241\206", nullptr));
        menu->setTitle(QCoreApplication::translate("RenderWindow", "\346\226\207\344\273\266", nullptr));
        menu_2->setTitle(QCoreApplication::translate("RenderWindow", "\347\211\251\344\275\223", nullptr));
        menuLUA->setTitle(QCoreApplication::translate("RenderWindow", "LUA", nullptr));
        toolBarL->setWindowTitle(QCoreApplication::translate("RenderWindow", "toolBar", nullptr));
        toolBar_2->setWindowTitle(QCoreApplication::translate("RenderWindow", "toolBar_2", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("RenderWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenderWindow: public Ui_RenderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERWINDOW_H
