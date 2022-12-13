#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QKeyEvent>
#include "pwddialog.h"
#include <QSettings>
#include <ctime>

QSettings cfg("user.ini", QSettings::IniFormat);
QString loginid;

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pB_Login->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pB_Register->setCursor(QCursor(Qt::PointingHandCursor));
    //读取数据
    QString usersname = cfg.value("usersname").toString();
    QString passswd = cfg.value("passswd").toString();
    QString RemeberPasswd = cfg.value("remeberPasswd").toString();
    //填入
    ui->lineEdit_user->setText(usersname);
    if (RemeberPasswd == "true" && passswd != "")
    {                                         //如果保存了记住密码
        ui->lineEdit_pwd->setText(passswd);   //填入密码
        ui->rpwd->setCheckState(Qt::Checked); //选择上
    }
    else
    {
        ui->rpwd->setCheckState(Qt::Unchecked); //否则取消选择
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pB_Register_clicked() //注册
{
    pwdDialog dialog;
    dialog.commitdata(0, 1);
    dialog.exec();
    //    QString user=ui->lineEdit_user->text();
    //    QString pwd=ui->lineEdit_pwd->text();
    //    if((user == "") || (pwd == "")){//检测是否为空
    //            QMessageBox::warning(this, tr("提示"), tr("用户名或密码不能为空"), QMessageBox::Yes);
    //        }
    //    QSqlQuery query;
    //    QString strSql("select COUNT(*) from stu_info where stuno= :stuno");//查询语句
    //    query.prepare(strSql); //预查询
    //    query.bindValue(":stuno",user);//绑定值
    //    query.exec(); //执行
    //    if(query.next()){//如果查询到
    //        if(query.value(0)>0){ //如果已经有一个
    //             QMessageBox::warning(this, tr("提示"), tr("该用户已存在"), QMessageBox::Yes);
    //             return;
    //        }else{//否则
    //            strSql=("insert into stu_info (`stuno`,`pwd`) values (:stuno,:pwd)");//编辑语句
    //            query.prepare(strSql);
    //            query.bindValue(":stuno",user);
    //            QString md5pwd = QCryptographicHash::hash(pwd.toLatin1(),QCryptographicHash::Md5).toHex(); //md5加密
    //            query.bindValue(":pwd",md5pwd);//绑定数据
    //            if(!query.exec()){ //如果执行失败
    //                qDebug() << "Error: Fail to create"<< query.lastError();
    //                QMessageBox::critical(this, tr("注册失败！"), query.lastError().text(), QMessageBox::Yes);
    //            }else{
    //                QMessageBox::information(this, tr("用户注册"), tr("注册成功！"), QMessageBox::Yes);
    //            }

    //            return;
    //        }
    //    }
}
void LoginDialog::keyPressEvent(QKeyEvent *event) //回车键检测
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit ui->pB_Login->click(); //发送信号
    }
}

void LoginDialog::on_pB_Login_clicked() //登陆
{
    QString user = ui->lineEdit_user->text();
    QString pwd = ui->lineEdit_pwd->text();
    if ((user == "") || (pwd == ""))
    {
        QMessageBox::warning(this, tr("提示"), tr("用户名或密码不能为空"), QMessageBox::Yes);
        return;
    }
    QSqlQuery query;
    QString strSql("select COUNT(*) from stu_info where stuno= :stuno and pwd = :pwd"); //sql语句
    query.prepare(strSql);
    query.bindValue(":stuno", user);
    QString md5pwd = QCryptographicHash::hash(pwd.toLatin1(), QCryptographicHash::Md5).toHex(); //MD5加密
    query.bindValue(":pwd", md5pwd);
    query.exec(); //执行
    if (query.next())
    {
        if (query.value(0) == 1)
        {                    //如果查找到用户
            this->hide();    //隐藏当前窗口
            loginid = user;  //设置全局变量loginid
            emit showmain(); //显示主窗口
            QMessageBox::information(this, tr("用户登录"), tr("登陆成功"), QMessageBox::Yes);
            if (ui->rpwd->isChecked())
            { //如果勾选
                cfg.setValue("usersname", user);
                QString passwd = pwd;
                cfg.setValue("passswd", passwd); //存入
                cfg.setValue("remeberPasswd", true);
                cfg.sync(); //记录
            }
            else
            {
                cfg.setValue("passswd", ""); //否则清除
            }
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("用户名或密码错误"), QMessageBox::Yes);
            return;
        }
    }
}
void LoginDialog::on_lineEdit_user_textChanged(const QString &arg1) //验证学号
{
    bool isDigital = QString(arg1).contains(QRegExp("^\\d+$")); //仅能包含数字
    if (isDigital)
    {
        ui->usrtip->setText("格式正确");
        ui->usrtip->setStyleSheet("color:#02B61E");
    }
    else
    {
        ui->usrtip->setText("仅能包含数字");
        ui->usrtip->setStyleSheet("color:#D20112");
    }
}

void LoginDialog::on_lineEdit_pwd_textChanged(const QString &arg1) //验证密码
{
    bool isRight = QString(arg1).contains(QRegExp("^(?![0-9]+$)[0-9a-zA-Z]+")); //不能全为数字或字母
    if (isRight)
    {
        ui->pwdtip->setText("格式正确");
        ui->pwdtip->setStyleSheet("color:#02B61E");
    }
    else
    {
        ui->pwdtip->setText("至少包含2种字符类型");
        ui->pwdtip->setStyleSheet("color:#D20112");
    }
}
void LoginDialog::receiveExit() //从主界面退出
{
    this->show(); //显示登陆界面
}
