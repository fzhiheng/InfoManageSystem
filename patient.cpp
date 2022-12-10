#include "patient.h"
#include <QDebug>

Patient::Patient()
{
    this->info_key = _INFO_KEY;
    for (int i=0; i<this->info_key.size(); ++i){
        this->info_required.insert(this->info_key[i], _INFO_REQUIRED[i]);
        this->info_show.insert(this->info_key[i], _INFO_SHOW[i]);
        this->info_map.insert(this->info_key[i],"");
        this->info_cn_map.insert(this->info_key[i],_INFO_KEY_CN[i]);
    }
}

void Patient::print_info(){
    qDebug() << "打印病人信息";
    foreach(const QString &one_info, this->info_key){
        QString required;
        if (this->info_required[one_info])
            required = "yes";
        else
            required = "no";
        qDebug() << one_info << " "<< this->info_map[one_info] << "required: " + required;
    }
}

QString Patient::get_info() const{
    QString output_info;
    foreach(QString one_info, this->info_key){
        output_info +=  one_info;
        output_info += this->info_map[one_info];
    }
    return output_info;
}

void Patient::reset(){
    for (int i=0; i<this->info_key.size(); ++i){
        this->info_map.insert(this->info_key[i],"");
    }
}


void Patient::set_info_map(const QHash<QString, QString> &infoMap){
    this->info_map = infoMap;
}
