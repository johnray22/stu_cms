#ifndef UTIL_H
#define UTIL_H
#include<QString>
#include<QRegExp>

static bool UtilisDigital(const QString &arg1){
    return QString(arg1).contains(QRegExp("^\\d+$"));
}
static bool Utilispwd(const QString &arg1){
    return QString(arg1).contains(QRegExp("^(?![0-9]+$)[0-9a-zA-Z]+"));
}

#endif // UTIL_H
