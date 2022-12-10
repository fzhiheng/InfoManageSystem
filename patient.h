#ifndef PATIENT_H
#define PATIENT_H
#include <QVector>
#include <QString>
#include <QHash>
#include <QDir>
#include <unordered_map>
#include "GlobalVar.h"

class Patient
{
public:
    Patient();
    void print_info();
    QString get_info() const;
    void reset();
    void set_info_map(const QHash<QString, QString> &info_map);
public:
    QStringList info_key = _INFO_KEY;
    QHash<QString, bool> info_required;
    QHash<QString, bool> info_show;
    QHash<QString, QString> info_map;
    QHash<QString, QString> info_cn_map;

};

#endif // PATIENT_H
