#ifndef PATIENT_H
#define PATIENT_H
#include <QVector>
#include <QString>
#include <QHash>
#include <unordered_map>
#include "infobase.h"

class Patient
{
public:
    Patient();
    void print_info();
    QString get_info() const;

private:
    QStringList info_key;
    QHash<QString, bool> info_required;
    QHash<QString, QString> info;
};

#endif // PATIENT_H
