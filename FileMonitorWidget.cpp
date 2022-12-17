#include "FileMonitorWidget.h"

#include <QDebug>
#include <QMenu>
#include <QAction>

FileMonitorWidget::FileMonitorWidget(const QString root_path, const QHash<QString, QString> &exe_map)
{
    this->base_dir = QDir(root_path);
    if(!QFile::exists(root_path)){
        return;
    }
    icon_map.insert("photo",":/myIcon/icon/icons8-collectibles-48.png");
    icon_map.insert("Xray",":/myIcon/icon/icons8-xray-48.png");
    icon_map.insert("CT",":/myIcon/icon/icons8-ct-scanner-48.png");
    icon_map.insert("MR",":/myIcon/icon/icons8-mri-48.png");
    icon_map.insert("Mo",":/myIcon/icon/icons8-jaw-642.png");
    icon_map.insert("Qu",":/myIcon/icon/icons8-survey-100.png");
    this->base_dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList all_dirs = this->base_dir.entryList();

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested (const QPoint&)),this,SLOT(OnListContextMenu(const QPoint&)));

    //  照片模块 Xray模块
    QStringList image_class = { "photo", "Xray"};
    foreach(const QString file_class, image_class){
        QString tmp_root = this->base_dir.absoluteFilePath(file_class);
        QDir tmp_dir = QDir(tmp_root);
        tmp_dir.setFilter(QDir::NoDotAndDotDot);
        if (QDir(tmp_root).exists() ){
            PhotoWidget *tmp_widget = new PhotoWidget(tmp_root);
            if (tmp_widget != nullptr){
                this->addTab(tmp_widget,QIcon(icon_map.value(file_class)), file_class);
            }
        }
    }

    // CT, MR, Mo 模块
    QStringList specail_file_class = { "CT", "MR", "Mo", "Qu"};
    foreach(const QString file_class, specail_file_class){
        QString tmp_root = this->base_dir.absoluteFilePath(file_class);
        if (QDir(tmp_root).exists()){
            QString exe_path ="";
            if (exe_map.contains(file_class)){
                exe_path = exe_map.value(file_class);
            }
            MyListWidget *tmp_widget = new MyListWidget(tmp_root,exe_path);
            this->addTab(tmp_widget, QIcon(icon_map.value(file_class)), file_class);
        }
    }
}


void FileMonitorWidget::changeShowWay(){
    int current_index = this->currentIndex();
    QString current_name = this->tabText(current_index);
    QWidget * current_widget = this->currentWidget();
    this->removeTab(current_index);

    QString tmp_root = this->base_dir.absoluteFilePath(current_name);
    if(current_widget->metaObject()->className() == QStringLiteral("MyListWidget")){
        PhotoWidget *ph_widget = new PhotoWidget(tmp_root);
        this->insertTab(current_index, ph_widget,QIcon(this->icon_map.value(current_name)), current_name);
    }else{
        MyListWidget *tmp_widget = new MyListWidget(tmp_root,"");
        this->insertTab(current_index,tmp_widget,QIcon(this->icon_map.value(current_name)), current_name);
    }
    this->setCurrentIndex(current_index);
    return;
}


void FileMonitorWidget::OnListContextMenu(const QPoint& pos){
    int current_index = this->currentIndex();
    QString current_name = this->tabText(current_index);
    if (current_name!= QString("photo") && current_name!= QString("Xray")){
        return;
    }else{
        QMenu *popMenu = new QMenu(this);
        popMenu->setStyleSheet("QMenu::item {min-width:50px;font-size: 16px}"
                             "QMenu::item:selected {background:rgba(82,130,164,1);border:1px solid rgba(82,130,164,1)}");

        QAction *openAct= new QAction(QIcon(":/myIcon/icon/icons8-switches-50.png"), "切换显示方式", this);
        openAct->setStatusTip("切换预览视图与列表视图");

        popMenu->addAction(openAct);
        connect(openAct, &QAction::triggered, this, &FileMonitorWidget::changeShowWay);
        popMenu->exec(QCursor::pos());
        delete popMenu;
        return;
    }

}

FileMonitorWidget::~FileMonitorWidget()
{
}


