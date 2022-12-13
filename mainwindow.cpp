#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include "logindialog.h"
#include "typedef.h"
#include <QDebug>
#include <QInputDialog>
#include "about.h"
int privil;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->usermng->setCursor(QCursor(Qt::PointingHandCursor));
    ui->scoremng->setCursor(QCursor(Qt::PointingHandCursor));
    ui->edit->setCursor(QCursor(Qt::PointingHandCursor));
    ui->exit->setCursor(QCursor(Qt::PointingHandCursor));
    ui->chngpwd->setCursor(QCursor(Qt::PointingHandCursor));

    QMenuBar *menuBar = new QMenuBar(this);   //创建菜单栏
    menuBar->setGeometry(0, 0, width(), 30);  //设置大小
    QMenu *sysMenu = new QMenu("系统", this); //创建系统菜单
    //创建行为(Action)
    QAction *refreshAction = new QAction("刷新信息", this);
    QAction *exitAction = new QAction("退出", this);
    //将行为添加到菜单
    sysMenu->addAction(refreshAction);
    sysMenu->addAction(exitAction);
    QMenu *aboutMenu = new QMenu("帮助", this);
    QAction *aboutAction = new QAction("关于", this);
    aboutMenu->addAction(aboutAction);
    //5.将菜单添加到菜单栏
    menuBar->addMenu(sysMenu);
    menuBar->addMenu(aboutMenu);
    //aciton连接槽函数
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(receiveLogin())); //刷新
    connect(exitAction, SIGNAL(triggered()), this, SLOT(on_exit_clicked())); //退出
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutAction()));    //关于
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::receiveLogin()
{
    this->show();
    ui->statusbar->setSizeGripEnabled(false);        //去掉状态栏右下角的三角
    ui->statusbar->showMessage("饶忠睿 2019444210"); //显示临时信息
    ui->statusbar->setStyleSheet("color:#BBBBBB");
    QSqlQuery query;
    QString strSql("select stuname,privil from stu_info where stuno= :stuno");//查询
    query.prepare(strSql);//预查询
    query.bindValue(":stuno", loginid);
    query.exec();//执行
    query.next();//获取数据
    QString info = query.value(0).toString() + "(" + loginid + ")";//将姓名、工号组合
    privil = query.value(1).toInt();//获取权限值
    QString role;
    if (privil == 100)//根据权限值设置身份
        role = "管理员";
    else if (privil == 50)
        role = "老师";
    else
        role = "学生";
    ui->label_3->setText("您好，" + role);//显示
    ui->info->setText(info);
}

void MainWindow::on_usermng_clicked()
{
    if (privil < 50)//权限不足
    {
        QMessageBox::critical(this, tr("认证提示"), tr("您的权限不足！"), QMessageBox::Yes);
        return;
    }else{
        mngview = new stumng(this);
        mngview->show();//打开用户管理
    }
}

void MainWindow::on_exit_clicked()
{
    loginid = "";
    this->close();
    emit showlogin();
}

void MainWindow::on_scoremng_clicked()
{
    if (privil < 50)//权限不足
    {
        QMessageBox::critical(this, tr("认证提示"), tr("您的权限不足！"), QMessageBox::Yes);
        return;
    }else{
        scoreview = new scoremng(this);
        scoreview->show();//打开成绩管理
    }
}

void MainWindow::on_edit_clicked()
{
    stuInfo *stuinfo = new stuInfo;//新建stuinfo结构体
    pwdDialog dialog;//建立dialog
    QSqlQuery query;
    QString strSql("select * from stu_info where stuno= :stuno"); //查询语句
    query.prepare(strSql);                                        //预查询
    query.bindValue(":stuno", loginid);                           //绑定值
    query.exec();                                                 //执行
    if (query.next())//如果查询到
    {
        dialog.commitdata(loginid, 2); //传输要更改的id和编辑类型
        stuinfo->stuno = query.value("stuno").toString(); //设置数据
        stuinfo->name = query.value("stuname").toString();
        stuinfo->sex = query.value("sex").toString();
        stuinfo->birth = query.value("birth").toString();
        stuinfo->region = query.value("region").toString();
        stuinfo->grade = query.value("grade").toString();
        stuinfo->classes = query.value("class").toString();
        dialog.setinput(stuinfo);//传输结构体
        dialog.exec();//执行模态框
        receiveLogin();//结束后刷新
    }
}

void MainWindow::on_chngpwd_clicked()
{
    bool isOK;
    QString pwd = QInputDialog::getText(NULL, "修改密码", "请输入原密码", QLineEdit::Normal, "", &isOK);
    if (isOK)
    {
        QString md5pwd = QCryptographicHash::hash(pwd.toLatin1(), QCryptographicHash::Md5).toHex();
        QSqlQuery query;
        QString strSql("select COUNT(*) from stu_info where stuno= :stuno and pwd = :pwd"); //sql语句
        query.prepare(strSql);
        query.bindValue(":stuno", loginid);
        query.bindValue(":pwd", md5pwd);
        query.exec(); //执行
        if (query.next())
        {
            if (query.value(0) == 1)
            { //如果查找到用户
                pwd = QInputDialog::getText(NULL, "修改密码", "请输入新密码", QLineEdit::Normal, "", &isOK);
                if (isOK)
                {
                    md5pwd = QCryptographicHash::hash(pwd.toLatin1(), QCryptographicHash::Md5).toHex(); //md5加密
                    strSql = "update stu_info set pwd = :pwd where stuno = :stuno";
                    query.bindValue(":stuno", loginid);
                    query.bindValue(":pwd", md5pwd);
                    query.prepare(strSql);
                    if (!query.exec())
                    { //如果执行失败
                        QMessageBox::critical(this, tr("操作失败！"), query.lastError().text(), QMessageBox::Yes);
                    }
                    else
                    {
                        QMessageBox::information(this, tr("密码修改"), tr("修改成功！"), QMessageBox::Yes);
                        return;
                    }
                }
            }
            else
            {
                QMessageBox::warning(this, tr("提示"), tr("用户名或密码不匹配！"), QMessageBox::Yes);
                on_chngpwd_clicked();
            }
        }
    }
}
void MainWindow::aboutAction()
{
    about *aboutdia = new about;
    aboutdia->show();
}
