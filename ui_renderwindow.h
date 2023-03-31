/********************************************************************************
** Form generated from reading UI file 'renderwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERWINDOW_H
#define UI_RENDERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "Core/UI/listbox.h"
#include "Core/Render/renderwidget.h"

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
    ListBox *listBox;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menuLUA;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QListWidget *listWidget;

    void setupUi(QMainWindow *RenderWindow)
    {
        if (RenderWindow->objectName().isEmpty())
            RenderWindow->setObjectName("RenderWindow");
        RenderWindow->resize(1660, 1080);
        actioncreatProject = new QAction(RenderWindow);
        actioncreatProject->setObjectName("actioncreatProject");
        actionopenProject = new QAction(RenderWindow);
        actionopenProject->setObjectName("actionopenProject");
        Render_action = new QAction(RenderWindow);
        Render_action->setObjectName("Render_action");
        Clear_action = new QAction(RenderWindow);
        Clear_action->setObjectName("Clear_action");
        actionWirefame = new QAction(RenderWindow);
        actionWirefame->setObjectName("actionWirefame");
        actionWirefame->setCheckable(true);
        centralwidget = new QWidget(RenderWindow);
        centralwidget->setObjectName("centralwidget");
        openGLWidget = new RenderWidget(centralwidget);
        openGLWidget->setObjectName("openGLWidget");
        openGLWidget->setGeometry(QRect(211, 0, 1440, 810));
        listBox = new ListBox(centralwidget);
        listBox->setObjectName("listBox");
        listBox->setGeometry(QRect(0, 0, 211, 600));
        RenderWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RenderWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1660, 22));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menuLUA = new QMenu(menubar);
        menuLUA->setObjectName("menuLUA");
        RenderWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RenderWindow);
        statusbar->setObjectName("statusbar");
        RenderWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(RenderWindow);
        toolBar->setObjectName("toolBar");
        RenderWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidget_2 = new QDockWidget(RenderWindow);
        dockWidget_2->setObjectName("dockWidget_2");
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName("dockWidgetContents_2");
        listWidget = new QListWidget(dockWidgetContents_2);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(0, 0, 256, 192));
        dockWidget_2->setWidget(dockWidgetContents_2);
        RenderWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_2);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menuLUA->menuAction());
        menu->addAction(actioncreatProject);
        menu->addAction(actionopenProject);
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
        toolBar->setWindowTitle(QCoreApplication::translate("RenderWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenderWindow: public Ui_RenderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERWINDOW_H
