#ifndef MCU_STUDIO_H
#define MCU_STUDIO_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QDebug>
#include "database.h"
#include "File.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QTextEdit>
#include "highlighter.h"
#include "com/com.h"
#include <QTimer>
#include <QPixmap>
#include <QDesktopWidget>
#include <QClipboard>   //剪切板
#include <QContextMenuEvent>
#include <QMenu>

namespace Ui {
class MCU_Studio;
}

class MCU_Studio : public QMainWindow
{
    Q_OBJECT



public:
    explicit MCU_Studio(QWidget *parent = 0);
    ~MCU_Studio();
    void Init();
    void AddRoot (QString name);
    //void AddChild(QTreeWidgetItem *parent,QString name);
    void AddChild(QTreeWidgetItem *parent,QString name,QString name1);
    QString mWorkspace,mProjectPath;

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void SettingWorkspace();

    void on_actionWorkspace_triggered();

    void setupEditor(int FontSize);

    void on_actionBuildProject_triggered();

    void on_actionSerial_Communication_triggered();

    void on_actionAbout_triggered();

    void New_Screen();

    void on_actionNew_Screen_triggered();

    void on_actionCopy_triggered();

    void on_actionPlaster_triggered();

private:
    Ui::MCU_Studio *ui;
    QString mFilename;
    Highlighter *highlighter;
    QTimer *timer;
    QPixmap pixmap;

};

#endif // MCU_STUDIO_H
