#include "PatienInfoBase.h"

QString PatientInfoBase::getName() const
{
    return name;
}

void PatientInfoBase::setName(const QString &value)
{
    name = value;
}

QString PatientInfoBase::getSex() const
{
    return sex;
}

void PatientInfoBase::setSex(const QString &value)
{
    sex = value;
}

QString PatientInfoBase::getAge() const
{
    return age;
}

void PatientInfoBase::setAge(const QString &value)
{
    age = value;
}

QString PatientInfoBase::getID() const
{
    return id_number;
}

void PatientInfoBase::setID(const QString &value)
{
    id_number = value;
}

QString PatientInfoBase::getAttendanceID() const
{
    return attend_id;
}

void PatientInfoBase::setAttendanceID(const QString &value)
{
    attend_id = value;
}

