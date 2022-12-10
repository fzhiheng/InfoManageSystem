#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QObject>
#include <QDir>
#include <QFileInfoList>
#include <QProcess>
#include <QListWidget>

class MyListWidget:public QListWidget
{
    Q_OBJECT;
public:
    MyListWidget(QString file_root, QString program_path);


public slots:
    void openFile(QListWidgetItem *item);
    void OnListContextMenu(const QPoint&);
    void openFileWithSetting();


private:
    QDir file_root;         // 文件夹根路径
    QDir current_root;      // 当前展示文件夹根路径
    QString program_path;   // 打开文件外部应用程序的路径

    void showFileInfoList(QFileInfoList list);
    void slotShow(QDir dir);

};

#endif // MYLISTWIDGET_H
