#ifndef PATIENINFOBASE_H
#define PATIENINFOBASE_H

#include <QString>


// 病人信息的一个基类，包含所有的病人信息
class PatientInfoBase
{

public:
    QString getName() const;
    void setName(const QString &value);

    QString getSex() const;
    void setSex(const QString &value);

    QString getAge() const;
    void setAge(const QString &value);

    QString getID() const;
    void setID(const QString &value);

    QString getAttendanceID() const;
    void setAttendanceID(const QString &value);


private:
    QString name;
    QString sex;
    QString age;
    QString id_number; //身份证号
    QString attend_id;  //就诊号
};

#endif // PATIENINFOBASE_H
