#ifndef FILEMONITORWIDGET_H
#define FILEMONITORWIDGET_H
#include <QTabWidget>
#include <QDir>
#include <QHash>
#include "PhotoWidget.h"
#include "MyListWidget.h"

class FileMonitorWidget:public QTabWidget
{
    Q_OBJECT
public:
    // 各模块的打开方式
    FileMonitorWidget(const QString root_path, const QHash<QString, QString> &exe_map);
    FileMonitorWidget(){};
    ~FileMonitorWidget();

public slots:
    void changeShowWay();
    void OnListContextMenu(const QPoint&);


private:
    QDir base_dir;
    QList<PhotoWidget*> ph_ptr;
    QList<MyListWidget*> list_ptr;
    QHash<QString,QString> icon_map;

};

#endif // FILEMONITORWIDGET_H
