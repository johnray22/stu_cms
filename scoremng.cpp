#include "scoremng.h"
#include "ui_scoremng.h"
#include "connection.h"
#include <QInputDialog>
#include <QSqlRecord>
QSqlTableModel *model_s;
scoremng::scoremng(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::scoremng)
{
    ui->setupUi(this);
    build();
}

scoremng::~scoremng()
{
    delete ui;
}
void scoremng::build()
{
    model_s = new QSqlTableModel;
    ui->tableView->setModel(model_s);       //设置tableview
    ui->tableView->setSortingEnabled(true); //排序
    on_btn_disall_clicked();
}

void scoremng::on_back_clicked()
{
    if (model_s->isDirty())
    {
        // 创建一个Warning弹出对话框，添加三个按钮
        QMessageBox *qMsgBox = new QMessageBox(QMessageBox::Warning, tr("返回警告"), tr("您还未保存数据，是否保存？"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Ignore);
        qMsgBox->button(QMessageBox::Yes)->setText("保存"); //更改文本显示
        qMsgBox->button(QMessageBox::Ignore)->setText("不保存");
        qMsgBox->button(QMessageBox::No)->setText("取消");
        // 启动对话框，用res变量记录用户最终点选的按钮
        int res = qMsgBox->exec();
        if (QMessageBox::Yes == res)
            model_s->submitAll(); //提交
        else if (QMessageBox::No == res)
            return; //取消
        else
            model_s->clear();
    }
    this->close(); //返回按钮
}

void scoremng::on_btn_search_clicked()
{
    QString name = ui->lineEdit->text();
    if (name.length())
    {
        //根据姓名进行筛选，使用单引号
        model_s->setFilter(QString("stuno like '%%1%' or stuname like '%%2%'").arg(name).arg(name));
        model_s->select();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("内容不能为空"));
    }
}

void scoremng::on_btn_disall_clicked()
{
    QSqlQuery query;
    //同步用户表的信息到成绩表
    query.prepare("insert into stu_score(stuno,stuname) SELECT stu_info.stuno,stu_info.stuname from stu_info where privil<10 or privil is null;");
    query.exec();
    model_s->setTable("stu_score");                           //设置表
    model_s->select();                                        //选择操作
    model_s->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置策略
    model_s->setHeaderData(0, Qt::Horizontal, "学号");        //设置标题栏
    model_s->setHeaderData(1, Qt::Horizontal, "姓名");        //设置标题栏
}

void scoremng::on_btn_revert_clicked()
{
    model_s->revertAll(); //回滚操作
}

void scoremng::on_btn_save_clicked()
{
    model_s->database().transaction();
    if (model_s->submitAll())
    {                                 //事物
        model_s->database().commit(); //提交
    }
    else
    {
        model_s->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"),tr("数据库错误: %1").arg(model_s->lastError().text()));
    }
}

void scoremng::on_btn_add_clicked()
{
    int colNum = model_s->columnCount(); // 获得表的列数
    bool isOK;
    //获取要添加的名称
    QString subject = QInputDialog::getText(NULL, "新增列", "请输入科目名称", QLineEdit::Normal, "", &isOK);
    if (isOK){ 
        QSqlQuery query;
        query.prepare("alter TABLE stu_score add COLUMN " + subject + " double"); //alter语句
        if (!query.exec()){ //如果执行失败，则打印错误信息
            QMessageBox::critical(this, tr("添加失败！"), query.lastError().text(), QMessageBox::Yes);
        }else{
            QMessageBox::information(this, tr("添加科目"), tr("添加成功！"), QMessageBox::Yes);
            on_btn_disall_clicked(); //刷新
        }
    }
}

void scoremng::on_lineEdit_textChanged(const QString &arg1)
{
    if (ui->realtime->isChecked())
    {
        on_btn_search_clicked();
    }
}

