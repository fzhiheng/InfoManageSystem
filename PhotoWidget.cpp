#include "PhotoWidget.h"
#include <QLabel>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "ImageViewerWidget.h"

//#include <QLabel>



PhotoWidget::PhotoWidget(QString root_path)
{
    this->base_dir = QDir(root_path);
    if (!this->base_dir.exists()){
        return;
    }

    // 将base_path下的所有图片作为tab选项卡
    this->base_dir.setFilter(QDir::Files);
    this->base_dir.setSorting(QDir::Name);
    this->base_dir.setNameFilters(IS_PHOTO.split(";"));
    this->all_imgs_dir = base_dir.entryInfoList();
    this->imgs_size = all_imgs_dir.size();
    this->setTabPosition(QTabWidget::South);
    foreach (const QFileInfo &img_dir, all_imgs_dir){
        ImageViewer* m_view = new ImageViewer;
        all_viewers.append(m_view);
//        this->addTab(m_view, img_dir.baseName());
         this->addTab(m_view, QIcon(":/myIcon/icon/icons8-image-32.png"), img_dir.baseName());
    }
    // 初始化界面，显示第0张图片
    this->init_show_img();

    // 更改tab，切换对应的照片
    connect(this ,&QTabWidget::currentChanged,this,&PhotoWidget::change_show_img);
}

void PhotoWidget::change_show_img(int index){
    QImage* img=new QImage;
    img->load(all_imgs_dir[index].absoluteFilePath());
    for(int i=0; i<this->imgs_size; ++i){
        this->all_viewers[i]->resetImage();
    }
    this->all_viewers[index]->setPixmap(QPixmap::fromImage(*img));
}

void PhotoWidget::init_show_img(){
    if (all_imgs_dir.size()>0){
        QImage* img=new QImage;
        img->load(all_imgs_dir[0].absoluteFilePath());
        this->all_viewers[0]->setPixmap(QPixmap::fromImage(*img));
    }
    return;
}





