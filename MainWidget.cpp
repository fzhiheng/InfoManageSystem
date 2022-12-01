#include "MainWidget.h"
#include <QDebug>
#include <QVector>
#include <QSet>
#include <QQueue>

MainWidget::MainWidget()
{
    /*
     * 主界面利用 QHBoxLayout 布局为两个区域，左边区域用 Left_Table_Box 实现，右边由 Right_Table_Box 实现
    */
    setWindowIcon(QIcon(":/logo.ico"));
    setWindowTitle("信息管理系统(上海交通大学医学院附属第九人民医院)");
    this->resize(1000,600);
    QHBoxLayout *HBoxLayout = new QHBoxLayout(this);

    QGroupBox *Left_Table_Box = createPatMess();
    QGroupBox *Right_Table_Box = createMenu();

    HBoxLayout->addWidget(Left_Table_Box,4);
    HBoxLayout->addWidget(Right_Table_Box,2);
}

//QGroupBox * MainWidget::createPatManager(){
//    QGroupBox * box = new QGroupBox("病人信息");


//}

//QGroupBox * MainWidget::createPatMonitor(){

//}


//构建病人信息面板，Left_Table_Box 中只包含一个 QTableWidget 表格控件，为了使表格尺寸自适应整个窗口，将表格添加到了 QHBoxLayout 布局工具中。
QGroupBox * MainWidget::createPatMess(){
    QGroupBox * box = new QGroupBox("病人信息");
    TableWidget = new QTableWidget;
    TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    TableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    TableWidget->setColumnCount(_INFO_KEY.size());
    TableWidget->setHorizontalHeaderLabels(_INFO_KEY);
    flushTable();
    QHBoxLayout * AutoHBoxLayout = new QHBoxLayout;
    AutoHBoxLayout->addWidget(TableWidget);

//    QVBoxLayout * v_layout = new QVBoxLayout;

    box->setLayout(AutoHBoxLayout);

    //当用户点击某个单元格时，更新列表中显示的病人信息
    connect(TableWidget,&QTableWidget::cellClicked,this,&MainWidget::flushListWidget);
    //当用户更改某个单元格内的内容时，调用 changePatMess() 方法处理
    connect(TableWidget,&QTableWidget::cellChanged,this,&MainWidget::changePatMess);
    //将用户更改某个单元格内的内容时，同时还要更新表格中显示的病人信息
    connect(TableWidget,&QTableWidget::cellChanged,this,&MainWidget::flushListWidget);
    return box;
}

/* 构建功能面板
 * 最外层有 QGroupBox 分组框，内部用 QVBoxLayout 将整个区域分为上下两部分，分别嵌入 QListWidget 和 QGridLayout
 * QGridLayout 内包含添加、删除、保存、退出按钮和 1 个实现查找功能的单行输入框。
*/
QGroupBox * MainWidget::createMenu(){
    QGroupBox * box = new QGroupBox("功能面板");
    QVBoxLayout *VBoxLayout = new QVBoxLayout;

    ListWidget = new QListWidget;
    QGridLayout * Buts = new QGridLayout;
    AddPatBtn = new QPushButton("添加");
    DelPatBtn = new QPushButton("删除");
    SaveBtn = new QPushButton("保存");
    ExitBtn = new QPushButton("退出");
    FindPatEdit = new QLineEdit;
    FindPatEdit->setPlaceholderText("输入病人相关信息查找...");
    FindPatEdit->setClearButtonEnabled(true);
    FindPatEdit->setStyleSheet("QLineEdit{padding:3,3,3,3;}");
    Buts->addWidget(AddPatBtn,0,0);
    Buts->addWidget(DelPatBtn,0,1);
    Buts->addWidget(FindPatEdit,1,0,1,0);
    Buts->addWidget(SaveBtn,2,0);
    Buts->addWidget(ExitBtn,2,1);

    VBoxLayout->addWidget(ListWidget,4);
    VBoxLayout->addLayout(Buts,2);

    box->setLayout(VBoxLayout);
    //点击添加按钮
    connect(AddPatBtn,&QPushButton::clicked,this,&MainWidget::AddSPatBox);
    //点击删除按钮
    connect(DelPatBtn,&QPushButton::clicked,this,&MainWidget::delPatFun);
    //当向输入框内输入信息后，执行 findPatMess() 方法
    connect(FindPatEdit,&QLineEdit::returnPressed,this,&MainWidget::findPatMess);
    //点击退出按钮，关闭主窗口
    connect(ExitBtn,&QPushButton::clicked,this,&MainWidget::close);
    //点击保存按钮，执行 savePatMess() 方法
    connect(SaveBtn,&QPushButton::clicked,this,&MainWidget::savePatMess);
    return box;
}


