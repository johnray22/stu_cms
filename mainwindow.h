#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "logindialog.h"
#include <QMainWindow>
#include "stumng.h"
#include "scoremng.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
       void receiveLogin();

private slots:
       void on_usermng_clicked();

       void on_exit_clicked();
       void on_scoremng_clicked();

       void on_edit_clicked();

       void on_chngpwd_clicked();
       void aboutAction();

signals:
  void showlogin();

private:
    Ui::MainWindow *ui;
    stumng *mngview;
    scoremng *scoreview;
};
extern void receiveLogin();
#endif // MAINWINDOW_H
