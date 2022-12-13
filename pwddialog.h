#ifndef PWDDIALOG_H
#define PWDDIALOG_H
#include "typedef.h"
#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class pwdDialog;
}

class pwdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit pwdDialog(QWidget *parent = nullptr);
    ~pwdDialog();
    void commitdata(QString stuno,int type);
    void getinput(stuInfo &stuinfo);
    void setinput(stuInfo *stuinfo);
private slots:
    void on_stuno_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pwd_textChanged(const QString &arg1);

    void on_grade_textChanged(const QString &arg1);

    void on_classes_textChanged(const QString &arg1);

private:
    Ui::pwdDialog *ui;
};

#endif // PWDDIALOG_H