void MainWidget::AddSPatBox(){
    messBox = new AddNewPatBox;
    //当添加信息窗口关闭时，更新表格，同时清空列表中显示的信息
    QObject::connect(messBox,&AddNewPatBox::closeBox,this,&MainWidget::flushTable);
    QObject::connect(messBox,&AddNewPatBox::closeBox,ListWidget,&QListWidget::clear);
    messBox->exec();
}

//刷新表格中的内容，当界面初始化、添加和删除病人信息后，都需要对表格内容进行更新
void MainWidget::flushTable(){
    //更新表格内容前，要断开与 cellChanged 信号关联的所有槽，否则会导致程序闪退
    disconnect(TableWidget,&QTableWidget::cellChanged,0,0);
    QFile file(file_name);
    file.open(QIODevice::ReadOnly);
    QDataStream dataStr(&file);
    QString name,sex,age,id_number,attend_id;
    TableWidget->setRowCount(0);
    while(!dataStr.atEnd()){
        TableWidget->setRowCount(TableWidget->rowCount()+1);
        for(int i=0; i<_INFO_KEY.size(); ++i){
            QString tmp_info;
            dataStr>>tmp_info;
            TableWidget->setItem(TableWidget->rowCount()-1,i,new QTableWidgetItem(tmp_info));
        }
    }
    file.close();
    //完成更新表格的任务后，重新关联与 cellChanged 相关的槽。
    connect(TableWidget,&QTableWidget::cellChanged,this,&MainWidget::changePatMess);
    connect(TableWidget,&QTableWidget::cellChanged,this,&MainWidget::flushListWidget);
}

//更新列表中显示的病人信息
void MainWidget::flushListWidget(int row){
    bool info_has_show = false;
    if(ListWidget->count() > 0)
        info_has_show = true;
    for(int i=0; i<_INFO_KEY.size(); ++i){
        QString show_info = _INFO_KEY[i] + "：" + TableWidget->item(row,i)->text();
        //当列表中有信息时，直接修改即可
        if(info_has_show){
            ListWidget->item(i)->setText(show_info);
        }else{
            ListWidget->addItem(show_info);
        }
    }
}

