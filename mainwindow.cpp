#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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
    connect(btn, SIGNAL( clicked() ), this, SLOT( showMessage() ) );

}

void MainWindow::showMessage(){
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
