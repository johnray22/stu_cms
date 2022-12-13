#ifndef STUMNG_H
#define STUMNG_H
#include <QWidget>
#include "pwddialog.h"

namespace Ui {
class stumng;
}

class stumng : public QWidget
{
    Q_OBJECT

public:
    explicit stumng(QWidget *parent = nullptr);
    ~stumng();

private slots:
    void on_back_clicked();

    void on_btn_save_clicked();

    void on_btn_revert_clicked();

    void on_btn_search_clicked();

    void on_btn_disall_clicked();

    void on_btn_del_clicked();

    void on_btn_add_clicked();

    void build();

    void on_btn_chngpwd_clicked();
    void showDialog();

    void on_btn_edit_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::stumng *ui;
};

#endif // STUMNG_H
