#include "com.h"
#include "ui_com.h"

COM::COM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::COM)
{
    ui->setupUi(this);

    ui->closeMyComBtn->setEnabled(false); //开始“关闭串口”按钮不可用

    ui->sendMsgBtn->setEnabled(false); //开始“发送数据”按钮不可用

    ui->sendMsgBtn->setDefault(true);

    open=false;
}

COM::~COM()
{
    delete ui;
}

void COM::on_openMyComBtn_clicked()
{

    QString portName = ui->portNameComboBox->currentText(); //获取串口名

    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    myCom = new Posix_QextSerialPort(portName,QextSerialBase::Polling);

    if(myCom->open(QIODevice::ReadWrite))	//打开串口
    {
        open=true;

        //设置波特率
        if(ui->baudRateComboBox->currentText()==tr("9600")) //根据组合框内容对串口进行设置

            myCom->setBaudRate(BAUD9600);

        else if(ui->baudRateComboBox->currentText()==tr("115200"))

            myCom->setBaudRate(BAUD115200);


        //设置数据位
        if(ui->dataBitsComboBox->currentText()==tr("8"))

            myCom->setDataBits(DATA_8);

        else if(ui->dataBitsComboBox->currentText()==tr("7"))

            myCom->setDataBits(DATA_7);


        //设置奇偶校验
        if(ui->parityComboBox->currentText()==tr("无"))

            myCom->setParity(PAR_NONE);

        else if(ui->parityComboBox->currentText()==tr("奇"))

            myCom->setParity(PAR_ODD);

        else if(ui->parityComboBox->currentText()==tr("偶"))

            myCom->setParity(PAR_EVEN);


        //设置停止位
        if(ui->stopBitsComboBox->currentText()==tr("1"))

            myCom->setStopBits(STOP_1);

        else if(ui->stopBitsComboBox->currentText()==tr("2"))

            myCom->setStopBits(STOP_2);

        myCom->setFlowControl(FLOW_OFF); //设置数据流控制，我们使用无数据流控制的默认设置

        myCom->setTimeout(500); //设置延时

        //timer
        readTimer = new QTimer(this);

        readTimer->start(100);//设置延时为100ms

        //信号和槽函数关联，延时一段时间，进行读串口操作
        connect(readTimer,SIGNAL(timeout()),this,SLOT(readMyCom()));

        ui->openMyComBtn->setEnabled(false); //打开串口后“打开串口”按钮不可用

        ui->closeMyComBtn->setEnabled(true); //打开串口后“关闭串口”按钮可用

        ui->sendMsgBtn->setEnabled(true); //打开串口后“发送数据”按钮可用

        ui->baudRateComboBox->setEnabled(false); //设置各个组合框不可用

        ui->dataBitsComboBox->setEnabled(false);

        ui->parityComboBox->setEnabled(false);

        ui->stopBitsComboBox->setEnabled(false);

        ui->portNameComboBox->setEnabled(false);
    }
}

void COM::on_closeMyComBtn_clicked()
{
    open=false;

    myCom->close(); //关闭串口，该函数在win_qextserialport.cpp文件中定义

    ui->openMyComBtn->setEnabled(true); //关闭串口后“打开串口”按钮可用

    ui->closeMyComBtn->setEnabled(false); //关闭串口后“关闭串口”按钮不可用

    ui->sendMsgBtn->setEnabled(false); //关闭串口后“发送数据”按钮不可用

    ui->baudRateComboBox->setEnabled(true); //设置各个组合框可用

    ui->dataBitsComboBox->setEnabled(true);

    ui->parityComboBox->setEnabled(true);

    ui->stopBitsComboBox->setEnabled(true);

    ui->portNameComboBox->setEnabled(true);

}

void COM::on_sendMsgBtn_clicked()
{
    if (ui->isHex->checkState())
    {
        QByteArray qrst;
        qrst = HEXStringToArr(ui->lineEdit->text());
        myCom->write(qrst);
    }else
    {
        myCom->write(ui->lineEdit->text().toLatin1());
    }
}

void COM::readMyCom() //读取串口数据并显示出来

{
    //如果可用数据大于或等于 n 字节再读取
    if(myCom->bytesAvailable() >=1 )
    {

        qDebug() << "read:"<< myCom->bytesAvailable()<<" bytes";

        //读取串口缓冲区的所有数据给临时变量temp
        QByteArray temp = myCom->readAll();

        if (ui->isHex->checkState())
        {
            int data;
            data=temp.at(0)&0x000000ff;
            //ui->textBrowser->insertPlainText("," << data);
            qDebug() << "Value:"<< data;
        }else
        {
            QTextCodec *codec = QTextCodec::codecForName("gbk");
            QString string = codec->toUnicode(temp + ",");
            ui->textBrowser->insertPlainText(string);
            qDebug() << "Value:"<<string;
        }


    }


}

QByteArray COM::HEXStringToArr(QString hexstr)
{
    QByteArray qrst;
    hexstr=hexstr.trimmed();
    hexstr=hexstr.replace("",",");
    QStringList sl=hexstr.split(",");
    foreach(QString s,sl)
    {
        if(!s.isEmpty())
            qrst.append((char)s.toInt(0,16)&0xFF);
    }
    return qrst;
}

#include <QMessageBox>
void COM::closeEvent(QCloseEvent *event)
{
    if (open)
    {
        event->ignore();
        QMessageBox msgBox;
        msgBox.setText("COM Port is not closed.");
        msgBox.exec();
    }else
    {
        event->accept();
    }
}
