#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>
#include "connection.h"
#include <QStyleFactory>
#include"typedef.h"
static bool createConnection();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
    qRegisterMetaType<stuInfo>("stuInfo");
    MainWindow w;
    LoginDialog login;
    login.setWindowTitle("管理系统-登陆"); //登录框标题
    login.setFixedSize(345,240); //固定大小
    login.show(); //显示
    createConnection();//建立数据库连接
    w.setFixedSize(800,540);
    w.setWindowTitle("学生信息管理系统");//设置标题
    QObject::connect(&login, SIGNAL(showmain()), &w, SLOT(receiveLogin()));//登录框与主程序建立链接
    QObject::connect(&w, SIGNAL(showlogin()), &login, SLOT(receiveExit()));//反向链接
    w.move((QApplication::desktop()->width() - w.width()) / 2, (QApplication::desktop()->height() - w.height()) / 2);  // 窗口设置居中显示
    return a.exec();
}
