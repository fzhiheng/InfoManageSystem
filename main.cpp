#include "patient.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QMap>
#include <WINDOWS.H>
#include "PhotoWidget.h"
#include "MessDispalyWidget.h"
#include "PatientDataBase.h"
#include <QDirIterator>
#include <QFileDialog>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QWidget widget;

//    QString f = QFileDialog::getExistingDirectory(&widget, "sdaf");
//    QSet<QString> t = getSubFolders(f);
//    foreach(const QString dir, t){
//        qDebug() << dir;
//    }

    widget.setWindowIcon(QIcon(":/myIcon/icon/icons8-google-tag-manager-48.png"));
    widget.setWindowTitle("上海交通大学医学院附属第九人民医院-文件管理系统");

    PatientDataBase *my_db=new PatientDataBase("my_db.db", "my_table");
    my_db->connect();
    my_db->creat_table();

    QHBoxLayout *HBoxLayout = new QHBoxLayout(&widget);
    MessDispalyWidget * mess_widget = new MessDispalyWidget(my_db->get_db_path(), my_db->get_table_name());
    HBoxLayout->addWidget(mess_widget);
    widget.show();
    return a.exec();
}
