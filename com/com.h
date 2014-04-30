#ifndef COM_H
#define COM_H

#include <QWidget>
#include "posix_qextserialport.h"
#include <QTimer>
#include <QDebug>
#include <QTextCodec>
#include <QCloseEvent>

namespace Ui {
class COM;
}

class COM : public QWidget
{
    Q_OBJECT

public:
    explicit COM(QWidget *parent = 0);
    ~COM();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_openMyComBtn_clicked();

    void on_closeMyComBtn_clicked();

    void on_sendMsgBtn_clicked();

    void readMyCom();

    QByteArray HEXStringToArr(QString hexstr);

private:
    Ui::COM *ui;
    Posix_QextSerialPort *myCom;
    QTimer *readTimer;
    bool open;

};

#endif // COM_H
