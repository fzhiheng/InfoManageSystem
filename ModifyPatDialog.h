#ifndef MODIFYPATDIALOG_H
#define MODIFYPATDIALOG_H
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


class ModifyPatDialog: public QDialog
{
    Q_OBJECT
public:
    ModifyPatDialog(const Patient &pat);
    void emitCloseBox();

signals:
    void closeBox(Patient pat);

public slots:
    void modifyPatMess();

private:

    QVector<QLineEdit*> add_info_opt;
    QPushButton * cancel;
    QPushButton * submit;
    Patient m_pat;
    QStringList info_modify;
};



#endif // MODIFYPATDIALOG_H
