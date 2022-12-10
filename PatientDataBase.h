#ifndef PATIENTDATABASE_H
#define PATIENTDATABASE_H
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QCoreApplication>


/* 维护一个数据库，数据库中每行保存的数据一个病人的所有信息 */

class PatientDataBase
{
public:
    PatientDataBase(){};
    PatientDataBase(const QString &dbName, const QString &tableName);

    bool connect();
    bool creat_table();
    bool delete_table();
    bool is_table_exist();

    QString get_table_name()const{return this->table_name;}
    QString get_db_path() const {return this->db_path;}
    QString get_db_name() const {return this->db_name;}


private:
    QString db_name = "patInfo.db";
    QString db_path = QCoreApplication::applicationDirPath() + "/" + db_name;
    QString table_name = "my_table";
};

#endif // PATIENTDATABASE_H
