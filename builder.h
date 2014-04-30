#ifndef BUILDER_H
#define BUILDER_H
#include <QtCore>
#include <QStringList>
#include <QTextBrowser>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QDir>
#include <QTextBrowser>
#include <QProcess>

class Builder :public QThread
{
public:
    Builder();
    void run();
    QStringList files;
    QTextBrowser *log;
    bool isstop;
    void StartBuilder(QStringList files,QTextBrowser *log);
};

#endif // BUILDER_H
