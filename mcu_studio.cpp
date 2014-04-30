#include "mcu_studio.h"
#include "ui_mcu_studio.h"
#include "builder.h"
#include <QCoreApplication>

MCU_Studio::MCU_Studio(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MCU_Studio)
{
    ui->setupUi(this);
    Init();

}

MCU_Studio::~MCU_Studio()
{
    delete ui;
}

//setTypeStyle
void MCU_Studio::setupEditor(int FontSize)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(FontSize);

    ui->textEdit->setFont(font);

    highlighter = new Highlighter(ui->textEdit->document());
}

void MCU_Studio::AddRoot (QString name)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,name);
    ui->treeWidget->addTopLevelItem(itm);

    QDir mDir(mWorkspace + "/" + name);
    QStringList filters;
    filters << "*.c" << "*.h" << "*.cpp";
    mDir.setNameFilters(filters);
    foreach (QFileInfo mitm,mDir.entryInfoList())
    {
        if(mitm.isFile())
        {
            AddChild(itm,mitm.fileName(),name);
        }
    }

}

void MCU_Studio::AddChild(QTreeWidgetItem *parent,QString name,QString path)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0,name);
    itm->setData(0,1,path);
    parent->addChild(itm);
}

void MCU_Studio::Init()
{
    createDatabase();

    OpenDb("sys.db");
    QSqlQuery query("SELECT value FROM tbsys_settings where setting='workspace'");
    while (query.next()) {
        mWorkspace = query.value(0).toString();
    }

    if (mWorkspace.isEmpty())
    {
        SettingWorkspace();
    }

    //[1]Setting Workspace list
    ui->treeWidget->setHeaderLabel("Workspace");
    QDir mDir(mWorkspace);
    foreach (QFileInfo mitm,mDir.entryInfoList())
    {
        if(mitm.isDir())
        {
           if(!(mitm.fileName()==".." | mitm.fileName()=="."))
            {
                AddRoot(mitm.fileName());
            }
        }
    }//[1]

    //[2]Setting Code Type Style
    setupEditor(10);
    //[2]
}

void MCU_Studio::SettingWorkspace()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Select Project Workspace"),
        tr("Workspace paths:"), QLineEdit::Normal,
        mWorkspace, &ok);
    if (ok && !text.isEmpty())
    {
        mWorkspace= text;
    }
    else
    {
        return;
    }

    QString sql;
    sql = "delete from tbsys_settings where setting='workspace'";
    qDebug()<<sql;
    ExecuteSql(sql);
    sql = "insert into tbsys_settings values('workspace','"+ mWorkspace +"')";
    qDebug()<<sql;
    ExecuteSql(sql);
}

void MCU_Studio::on_actionNew_triggered()
{
    mFilename = QFileDialog::getSaveFileName(this, tr("Save File"),
                               mWorkspace,
                               tr("Project(*.mcu *.c *.h *.cpp)"));
    ui->textEdit->setText(Read(mFilename));
}

void MCU_Studio::on_actionOpen_triggered()
{
    QStringList files;
    files = QFileDialog::getOpenFileNames(
                    this, tr("Open Project file"),
                    mWorkspace,
                    tr("Project(*.mcu *.c *.h *.cpp);;All(*.*)"));
        if (files.count()) {
            mFilename = files[0];
            qDebug() << mFilename;
        }
  ui->textEdit->setText(Read(mFilename));

}

void MCU_Studio::on_actionSave_triggered()
{
    Write(mFilename,ui->textEdit->toPlainText());
    ui->textEdit->document()->setModified(false);
}

void MCU_Studio::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (ui->textEdit->document()->isModified())
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
                // Save was clicked
                Write(mFilename,ui->textEdit->toPlainText());
                break;
          case QMessageBox::Discard:
                // Don't Save was clicked
                break;
          case QMessageBox::Cancel:
                // Cancel was clicked
                return;
                break;
          default:
                // should never be reached
                return;
                break;
        }
    }

    mProjectPath = mWorkspace + "/" + item->data(0,1).toString() ;
    mFilename = mProjectPath + "/" + item->text(0);
    ui->textEdit->setText(Read(mFilename));
    ui->tabWidget->setTabText(0,mFilename);
    ui->statusbar->showMessage("Builder Path: " + mProjectPath);
}

void MCU_Studio::on_actionWorkspace_triggered()
{
    SettingWorkspace();
}


void MCU_Studio::on_actionBuildProject_triggered()
{
    QDir mDir(mProjectPath);
    QStringList filters,files;
    filters << "*.c";
    mDir.setNameFilters(filters);
    foreach (QFileInfo mitm,mDir.entryInfoList()) {
        if(mitm.isFile())
        {
            files << mitm.filePath();
        }
    }
    //StartBuilder(files,ui->textBrowser);

    Builder *avr_builder = new Builder();
    avr_builder->files=files;
    avr_builder->log=ui->textBrowser;
    avr_builder->start();

}

void MCU_Studio::on_actionSerial_Communication_triggered()
{
    COM *a;
    a = new COM();

    a->show();
}

void MCU_Studio::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Application"),
             tr("The <b>MCU Studio</b> is the ATMEL AVR integrated development environment(IDE). "));
}

void MCU_Studio::on_actionNew_Screen_triggered()
{
    //new Screen    fn SysRq
    this->timer= new QTimer;
    this->hide();
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(New_Screen()));
    this->timer->start(1000);
}

void MCU_Studio::New_Screen()
{
    this->timer->stop();
    this->pixmap=QPixmap::grabWindow(QApplication::desktop()->winId());
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(this->pixmap);
    this->pixmap.save("/tmp/one.png");
    this->show();
}



void MCU_Studio::on_actionCopy_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->textEdit->toPlainText());
}

void MCU_Studio::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(ui->treeWidget);
    QAction *action = new QAction(this);
    QObject::connect(action,SIGNAL(triggered()),this,SLOT(on_actionNew_Screen_triggered()));
    action->setText("Screen");
    action->setIcon(QIcon("://img/save.png"));
    menu->addAction(action);
    menu->addSeparator();
    menu->exec(event->globalPos());
}

void MCU_Studio::on_actionPlaster_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    ui->textEdit->setText(ui->textEdit->toPlainText()+originalText);
}
