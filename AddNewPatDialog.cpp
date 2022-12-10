#include "AddNewPatDialog.h"
#include <QIntValidator>

AddNewPatDialog::AddNewPatDialog()
{
    this->m_pat = Patient();
    foreach (const QString& key, this->m_pat.info_key){
        // 只有需要显示的信息才会让用户在这里添加
        if (this->m_pat.info_show.value(key)){
            this->info_add.append(key);
        }
    }
    setWindowTitle("添加病人信息");
    QFormLayout *FormLayout = new QFormLayout;
    foreach (const QString& key, this->info_add){
        QLineEdit *one_opt = new QLineEdit;
        add_info_opt.append(one_opt);
        QString row_name =  this->m_pat.info_cn_map.value(key);
        if (this->m_pat.info_required.value(key)){
            row_name += "*";
        }
        if (key == "age"){
            QIntValidator* aIntValidator = new QIntValidator;
            aIntValidator->setRange(0, 199);
            one_opt->setValidator(aIntValidator);
            one_opt->setPlaceholderText("0~999");
        }
        FormLayout->addRow(row_name, one_opt);
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
    //点击提交按钮时，将添加的病人信息发送出去
    QObject::connect(submit,&QPushButton::clicked,this,&AddNewPatDialog::addPatMess);
    //点击取消按钮时，关闭添加信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&AddNewPatDialog::close);
}


void AddNewPatDialog::addPatMess(){
    bool info_completed = true;
    for(int i=0; i<this->info_add.size(); ++i){
        QString key = this->info_add[i];
        if (this->m_pat.info_required.value(key) && this->add_info_opt[i]->text()==""){
            info_completed = false;
            break;
        }else{
            continue;
        }
    }

    if(info_completed){
        QHash<QString, QString> tmp_info_map = this->m_pat.info_map;
        for(int i=0; i<this->info_add.size(); ++i){
            QString key = this->info_add[i];
            tmp_info_map[key] = this->add_info_opt[i]->text();
        }
        this->m_pat.set_info_map(tmp_info_map);
        this->close();
        emitCloseBox();
        this->m_pat.reset();
    }else{
        QMessageBox::warning(this,"提示","请将必填项填写完整",QMessageBox::Ok);
    }
}

void AddNewPatDialog::emitCloseBox(){
    emit closeBox(this->m_pat);
}

