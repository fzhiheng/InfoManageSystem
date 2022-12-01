#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QDataStream>
#include "Tool.h"
#include "EditPatientMessBox.h"
#include "add_new_patient_box.h"
#include "infobase.h"

//自定义主窗口
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    //构建主窗口界面
    MainWidget();
    //当用户点击添加按钮时，弹出添加病人信息窗口
    void AddSPatBox();
public slots:
    //更新表格中罗列的信息
    void flushTable();

    //更新列表中显示的信息
    void flushListWidget(int row);

    //实现删病人生信息功能
    void delPatFun();

    //实现查找病人信息功能
    void findPatMess();

    //实现修改病人信息功能
    void changePatMess(int row);

    //实现保存病人信息功能
    void savePatMess();

private:
    //创建主窗口中的信息面板
    QGroupBox * createPatMess();
    //创建主窗口中的功能面板
    QGroupBox * createMenu();

    QGroupBox * createPatManager();

    QGroupBox * createPatMonitor();


private:
    //主窗口中的表格控件
    QTableWidget * TableWidget;
    //主窗口中的列表控件
    QListWidget *ListWidget;
    //主窗口中的添加、删除、保存、退出按钮和查找文本框控件
    QPushButton * AddPatBtn;
    QPushButton * DelPatBtn;
    QLineEdit * FindPatEdit;
    QPushButton * SaveBtn;
    QPushButton * ExitBtn;
    //弹出的信息添加窗口
    AddNewPatBox * messBox;
};

#endif // MAINWIDGET_H
