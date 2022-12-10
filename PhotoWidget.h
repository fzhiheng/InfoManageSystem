#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H
#include <QTabWidget>
#include <QDir>
#include <QHash>
#include "ImageViewerWidget.h"

//extern QString IS_PHOTO;

class PhotoWidget:public  QTabWidget
{
    Q_OBJECT
public:
    PhotoWidget(QString root_path);
    void init_show_img();

public slots:
    void change_show_img(int index);

private:
    QString IS_PHOTO = "*.jpg;*.png;.jpeg;*.tiff;*.bmp";
    QDir base_dir;
    QFileInfoList all_imgs_dir;
    int imgs_size = 0;
    QVector<ImageViewer*> all_viewers;
};

#endif // PHOTO_WIDGET_H
