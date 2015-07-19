#include "mainwindow.h"
#include "ui_mainwindow.h"



#include <QtGui>

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>

//! [0]
void MainWindow::Window()
{
    iconList<<QIcon(":/images/bad.png")<<QIcon(":/images/heart.png")<<QIcon(":/images/trash.png");

    createActions();
    createTrayIcon();


    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));


    setIcon(1);
    trayIcon->show();


}
//! [0]



void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}



//! [1]
void MainWindow::setVisible(bool visible)
{

    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}
//! [1]

//! [2]
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}
//! [2]

//! [3]
void MainWindow::setIcon(int index)
{
//    QIcon icon = iconComboBox->itemIcon(index);
    currentIconIndex = index;
    QIcon icon = iconList[index];
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip(tr("iofjoijoeijsofej"));
}
//! [3]

//! [4]
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        currentIconIndex = (currentIconIndex+1) % iconList.count();
        setIcon(currentIconIndex);
        showMessage();

        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}
//! [4]

//! [5]
void MainWindow::showMessage()
{
    trayIcon->showMessage(tr("mmmmm1"), tr("mmmmm2222"),
                          QSystemTrayIcon::Warning, 5 * 1000);
}
//! [5]

//! [6]
void MainWindow::messageClicked()
{
    QMessageBox::information(0, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}
//! [6]




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    Window();

    // transparent background
//    setAttribute(Qt::WA_TranslucentBackground);
//    setStyleSheet("background:transparent;");

    // no window decorations
//    setWindowFlags(Qt::FramelessWindowHint);

    ui->setupUi(this);

    basewindow = new QWidget;
    basewindow->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setCentralWidget(basewindow);

    view = new QWebEngineView(this);
    view->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    setCentralWidget(view);
    view->resize(this->size());
    view->load(QUrl("http://1111hui.com/pdf/web/viewer.html"));
    view->show();

    QPushButton * btn = new QPushButton(this);
    btn->setText("OISJDOFJ");
    btn->show();
//    connect(btn, SIGNAL( clicked() ), this, SLOT( showMessageBox() ) );

    QWebView * view2 = new QWebView(parent);
    view2->resize(this->size());
    view2->load(QUrl("http://1111hui.com/pdf/web/viewer.html"));
    //view2->show();


}


void MainWindow::showMessageBox(){
    QString str="Some Text", helper="";
    QString factor = QString::number( view->page()->zoomFactor() );
    QMessageBox msgBox;
    if(helper!="") msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setTextFormat(Qt::RichText); // this does the magic trick and allows you to click the link
    msgBox.setText(str+factor); // "Text<br /><a href=\"http://www.such-and-such.com\">http://www.such-and-such.com</a>"
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
