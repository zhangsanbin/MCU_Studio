#include "File.h"

void Write (QString Filename,QString mText)
{
    QFile mFile(Filename) ;
    if (!mFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"could not open";
        return;
    }

    QTextStream out (&mFile);
    out << mText;

    mFile.flush();
    mFile.close();

}

QString Read(QString Filename)
{
    QFile mFile(Filename);
    if (!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"could not open";
        return "";
    }

    QTextStream in (&mFile);
    QString mText=in.readAll();
    mFile.close();
    return mText;
}


