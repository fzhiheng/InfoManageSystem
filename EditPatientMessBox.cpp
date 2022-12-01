#include "EditPatientMessBox.h"
#include "PatienInfoBase.h"

//构建添加病人主界面
EditPatientMessBox::EditPatientMessBox()
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
    */
//    setWindowIcon(QIcon(":/logo.ico"));
//    setWindowFlags(Qt::Dialog);
    setWindowTitle("添加病人信息");
    QVBoxLayout * VBox = new QVBoxLayout;

    // 制作一个表格布局，放置病人属性
    QFormLayout *FormLayout = new QFormLayout;
    name = new QLineEdit;
    sex = new QLineEdit;
    age = new QLineEdit;
    id_number = new QLineEdit;
    attend_id = new QLineEdit;
    FormLayout->addRow("姓 名：",name);
    FormLayout->addRow("性 别：",sex);
    FormLayout->addRow("年 龄：",age);
    FormLayout->addRow("身份证：",id_number);
    FormLayout->addRow("就诊号：",attend_id);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    // 放置一个水平布局，提交与取消按钮
    QHBoxLayout * HBox = new QHBoxLayout;
    submit = new QPushButton("提交");
    cancel = new QPushButton("取消");
    HBox->addWidget(submit);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout,4);
    VBox->addLayout(HBox,1);
    this->setLayout(VBox);
    //点击提交按钮时，将新信息保存到文件中
    QObject::connect(submit,&QPushButton::clicked,this,&EditPatientMessBox::savePatMess);
    //点击取消按钮时，关闭添加信息窗口
    QObject::connect(cancel,&QPushButton::clicked,this,&EditPatientMessBox::close);
}

//当用户添加提交按钮时，保存学生信息
void EditPatientMessBox::savePatMess(){
    //确保所有信息填写后，将信息写入到文件中，否则提示用户将信息填写完整
    if(this->id_number->text() !="" && this->name->text()!="" && this->sex->text()!="" && this->sex->text()!=""){
        PatientInfoBase patient;
        patient.setName(this->name->text());
        patient.setSex(this->sex->text());
        patient.setAge(this->age->text());
        patient.setID(this->id_number->text());
        patient.setAttendanceID(this->attend_id->text());

        QFile file(file_name);
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        QDataStream dataStr(&file);
        dataStr << patient.getName() << patient.getSex() <<  patient.getAge() <<patient.getID()
                << patient.getAttendanceID();
        file.close();
        this->close();
        emitCloseBox();
    }else{
        QMessageBox::warning(this,"提示","请将信息填写完整",QMessageBox::Ok);
    }
}

void EditPatientMessBox::emitCloseBox(){
    emit closeBox();
}
