#include "PatientDataBase.h"
#include <QDebug>


PatientDataBase::PatientDataBase(const QString &dbName, const QString &tableName)
{
    this->db_name = dbName;
    this->db_path = QCoreApplication::applicationDirPath() + "/" + dbName;
    this->table_name = tableName;
}


bool PatientDataBase::connect(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->db_path);
    if (!db.open()) {
       QMessageBox::critical(0, QObject::tr("Database Error"),
                             db.lastError().text());
       return false;
    }
    return true;
}

bool PatientDataBase::creat_table(){

    if (this->connect()){
        if (is_table_exist()){
            return true;
        }
        QString crate_sql_command = QString("CREATE TABLE %1 ("
                                "name VARCHAR(40) PRIMARY KEY,"
                                "sex VARCHAR(20) not null,"
                                "age VARCHAR(3) not null,"
                                "id_number VARCHAR(18) not null,"
                                "outpatient_number VARCHAR(20) not null,"
                                "file_root VARCHAR(100) not null"
                                " )").arg(this->table_name);

        QSqlQuery *query = new QSqlQuery();
        if (query->exec(crate_sql_command)){
            delete query;
            return true;
        }
        else{
            QMessageBox::critical(0, QObject::tr("Database Error"),query->lastError().text());
            delete query;
            return false;
        }
    }
    return false;
}


bool PatientDataBase::delete_table(){
    if (this->connect()){
        QSqlQuery *query = new QSqlQuery();
        if (query->exec(QString("DROP TABLE %1").arg(this->table_name))){
            delete query;
            return true;
        }
        else{
            QMessageBox::critical(0, QObject::tr("Database Error"),query->lastError().text());
            delete query;
            return false;
        }
    }
    return false;
}

bool PatientDataBase::is_table_exist(){
    QSqlDatabase db = QSqlDatabase::database();
    if(db.tables().contains(this->table_name)){
        return true;
    }else{
        return false;
    }
}























