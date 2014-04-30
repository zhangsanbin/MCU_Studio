#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>

#include <iostream>
#include "unistd.h"

static bool createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if(access("sys.db",0))//If erp.db exist,access() return 0;
    {
        db.setDatabaseName("sys.db");
        if(!db.open()) return false;
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS tbsys_settings(setting TEXT UNIQUE, value TEXT)");

        return true;
    }
    else {
        db.setDatabaseName("sys.db");
        if(!db.open()) return false;
        return true;
    }
    return false;
}

static bool ExecuteSql(const QString &sql)
{
    QSqlQuery query;
    bool bSuccess = query.exec(sql);
    if (bSuccess)
    {
        return true;
    }
    else
    {
        return false;
    }
     return false;
}

static bool OpenDb(const QString &DbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DbName);
    if(!db.open()) return false;
    return true;
}


#endif // DATABASE_H
