#include "ModifyPatDialog.h"
#include <QIntValidator>

ModifyPatDialog::ModifyPatDialog(const Patient &pat)
{
    this->m_pat = pat;
    QString name="";
    foreach (const QString& key, this->m_pat.info_key){
        // 作为主键值的姓名不允许修改
        if (key=="name")
        {
            name = this->m_pat.info_map.value(key);
            continue;
        }
        if (this->m_pat.info_show.value(key)){
            this->info_modify.append(key);
        }
    }
    QStringList title;
    title << "修改" << name << "信息";
    setWindowTitle(title.join(""));
    QFormLayout *FormLayout = new QFormLayout;
    foreach (const QString& key, this->info_modify){
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
        one_opt->setText(this->m_pat.info_map.value(key));
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
    //点击提交按钮时，将修改后的病人信息发送出去
    QObject::connect(submit,&QPushButton::clicked,this,&ModifyPatDialog::modifyPatMess);
    //点击取消按钮时，关闭添加信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&ModifyPatDialog::close);

}

void ModifyPatDialog::modifyPatMess(){
    bool info_completed = true;
    for(int i=0; i<this->info_modify.size(); ++i){
        QString key = this->info_modify[i];
        if (this->m_pat.info_required.value(key) && this->add_info_opt[i]->text()==""){
            info_completed = false;
            break;
        }else{
            continue;
        }
    }

    if(info_completed){
        QHash<QString, QString> tmp_info_map = this->m_pat.info_map;
        for(int i=0; i<this->info_modify.size(); ++i){
            QString key = this->info_modify[i];
            tmp_info_map[key] = this->add_info_opt[i]->text();
        }
        this->m_pat.set_info_map(tmp_info_map);
        this->close();
        emitCloseBox();
    }else{
        QMessageBox::warning(this,"提示","请将必填项填写完整",QMessageBox::Ok);
    }
}

void ModifyPatDialog::emitCloseBox(){
    emit closeBox(this->m_pat);
}
