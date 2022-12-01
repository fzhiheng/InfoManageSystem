#ifndef EDITPATIENTMESSBOX_H
#define EDITPATIENTMESSBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>

#include "PatienInfoBase.h"
#include "Tool.h"


//自定义添加病人信息窗口

class EditPatientMessBox : public QDialog
{
    Q_OBJECT
public:
    //构建添加学生信息界面
    EditPatientMessBox();
signals:
    //自定义信号，当添加信息完成后，更新主界面上的表格
    void closeBox();

public slots:
    //将新学生的信息写入到文件中
    void savePatMess();
public:
    //发出 closeBox 信号
    void emitCloseBox();
private:
    QLineEdit * id_number;
    QLineEdit * name;
    QLineEdit * sex;
    QLineEdit * age;
    QLineEdit * attend_id;
    QPushButton * cancel;
    QPushButton * submit;
};

#endif // EDITPATIENTMESSBOX_H




