#include "stumng.h"
#include "ui_stumng.h"
#include "connection.h"
#include "main.h"
#include <QInputDialog>
#include "typedef.h"
#include <QSqlRecord>

QSqlTableModel *model;
stumng::stumng(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::stumng)
{
    ui->setupUi(this);
    build();
}

stumng::~stumng()
{
    delete ui;
}

void stumng::on_btn_chngpwd_clicked() //弹出修改密码对话框
{
    bool isOK;
    QString pwd = QInputDialog::getText(NULL, "修改密码", "请输入密码", QLineEdit::Normal, "", &isOK);//弹出对话框
    if (isOK)
    {
        QString md5pwd = QCryptographicHash::hash(pwd.toLatin1(), QCryptographicHash::Md5).toHex(); //md5加密
        model->setData(model->index(ui->tableView->currentIndex().row(), 2), md5pwd);               //重新设置当前单元格内容
        model->submitAll();                                                                         //提交修改
        on_btn_disall_clicked();//刷新
    }
}

void stumng::build()//初始化
{
    model = new QSqlTableModel;
    ui->tableView->setModel(model);         //设置tableview
    ui->tableView->setSortingEnabled(true); //排序
    on_btn_disall_clicked();
}

void stumng::on_btn_disall_clicked()
{
    model->setTable("stu_info");                            //设置表
    model->select();                                        //选择操作
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置策略
    QString title[10] = {"学/工号", "姓名", "密码", "生日", "性别", "籍贯", "年级", "班级", "权限"};
    QString title2[8] = {"学/工号", "姓名", "生日", "性别", "籍贯", "年级", "班级"};
    if (privil < 100){                                       //如果不是管理员
        model->removeColumn(8);             //权限栏
        model->removeColumn(2);             //移除密码栏
        ui->btn_chngpwd->setDisabled(true); //禁用修改密码按钮
    }
    for (int i = 0; i < model->columnCount(); i++)
    {
        model->setHeaderData(i, Qt::Horizontal, (privil < 100 ? title2[i] : title[i])); //设置标题栏
    }
}

void stumng::on_back_clicked()
{
    if (model->isDirty())
    {
        // 创建一个Warning弹出对话框，添加三个按钮
        QMessageBox *qMsgBox = new QMessageBox(QMessageBox::Warning, tr("返回警告"), tr("您还未保存数据，是否保存？"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Ignore);
        qMsgBox->button(QMessageBox::Yes)->setText("保存"); //更改文本显示
        qMsgBox->button(QMessageBox::Ignore)->setText("不保存");
        qMsgBox->button(QMessageBox::No)->setText("取消");
        // 启动对话框，用res变量记录用户最终点选的按钮
        int res = qMsgBox->exec();
        if (QMessageBox::Yes == res)//选择保存
            model->submitAll(); //提交
        else if (QMessageBox::No == res)//选择取消
            return; //取消
        else
            model->clear();//选择不保存，则清除
    }
    this->close(); //返回按钮
}

void stumng::on_btn_save_clicked()
{
    model->database().transaction();
    if (model->submitAll())
    {                               //事物
        model->database().commit(); //提交
    }else{
        model->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"),
                             tr("数据库错误: %1").arg(model->lastError().text()));
    }
}

void stumng::on_btn_revert_clicked()
{
    model->revertAll(); //回滚操作
}

void stumng::on_btn_search_clicked()
{
    QString name = ui->lineEdit->text();
    if (name.length())
    {
        //根据姓名、学号进行模糊筛选，使用单引号
        model->setFilter(QString("stuno like '%%1%' or stuname like '%%2%'").arg(name).arg(name));
        model->select();
    } else{
        QMessageBox::warning(this, tr("Warning"), tr("内容不能为空"));
    }
}

void stumng::on_btn_del_clicked()
{
    int curRow = ui->tableView->currentIndex().row(); // 获取选中的行
    model->removeRow(curRow);                         // 删除该行
    int ok = QMessageBox::warning(this, tr("删除当前行!"),
                                  tr("你确定删除当前行吗？ "), QMessageBox::Yes, QMessageBox::No);
    if (ok == QMessageBox::No)
    { // 如果不删除， 则撤销
        model->revertAll();
    }
    else
    { // 否则提交， 在数据库中删除该行
        model->submitAll();
    }
}

void stumng::on_btn_add_clicked()
{
    if (privil < 100){ //不是管理员
        showDialog();
    }else{ //管理员
        int rowNum = model->rowCount();// 获得表的行数
        model->insertRow(rowNum);// 添加一行
    }
}

void stumng::showDialog()
{ //显示添加窗口
    pwdDialog dialog;
    bool ok = false;
    //    qDebug()<<"cur value"<<model->record(ui->tableView->currentIndex().row()).value("stuno").toString();
    //    qDebug()<<"cur value"<<model->record(ui->tableView->currentIndex().row()).value("学号").toString();
    dialog.commitdata(model->record(ui->tableView->currentIndex().row()).value("stuno").toString(), 1);
    dialog.exec();
    on_btn_disall_clicked();
}

void stumng::on_btn_edit_clicked()
{
    pwdDialog dialog;
    if (ui->tableView->currentIndex().row() < 0)
    {
        QMessageBox::warning(this, tr("提示"),tr("请选择行"));
        return;
    }
    //传输学号和操作类型
    dialog.commitdata(model->record(ui->tableView->currentIndex().row()).value("stuno").toString(), 0);
    stuInfo *stuinfo = new stuInfo;
    int index_ = ui->tableView->currentIndex().row();//获取所在行的信息
    stuinfo->stuno = model->record(index_).value("stuno").toString();
    stuinfo->name = model->record(index_).value("stuname").toString();
    stuinfo->sex = model->record(index_).value("sex").toString();
    stuinfo->birth = model->record(index_).value("birth").toString();
    stuinfo->region = model->record(index_).value("region").toString();
    stuinfo->grade = model->record(index_).value("grade").toString();
    stuinfo->classes = model->record(index_).value("class").toString();
    dialog.setinput(stuinfo);//将获取的信息传输过去显示
    dialog.exec();//执行模态框
    on_btn_disall_clicked();//刷新
}

void stumng::on_lineEdit_textChanged(const QString &arg1)
{
    if (ui->realtime->isChecked())
    {
        on_btn_search_clicked();
    }
}
