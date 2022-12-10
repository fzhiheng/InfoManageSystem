#ifndef ADDNEWPATDIALOG_H
#define ADDNEWPATDIALOG_H
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
#include "GlobalVar.h"
#include "patient.h"


class AddNewPatDialog: public QDialog
{
    Q_OBJECT
public:
    AddNewPatDialog();
    void emitCloseBox();

signals:
    void closeBox(Patient pat);

public slots:
    void addPatMess();

private:

    QVector<QLineEdit*> add_info_opt;
    QPushButton * cancel;
    QPushButton * submit;
    Patient m_pat;
    QStringList info_add;

};

#endif // ADDNEWPATDIALOG_H
