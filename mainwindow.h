#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget *basewindow;
    QWebEngineView *view;

public slots:
    void showMessage();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
