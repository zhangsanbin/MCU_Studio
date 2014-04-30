#include "builder.h"

Builder::Builder()
{
}

void Builder::run()
{
    isstop=false;

    QFile file_o("/tmp/avr.o");
    if (file_o.exists())
        file_o.remove();

    QFile file_h("/tmp/avr.hex");
    if (file_h.exists())
        file_h.remove();

    //************Start Builder***************

    QString program = "avr-gcc";
    QStringList arguments;
    arguments << "-mmcu=atmega16" << "-Wall" << "-Os" << "-o" << "/tmp/avr.o" ;
    for (int var = 0; var < files.length(); ++var) {
        arguments << files[var];
    }

    QProcess myProcess;
    myProcess.start(program, arguments);
    if (!myProcess.waitForStarted())
    {
        return ;
    }

    // 等待进程结束
    while (false == myProcess.waitForFinished())
    {
        ;
    }

    program = "avr-objcopy";
    arguments.clear();
    arguments << "-j" << ".text" << "-j" << ".data" << "-O" << "ihex" << "/tmp/avr.o" << "/tmp/avr.hex" ;
    myProcess.start(program, arguments);
    // 等待进程启动
    if (!myProcess.waitForStarted())
    {
        return ;
    }

    // 等待进程结束
    while (false == myProcess.waitForFinished())
    {
        ;
    }

    program = "avrdude";
    arguments.clear();
    arguments << "-p" << "m16" << "-c" << "usbasp" << "-e" << "-F" << "-U" << "flash:w:/tmp/avr.hex" ;
    myProcess.start(program, arguments);
    // 等待进程启动
    if (!myProcess.waitForStarted())
    {
        return ;
    }

    // 等待进程结束
    while (false == myProcess.waitForFinished())
    {
        ;
    }

    QFile file_h2("/tmp/avr.hex");
    if (file_h2.exists())
    {
        isstop=true;
        qDebug() << "Build hex ok. ";
    }else
    {
        isstop=false;
         qDebug() << "Build hex Error. ";
    }
    //****************end Builder**************************/
}






void ReMove()
{
    QFile file_o("/tmp/avr.o");
    if (file_o.exists())
        file_o.remove();

    QFile file_h("/tmp/avr.hex");
    if (file_h.exists())
        file_h.remove();
}

void StartBuilder(QStringList files,QTextBrowser *log)
{

    ReMove();

    log->setText("");
    log->setText(log->toPlainText() + "****Start**** \n");
    log->setText(log->toPlainText() + "\n");
    log->setText(log->toPlainText() + "avr-gcc -mmcu=atmega16 -Wall -Os -o avr.o avr.c ...");
    log->setText(log->toPlainText() + "\n");

    QString program = "avr-gcc";
    QStringList arguments;
    arguments << "-mmcu=atmega16" << "-Wall" << "-Os" << "-o" << "/tmp/avr.o" ;
    for (int var = 0; var < files.length(); ++var) {
        arguments << files[var];
    }

    log->setText(log->toPlainText() + "Start Builder O File...\n");
    QProcess myProcess;
    myProcess.start(program, arguments);
    if (!myProcess.waitForStarted())
    {
        log->setText(log->toPlainText() + "启动失败,编译C文件Error \n");
        return ;
    }

    // 等待进程结束
    while (false == myProcess.waitForFinished())
    {
        ;
    }
    log->setText(log->toPlainText() + "\n");
    //end o



    log->setText(log->toPlainText() + "Start Builder Hex File...\n");
    program = "avr-objcopy";
    arguments.clear();
    arguments << "-j" << ".text" << "-j" << ".data" << "-O" << "ihex" << "/tmp/avr.o" << "/tmp/avr.hex" ;
    myProcess.start(program, arguments);
    // 等待进程启动
    if (!myProcess.waitForStarted())
    {
        log->setText(log->toPlainText() + "启动失败,编译hex文件Error \n");
        return ;
    }

    // 等待进程结束
    while (false == myProcess.waitForFinished())
    {
        ;
    }
    log->setText(log->toPlainText() + "\n");
    //end hex


    log->setText(log->toPlainText() + "Start Download Hex File to MCU...\n");
    program = "avrdude";
    arguments.clear();
    arguments << "-p" << "m16" << "-c" << "usbasp" << "-e" << "-F" << "-U" << "flash:w:/tmp/avr.hex" ;
    myProcess.start(program, arguments);
    // 等待进程启动
    if (!myProcess.waitForStarted())
    {
        log->setText(log->toPlainText() + "下载进程启动失败 \n");
        return ;
    }

    // 等待进程结束
    while (false == myProcess.waitForFinished())
    {
        ;
    }
    log->setText(log->toPlainText() + "Download Hex File Ok!\n");
    log->setText(log->toPlainText() + "\n");
    //end dow


    log->setText(log->toPlainText() + "****End**** \n");
    log->setText(log->toPlainText() + "sudo avrdude -p m16 -c usbasp -e -F -U flash:w:avr.hex \n");

    QFile file_h("/tmp/avr.hex");
    if (file_h.exists())
    {
        log->setText(log->toPlainText() + "Build hex ok. \n");
    }else
    {
        log->setText(log->toPlainText() + "Build hex Error. \n");
    }

}


