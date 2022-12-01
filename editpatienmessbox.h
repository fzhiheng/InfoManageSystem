#ifndef EDITPATIENMESSBOX_H
#define EDITPATIENMESSBOX_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "PatienInfoBase.h"

class EditPatienMessBox: public QDialog
{
    Q_OBJECT

public:
    //构建添加病人信息界面
    EditPatienMessBox();

signals:
    //自定义信号，当添加学生信息完成后，更新主界面上的表格
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
    QPushButton * submit;
    QPushButton * cancel;
};

#endif // EDITPATIENMESSBOX_H
