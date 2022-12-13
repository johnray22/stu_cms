#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QBoxLayout>
#include <QCryptographicHash>
extern QString loginid; //全局变量，保存userid
extern int privil;      //全局变量，保存权限

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); //创建数据库
    db.setHostName("ali.zhongruigo.com");                  //域名
    db.setDatabaseName("studentcms");                      //数据库名
    db.setUserName("root");                                //用户名
    db.setPassword("liboBOLI22!&");                        //密码
    if (!db.open())
    { //如果无法连接
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),
                              db.lastError().text(), QMessageBox::Cancel);
        return false;
    }
    // 如果 MySQL 数据库中已经存在同名的表， 那么下面的代码不会执行
    QSqlQuery query(db);

    // 创建 stu_info 表
    if (!query.exec("CREATE TABLE `studentcms`.`stu_info`  ("
                    "`stuno` int(11) NOT NULL,"
                    "`stuname` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
                    "`pwd` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
                    "`birth` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,"
                    "`sex` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,"
                    "`region` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,"
                    "`grade` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,"
                    "`class` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,"
                    "PRIMARY KEY (`stuno`) USING BTREE"
                    ") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;"))
    {
        qDebug() << "Error: Fail to create table." << query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
    //创建stu_score表
    if (!query.exec("CREATE TABLE `studentcms`.`stu_score`  ("
                    "`stuno` int(11) NOT NULL,"
                    "`chinese` double NULL DEFAULT NULL,"
                    "`math` double NULL DEFAULT NULL,"
                    "`english` double NULL DEFAULT NULL,"
                    "PRIMARY KEY (`stuno`) USING BTREE"
                    ") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;"))
    {
        qDebug() << "Error: Fail to create table." << query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
    return true;
}
#endif // CONNECTION_H
