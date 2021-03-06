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
//    minimizeAction = new QAction(tr("最小化"), this);
//    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

//    maximizeAction = new QAction(tr("最大化"), this);
//    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

//    restoreAction = new QAction(tr("恢复"), this);
//    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormalIcon()));

    quitAction = new QAction(tr("退出"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
//    trayIconMenu->addAction(restoreAction);
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

    //minimizeAction->setEnabled(visible);
    //maximizeAction->setEnabled(!isMaximized());
    //restoreAction->setEnabled(isMaximized() || !visible);
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
        //trayIcon->setContextMenu(trayIconMenu);
        //trayIcon->setContextMenu( new QMenu(this) );


        break;
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        messageClicked();


//        currentIconIndex = (currentIconIndex+1) % iconList.count();
//        setIcon(currentIconIndex);


        break;
    case QSystemTrayIcon::MiddleClick:

        break;
    default:
        ;
    }
}
//! [4]

//! [5]
void MainWindow::showMessage(QString title, QString msg="", int iconNum=1)
{
    QSystemTrayIcon::MessageIcon icon=QSystemTrayIcon::MessageIcon(iconNum);  //设置图表是标准的系统托盘  信息
    trayIcon->showMessage(title, msg, icon, 5 * 1000);
}
//! [5]

//! [6]
void MainWindow::messageClicked()
{
    this->show();
    this->setWindowState( (this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

    if(TimerCount>0){
        showNormalIcon();
        //QMessageBox::information(0, tr("Systray"), tr("Sorry, I already gave what help I could.\nMaybe you should try asking a human?"));
        if(pdfurl!=""){
            openPDFWindow(pdfurl);
        }
    }
}
//! [6]


void MainWindow::findUrlByPdfView(QWidget *pdfView){
    QMapIterator<QString, QWebEngineView*> i(pdfViewList);
    while (i.hasNext()) {
        i.next();
        if(pdfView == i.value()){
            qDebug() << i.key() << ": " << i.value() << endl;
            pdfViewList.remove(i.key());
        }
    }
}

void MainWindow::onPDFViewClose(QObject* pdfView){
    QWidget* _pdfView = qobject_cast<QWidget*>(pdfView);
    findUrlByPdfView(_pdfView);
    qDebug()<<"Exit:"<<_pdfView;

}

void MainWindow::openPDFWindow(QString url){
    QWebEngineView * pdfView;
    if(!pdfViewList.contains(url)){
        pdfView = new QWebEngineView(0);
        QUrl theurl = QUrl(url);
        pdfView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        pdfView->load( theurl );
        pdfView->show();
        pdfViewList.insert(url, pdfView);
        pdfView->setAttribute(Qt::WA_DeleteOnClose);
        pdfView->showNormal();

        int frameHeight = pdfView->frameGeometry().height()-pdfView->geometry().height();
        pdfView->move( (screenRect.width() - 640)/2, screenRect.y() );
        pdfView->resize( 640, screenRect.height()-frameHeight );

        connect(pdfView, SIGNAL(destroyed(QObject*)), this, SLOT(onPDFViewClose(QObject*)) );
        //pdfView->settings()->enablePersistentStorage(QDir::tempPath());
        QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    }else{
        pdfView =pdfViewList.value(url);
    }
    qDebug()<<pdfView;
    qApp->setActiveWindow(pdfView);
    pdfView->setWindowState( (pdfView->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    pdfView->activateWindow();
    pdfView->raise();
}


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
    timer->stop();
    timer->start(500);  //每500ms都刷新一次
    timer->setSingleShot(true);  //如果为真，表示只重复一次,为假表示无限次循环
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
    if(TimerCount>0){
        timer->start(500);  //每500ms都刷新一次
    }
}

void MainWindow::mainViewLoadFinished(bool ok){
    if(ok){
        qDebug()<<"Web loaded."<<ok;
        //mainView->mainFrame()->evaluateJavaScript(tr("msg('iogjweoij')"));
        mainView->page()->runJavaScript(tr("msg('iogjweoij')"));
    }
}


QString rstrip(const QString& str) {
  int n = str.size() - 1;
  for (; n >= 0; --n) {
    if (!str.at(n).isSpace()) {
      return str.left(n + 1);
    }
  }
  return "";
}


/******
 * we use title to pass parameters, limit is 10K=10240B
 * think pass data batchly. In Windows+Webkit limit is 1K=1024B
 */
void MainWindow::mainViewTitleChanged(QString str){

    str = str.trimmed();
    qDebug()<<str;
    if(str=="")return;

    showBlinkIcon();
    showMessage(tr("json received"), str, 1);

    QByteArray ba = str.toUtf8();
    QJsonDocument json = QJsonDocument::fromJson( ba );

    if(json.isArray()){
        QJsonObject d = json.array()[0].toObject();
        //QMessageBox::information(this, "", d["name"].toString());
    }

    if(json.isObject()){
        QJsonObject jsonObj = json.object();
        if(jsonObj["action"]=="openPDF"){
            pdfurl = jsonObj["url"].toString();
            qDebug()<<pdfurl;
        }
    }

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    MainParent = parent;

    timer=new QTimer(this);
    TimerCount=0;  //初始化为零
    Window();

    // transparent background
//    setAttribute(Qt::WA_TranslucentBackground);
//    setStyleSheet("background:transparent;");

    // no window decorations
//    setWindowFlags(Qt::FramelessWindowHint);

    //ui->setupUi(this);

    basewindow = new QWidget;
    basewindow->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setCentralWidget(basewindow);

    pdfurl = "";
    //using webengine
    mainView = new QWebEngineView(this);
    //using webkit
//    mainView = new QWebView(this);
    mainView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    setCentralWidget(mainView);
    mainView->resize(this->size());
    mainView->load(QUrl("http://1111hui.com/pdf/client/click.html"));
    mainView->show();

    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);

    connect(mainView, SIGNAL(loadFinished(bool)), this, SLOT(mainViewLoadFinished(bool)) );
    connect(mainView, SIGNAL(titleChanged(QString)), this, SLOT(mainViewTitleChanged(QString)) );


    QPushButton * btn = new QPushButton(this);
    btn->setText("OISJDOFJ");
    btn->show();
    connect(btn, SIGNAL( clicked() ), this, SLOT( showBlinkIcon() ) );


    resize(320, 640);

    screen = new QDesktopWidget();
    QRect rect = getWindowPositionAndSize();
    screenRect = screen->availableGeometry();
    rect.moveTopRight( screenRect.topRight() );
    setWindowPositionAndSize(rect);
    qDebug()<<"screen"<<screen->screenGeometry()<<screen->availableGeometry();
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
    QRect rect = frameGeometry();
    QRect screenRect = screen->availableGeometry();

     if ( rect.right() > screenRect.right()-30  ) {
        //qDebug("Within!!!");
        move( screenRect.right()-rect.width(), pos().y() );
     }
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    //qDebug()<<"moveEvent:"<<e;
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
//    QString factor = QString::number( view->page()->zoomFactor() );
    QString factor="";
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
