#include "pwddialog.h"
#include "ui_pwddialog.h"
#include "util.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QCryptographicHash>
bool complete;
QString stuno_;
int type_;
pwdDialog::pwdDialog(QWidget *parent) : QDialog(parent),
                                        ui(new Ui::pwdDialog)
{
    ui->setupUi(this);
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
}

pwdDialog::~pwdDialog()
{
    delete ui;
}

void pwdDialog::commitdata(QString stuno, int type)
{
    type_ = type;
    stuno_ = stuno;
}
void pwdDialog::getinput(stuInfo &stuinfo)
{
    stuinfo.stuno = ui->stuno->text();
    stuinfo.pwd = ui->pwd->text();
    stuinfo.name = ui->name->text();
    stuinfo.sex = ui->comboBox->currentText();
    stuinfo.birth = ui->dateEdit->date().toString("yyyy-MM-dd");
    stuinfo.region = ui->region->text();
    stuinfo.grade = ui->grade->text();
    stuinfo.classes = ui->classes->text();
}
void pwdDialog::setinput(stuInfo *stuinfo)
{ //设置输入框内容
    ui->stuno->setText(stuinfo->stuno);
    ui->name->setText(stuinfo->name);
    ui->comboBox->setCurrentText(stuinfo->sex);                             //设置选择框内容
    ui->dateEdit->setDate(QDate::fromString(stuinfo->birth, "yyyy-MM-dd")); //格式化日期格式
    ui->region->setText(stuinfo->region);
    ui->grade->setText(stuinfo->grade);
    ui->classes->setText(stuinfo->classes);
    if (type_ == 0 || type_ == 2)
    {                                 //如果是修改或注册操作
        ui->pwd->setDisabled(true);   //禁用密码修改框
    }
    if(type_==2){
                ui->stuno->setDisabled(true); //禁用学号修改框
    }
}

void pwdDialog::on_pushButton_clicked()
{
    if (!complete || ui->stuno->text() == "" || ui->name->text() == "")//检测是否为空，complete代表是否填写正确
    { QMessageBox::warning(this, tr("提示"),tr("请将信息填写正确"));
    return;
    }
    QSqlQuery query;
    //编辑SQL插入、修改语句
    QString insertSql = ("insert into stu_info (`stuno`,`stuname`,`pwd`,`birth`,`sex`,`region`,`grade`,`class`) values (:stuno,:stuname,:pwd,:birth,:sex,:region,:grade,:class)");  //编辑语句
    QString editSql = ("update stu_info set `stuno`= :stuno ,`stuname`=:stuname,`birth`=:birth,`sex`=:sex,`region`=:region,`grade`=:grade,`class`=:class where stuno = :prestuno"); //编辑语句
    if (type_ == 1){//如果是添加操作
        query.prepare(insertSql);//预执行
    }else//如果是编辑操作
        query.prepare(editSql);//预执行
    //数据操作
    query.bindValue(":stuno", ui->stuno->text());//绑定数据
    query.bindValue(":stuname", ui->name->text());
    QString md5pwd = QCryptographicHash::hash(ui->pwd->text().toLatin1(), QCryptographicHash::Md5).toHex();//对密码md5加密
    query.bindValue(":pwd", md5pwd);
    query.bindValue(":birth", ui->dateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":sex", ui->comboBox->currentText());
    query.bindValue(":region", ui->region->text());
    query.bindValue(":grade", ui->grade->text());
    query.bindValue(":class", ui->classes->text());
    query.bindValue(":prestuno", stuno_);
    if (!query.exec()){ //如果执行失败
        QMessageBox::critical(this, tr("操作失败！"), query.lastError().text(), QMessageBox::Yes);
    }else{
        QMessageBox::information(this, tr("信息变动"), type_ == 1 ? tr("添加成功！") : tr("修改成功！"), QMessageBox::Yes);
        this->close();
    }
}

void pwdDialog::on_pushButton_2_clicked()
{
    this->close();
}

void pwdDialog::on_stuno_textChanged(const QString &arg1)
{
    if (type_ == 1||(type_==0&&arg1!=stuno_))
    {
        QSqlQuery query;
        QString strSql("select COUNT(*) from stu_info where stuno= :stuno"); //查询语句
        query.prepare(strSql);                                               //预查询
        query.bindValue(":stuno", arg1);                                     //绑定值
        query.exec();                                                        //执行
        if (query.next())
        { //如果查询到
            if (query.value(0) > 0){ //如果已经有一个
                ui->label_no->setText("该用户已存在");
                ui->label_no->setStyleSheet("color:#D20112");
                complete = false;
                return;
                }else{
                if (!UtilisDigital(arg1))
                {
                    ui->label_no->setText("仅能包含数字");
                    ui->label_no->setStyleSheet("color:#D20112");
                    complete = false;
                }
                else
                {
                    ui->label_no->setText("格式正确");
                    ui->label_no->setStyleSheet("color:#02B61E");
                    complete = true;
                }
            }
        }
    }
}

void pwdDialog::on_pwd_textChanged(const QString &arg1)
{
    if (!Utilispwd(arg1))
    {
        ui->label_pwd->setText("需至少包含两种字符");
        ui->label_pwd->setStyleSheet("color:#D20112");
        complete = false;
    }
    else
    {
        ui->label_pwd->setText("格式正确");
        ui->label_pwd->setStyleSheet("color:#02B61E");
        complete = true;
    }
}

void pwdDialog::on_grade_textChanged(const QString &arg1)
{
    if (!UtilisDigital(arg1))
    {
        ui->label_grade->setText("仅能包含数字");
        ui->label_grade->setStyleSheet("color:#D20112");
        complete = false;
    }
    else
    {
        ui->label_grade->setText("格式正确");
        ui->label_grade->setStyleSheet("color:#02B61E");
        complete = true;
    }
}

void pwdDialog::on_classes_textChanged(const QString &arg1)
{
    if (!UtilisDigital(arg1))
    {
        ui->label_grade->setText("仅能包含数字");
        ui->label_grade->setStyleSheet("color:#D20112");
        complete = false;
    }
    else
    {
        ui->label_grade->setText("格式正确");
        ui->label_grade->setStyleSheet("color:#02B61E");
        complete = true;
    }
}
