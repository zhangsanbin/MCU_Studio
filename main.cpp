#include "mcu_studio.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(mdi);

    QApplication a(argc, argv);
    QDesktopWidget *pDesk = QApplication::desktop();
    MCU_Studio w(pDesk);
    w.resize(800, 600);
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 2);
    w.show();
    return a.exec();

    /*QApplication a(argc, argv);
    Builder avr_builder ;
    avr_builder.start();
    return a.exec();*/

}
