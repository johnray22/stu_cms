#ifndef SCOREMNG_H
#define SCOREMNG_H

#include <QWidget>

namespace Ui {
class scoremng;
}

class scoremng : public QWidget
{
    Q_OBJECT

public:
    explicit scoremng(QWidget *parent = nullptr);
    ~scoremng();

private slots:
    void on_back_clicked();

    void on_btn_search_clicked();

    void on_btn_disall_clicked();

    void on_btn_revert_clicked();

    void on_btn_save_clicked();

    void on_btn_add_clicked();

    void build();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::scoremng *ui;
};

#endif // SCOREMNG_H
