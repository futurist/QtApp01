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

    connect(trayIconMenu, SIGNAL(aboutToShow()), this, SLOT( showIconMenu() ) );

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::showIconMenu(){

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
//        QMessageBox::information(this, tr("Systray"), tr("choose <b>Quit</b> in the context menu "));
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
    case QSystemTrayIcon::Context:
        trayIcon->setContextMenu( new QMenu(this) );
        QMessageBox::information(this, tr("Systray"), tr("choose <b>Quit</b> in the context menu "));

        break;
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormalIcon();
        trayIcon->setContextMenu(trayIconMenu);

//        currentIconIndex = (currentIconIndex+1) % iconList.count();
//        setIcon(currentIconIndex);
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
    QSystemTrayIcon::MessageIcon icon=QSystemTrayIcon::MessageIcon(1);  //设置图表是标准的系统托盘  信息
    trayIcon->showMessage(tr("mmmmm1"), tr("mmmmm2222"),
                          icon, 5 * 1000);
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


//恢复正常图标
void MainWindow::showNormalIcon()
{
    TimerCount=0;
    timer->stop();
    trayIcon->setIcon(iconList[currentIconIndex]);   //正常显示时候恢复原有图标，防止定时器在无显示图表时候被终止
    this->show();
}

//模拟QQ消息到来时候，托盘图表闪动
void MainWindow::showBlinkIcon()
{
    timer->start(500);  //每500ms都刷新一次
    timer->setSingleShot(false);  //如果为真，表示只重复一次,为假表示无限次循环
    connect(timer,SIGNAL(timeout()),this,SLOT(updateIcon()));
}
//刷新托盘图标
void MainWindow::updateIcon()
{
    TimerCount++;
    if(TimerCount%2)
    {
        trayIcon->setIcon(iconList[0]);   //实际上没有这个图标，然后会显示没有图表
    }
    else
    {
        trayIcon->setIcon(iconList[1]);
    }
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    timer=new QTimer(this);
    TimerCount=0;  //初始化为零
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
    connect(btn, SIGNAL( clicked() ), this, SLOT( showBlinkIcon() ) );

    QWebView * view2 = new QWebView(parent);
    view2->resize(this->size());
    view2->load(QUrl("http://1111hui.com/pdf/web/viewer.html"));
    //view2->show();

    resize(320, 640);

    screen = new QDesktopWidget();
    QRect rect = getWindowPositionAndSize();
    QRect screenRect = screen->availableGeometry();
    rect.moveTopRight( screenRect.topRight() );
    setWindowPositionAndSize(rect);

    this->installEventFilter(this);

}


bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
        if( event->type() == QEvent::NonClientAreaMouseButtonRelease ) {
            //qDebug() << "Event type:" << event->type();
            checkEdge();
        }
        if ( event->type() == QEvent::Move ) {
            QMoveEvent *moveEvent = static_cast<QMoveEvent*>(event);
            //qDebug() << "Move event:" << moveEvent->pos();
        }
        return QWidget::eventFilter(obj, event);
}


void MainWindow::checkEdge(){
    QRect rect = getWindowPositionAndSize();
    QRect screenRect = screen->availableGeometry();

     if ( rect.right() > screenRect.right()-100  ) {
        qDebug("Within!!!");
        move( screenRect.right()-rect.width(), pos().y() );
     }
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    qDebug()<<"moveEvent:"<<e;
        //e->ignore();

}

/*****
 * below code from to move window in client area:
 * http://www.qtcentre.org/threads/11903-How-do-I-natively-move-a-QWidget-top-level-window
 *
void mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
****/


void MainWindow::setWindowPositionAndSize( QRect& fg) {resize(fg.size()); move(fg.topLeft());}
QRect MainWindow::getWindowPositionAndSize()  { return QRect(pos(), size()); }

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