/* 删除信息
 * 思路：将除目标病人外，其它病人的信息拷贝到一个临时文件中，然后删除原来的文件，并将临时文件的文件名改为和原文件相同的名称。
*/
void MainWidget::delPatFun()
{
    QList<QTableWidgetItem*>items = TableWidget->selectedItems();
    int info_length = _INFO_KEY.size();
    //判断用户是否在表格中选中了某个病人信息，只有选中之后，才能执行删除操作
    if(items.count() > 0){
        QSet<int> delete_rows;
        for(const auto &item :items){
            delete_rows.insert(item->row());
        }
        QStringList delete_row_str1;
        for(int rr:delete_rows){
            delete_row_str1.append(QString::number(rr+1));
        }
        delete_row_str1.sort();
        QString delete_row_str2 = delete_row_str1.join(",");

        QMessageBox::StandardButton result=QMessageBox::question(this, "删除","确定要删除第"+ delete_row_str2 +"行数据吗？");
        if(result == QMessageBox::Yes){
            QVector<QString> one_patirnt_info(info_length, "");

            QFile file(file_name);
            file.open(QIODevice::ReadOnly);
            QDataStream readDataStr(&file);

            QFile tempFile(tempFile_name);
            tempFile.open(QIODevice::WriteOnly);
            QDataStream writeDataStr(&tempFile);

            int current_row = 0;
            while (!readDataStr.atEnd()) {
                for(int i=0; i<info_length; ++i){
                    readDataStr>>one_patirnt_info[i];
                }
                if(delete_rows.find(current_row) == delete_rows.end()){
                    for(int j=0; j<info_length; ++j){
                        writeDataStr<<one_patirnt_info[j];
                    }
                }
                current_row +=1;
            }
            tempFile.close();
            file.close();
            //删除原文件，将临时文件重命名为和原文件相同的名字
            file.remove();
            tempFile.rename(file_name);
            flushTable();
            ListWidget->clear();
        }
    }else{
        QMessageBox::warning(this,"提示","请选择要删除的病人");
    }
}

//根据病人信息完成查找
void MainWidget::findPatMess()
{
    QString targrt_name = FindPatEdit->text();
    qint32 row_num = TableWidget->rowCount();
    if (targrt_name==""){//判断是否是空，如果是空就显示所有行
        for(int i=0;i<row_num;i++){
            TableWidget->setRowHidden(i,false);//为false就是显示
        }
     }else{
        QList <QTableWidgetItem *> item = TableWidget->findItems(targrt_name, Qt::MatchContains);
        //然后把所有行都隐藏
        for(int i=0;i<row_num;i++)
        {
            TableWidget->setRowHidden(i,true);//隐藏
        }
        if(!item.empty())
        {
            //恢复对应的行
            for(int i=0;i<item.count();i++)
            {
                TableWidget->setRowHidden(item.at(i)->row(),false);//回复对应的行，也可以回复列
            }
        }else{
           QMessageBox::information(this,"查找失败","当前表格中没有【"+targrt_name+"】");
        }
    }
}

/* 更改病人信息
 * 思路：逐一将病人信息拷贝到另一个临时文件中，当读取到要更改的病人信息时，将更改后的病人信息写入临时文件。最终，临时文件中存储的是更改后所有病人的信息。
 * 拷贝完成后，删除原文件，将临时文件的名称改为和原文件一样。
*/
void MainWidget::changePatMess(int row)
{
    QString name,sex,age,id_number,attend_id;
    QString PatName = TableWidget->item(row,1)->text();
    QFile file(file_name);
    file.open(QIODevice::ReadOnly);
    QDataStream readDataStr(&file);

    QFile tempFile(tempFile_name);
    tempFile.open(QIODevice::WriteOnly);
    QDataStream writeDataStr(&tempFile);

    while (!readDataStr.atEnd()) {
        readDataStr >> name >> sex>>age>>id_number>> attend_id;
        if(name != PatName){
            writeDataStr << name << sex << age << id_number << attend_id;
        }else{
            for(int i=0;i<TableWidget->columnCount();i++){
                writeDataStr<<TableWidget->item(row,i)->text();
            }
        }
    }
    tempFile.close();
    file.close();
    file.remove();
    tempFile.rename(file_name);
}

//保存病人信息，将表格中的病人信息重新存储到文件中
void MainWidget::savePatMess()
{
    QFile file(file_name);
    file.open(QIODevice::WriteOnly);
    QDataStream dataStr(&file);

    for(int i=0;i<TableWidget->rowCount();i++){
        for(int j=0;j<TableWidget->columnCount();j++){
            dataStr << TableWidget->item(i,j)->text();
        }
    }
    file.close();
    QMessageBox::information(this,"保存","保存成功！");
}
