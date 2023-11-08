#ifndef JSDBPARAMETERS_H
#define JSDBPARAMETERS_H
#include <QString>

struct JSDBParameters
{
    QString dbTypeStr; //"MYSQL");
    QString hostStr; // "127.0.0.1");
    QString portStr; //
    QString databaseName;
    QString databaseUser;
    QString databasePassword;
};

#endif // JSDBPARAMETERS_H
