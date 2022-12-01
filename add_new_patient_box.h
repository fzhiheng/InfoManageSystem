#ifndef ADDNEWPATBOX_H
#define ADDNEWPATBOX_H
#include <QVector>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDataStream>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include "infobase.h"
#include "Tool.h"


class AddNewPatBox: public QDialog
{
    Q_OBJECT
public:
    AddNewPatBox();
    void emitCloseBox();

signals:
    //自定义信号，当添加信息完成后，更新主界面上的表格
    void closeBox();

public slots:
    void savePatMess();

private:
    QVector<QLineEdit*> add_info_opt;
    QPushButton * cancel;
    QPushButton * submit;

};

#endif // ADDNEWPATBOX_H
