#include "patient.h"
#include <QDebug>
Patient::Patient()
{
    info_key = _INFO_KEY;
    for (int i=0; i<_INFO_KEY.size(); ++i){
        info_required.insert(_INFO_KEY[i], _INFO_REQUIRED[i]);
        info.insert(_INFO_KEY[i],"");
    }
}

void Patient::print_info(){
    qDebug() << "打印病人信息";
    for(QString one_info : this->info_key){
        QString required;
        if (this->info_required[one_info])
            required = "yes";
        else
            required = "no";
        qDebug() << one_info << " "<< this->info[one_info] << "required: " + required;
    }
}

QString Patient::get_info() const{
    QString output_info;
    for(QString one_info : this->info_key){
        output_info +=  one_info;
        output_info += this->info[one_info];
    }
    return output_info;
}
