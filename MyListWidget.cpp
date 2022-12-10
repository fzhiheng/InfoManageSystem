#include "MyListWidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QMenu>

MyListWidget::MyListWidget(QString fileRoot, QString programPath)
{
    this->file_root = QDir(fileRoot);
    this->program_path = programPath;
    this->current_root = this->file_root;
    this->setContextMenuPolicy(Qt::CustomContextMenu); // 支持右键菜单
    QFileInfoList list;

    list = this->current_root.entryInfoList();
    showFileInfoList(list);
    connect(this,&QListWidget::itemDoubleClicked,this,&MyListWidget::openFile);
    if(programPath!="" && QFile::exists(programPath)){
        connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(OnListContextMenu(const QPoint&)));
    }
}

void MyListWidget::OnListContextMenu(const QPoint& pos){
    QListWidgetItem* curItem = this->itemAt(pos);
    if(curItem == NULL)
        return;
    QString str = curItem->text();
    QDir dir;
    dir = this->current_root;
    if(dir.cd(str))
    {
       return;
    }
    QMenu *popMenu = new QMenu(this);
    popMenu->setStyleSheet("QMenu::item {min-width:50px;font-size: 16px}"
                         "QMenu::item:selected {background:rgba(82,130,164,1);border:1px solid rgba(82,130,164,1)}");

    QAction *openAct= new QAction(QIcon(":/myIcon/icon/icons8-opened-folder-48.png"), "以设定方式打开", this);
    openAct->setStatusTip("用选定的外部程序打开此文件");

    popMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MyListWidget::openFileWithSetting);
    popMenu->exec(QCursor::pos());
    delete popMenu;
}


void MyListWidget::openFileWithSetting(){
    QListWidgetItem *item = this->currentItem();
    QString str = item->text();
    QDir dir;
    dir = this->current_root;
    if(!dir.cd(str))
    {
        QStringList arguments;
        QString start_path = this->file_root.absoluteFilePath(str);
        arguments << "/c" << start_path;
        QProcess* process = new QProcess(this);
        process->start(this->program_path,arguments);  //开启新进程打开文件
     }
    return;
}


void MyListWidget::showFileInfoList(QFileInfoList list)
{
    this->clear();
    for(int i=0; i<list.count() ;i++)
    {
        QFileInfo tmpFileInfo = list.at(i);
        if(tmpFileInfo.isDir())    //为目录则添加文件夹图标及对应文件名
        {
            QIcon icon(":/myIcon/icon/icons8-folder-30.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName);
            this->addItem(tmp);
        }
        else if(tmpFileInfo.isFile())    //为普通文件则添加普通文件图标及对应文件名
        {
            QIcon icon(":/myIcon/icon/icons8-file-32.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName);
            this->addItem(tmp);
        }
    }
}


void MyListWidget::openFile(QListWidgetItem *item){
    QString str = item->text();
    QDir dir;
    dir = this->current_root;
    if(!dir.cd(str))         //如果进入失败，则为普通文件，创建新的进程打开对应文件
    {
        QStringList arguments;
        QString start_path = this->file_root.absoluteFilePath(str);
        arguments << "/c" << start_path;

        QProcess* process = new QProcess(this);
        process->start("cmd.exe",arguments);  //开启新进程打开文件
        return;
    }

    // 禁止访问根路径上层的文件夹
    if (dir == this->file_root.absoluteFilePath("..")){
        dir = this->file_root;
    }
    this->current_root = dir.absolutePath();    //更新路径
    slotShow(dir);
}

void MyListWidget::slotShow(QDir dir)
{
    QFileInfoList list = dir.entryInfoList();
    showFileInfoList(list);
}
