#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H


#include "connection.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    void keyPressEvent(QKeyEvent *event);    //设置回车响应函数

signals:
  void showmain();

private slots:
  void on_pB_Login_clicked();
  void on_pB_Register_clicked();
       void receiveExit();

void on_lineEdit_pwd_textChanged(const QString &arg1);
  void on_lineEdit_user_textChanged(const QString &arg1);

private:
    Ui::LoginDialog *ui;

};
extern void receiveExit();

#endif // LOGINDIALOG_H
