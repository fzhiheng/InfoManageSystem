#include "add_new_patient_box.h"
#include "Tool.h"

AddNewPatBox::AddNewPatBox()
{
    setWindowTitle("添加病人信息");
    for (const QString &opt_name : _INFO_KEY){
        QLineEdit *one_opt = new QLineEdit;
        add_info_opt.append(one_opt);
    }
    cancel = new QPushButton("取消");
    submit = new QPushButton("提交");

    QFormLayout *FormLayout = new QFormLayout;
    assert(_INFO_KEY.size() == this->add_info_opt.size());
    for (int i=0; i<_INFO_KEY.size(); ++i){
        QString opt_name = _INFO_KEY[i];
         QLineEdit * opt_line = this->add_info_opt[i];
        FormLayout->addRow(opt_name, opt_line);
    }
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    // 放置一个水平布局，提交与取消按钮
    QHBoxLayout * HBox = new QHBoxLayout;
    submit = new QPushButton("提交");
    cancel = new QPushButton("取消");
    HBox->addWidget(submit);
    HBox->addWidget(cancel);

    QVBoxLayout * VBox = new QVBoxLayout;
    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);
    //点击提交按钮时，将新信息保存到文件中
    QObject::connect(submit,&QPushButton::clicked,this,&AddNewPatBox::savePatMess);
    //点击取消按钮时，关闭添加信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&AddNewPatBox::close);
}

void AddNewPatBox::savePatMess(){
    bool info_completed = true;
    for(int i=0; i<_INFO_KEY.size(); ++i){
        if ( _INFO_REQUIRED[i] && this->add_info_opt[i]->text()==""){
            info_completed = false;
            break;
        }else{
            continue;
        }
    }
    if(info_completed){
        QFile file(file_name);
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        QDataStream dataStr(&file);
        for(QLineEdit * opt: this->add_info_opt){
            dataStr<<opt->text();
        }
        file.close();
        this->close();
        emitCloseBox();
    }else{
        QMessageBox::warning(this,"提示","请将信息填写完整",QMessageBox::Ok);
    }
}

void AddNewPatBox::emitCloseBox(){
    emit closeBox();
}
