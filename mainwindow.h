#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QtWebKitWidgets>
#include <QSystemTrayIcon>
#include <QDebug>

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

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
    void showMessageBox();
    Ui::MainWindow *ui;



    QTimer *timer;         //用于闪烁ICON的定时器
    int TimerCount;      //用于计算定时器超时次数，单数显示图标，双数不显示  并且为0时候表示没有消息
    QDesktopWidget* screen;
    QPoint dragPosition;
    void setWindowPositionAndSize( QRect& fg);
     QRect getWindowPositionAndSize();

    QList<QIcon> iconList;
    int currentIconIndex;
    void Window();
    void setVisible(bool visible) Q_DECL_OVERRIDE;
    void moveEvent(QMoveEvent *) Q_DECL_OVERRIDE;

     bool eventFilter(QObject *obj, QEvent *event);
    void checkEdge();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void showIconMenu();
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();

    void showNormalIcon();
    void showBlinkIcon();
    void updateIcon();       //定时器刚


private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

    QGroupBox *iconGroupBox;
    QLabel *iconLabel;
    QComboBox *iconComboBox;
    QCheckBox *showIconCheckBox;

    QGroupBox *messageGroupBox;
    QLabel *typeLabel;
    QLabel *durationLabel;
    QLabel *durationWarningLabel;
    QLabel *titleLabel;
    QLabel *bodyLabel;
    QComboBox *typeComboBox;
    QSpinBox *durationSpinBox;
    QLineEdit *titleEdit;
    QTextEdit *bodyEdit;
    QPushButton *showMessageButton;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

};

#endif // MAINWINDOW_H
