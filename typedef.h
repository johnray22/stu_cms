#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <QMetaType>                            //自定义参数类型的头文件----***
#include <QString>
struct stuInfo{
    QString stuno;
    QString pwd;
    QString name;
    QString sex;
    QString birth;
    QString region;
    QString grade;
    QString classes;
    int Confirm;
};
Q_DECLARE_METATYPE(stuInfo)             //要调用Q_DECLARE_METATYPE，向QT声明这个结构体

#endif // TYPEDEF_H
