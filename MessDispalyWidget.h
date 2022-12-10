#ifndef MESSDISPALYWIDGET_H
#define MESSDISPALYWIDGET_H
#include <QTableView>
#include <QMainWindow>
#include <QHeaderView>
#include <QPushButton>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTreeWidget>

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QSignalMapper>

#include "PatientDataBase.h"
#include "AddNewPatDialog.h"
#include "ModifyPatDialog.h"
#include "FileMonitorWidget.h"
#include "patient.h"
#include "GlobalVar.h"



class MessDispalyWidget: public QMainWindow
{
    Q_OBJECT
public:
    MessDispalyWidget(const QString &db_file, const QString &table_name);
    ~MessDispalyWidget();
    QTableView * get_viewer(){ return this->m_viewer; };


    // 连接数据库
    bool connect();
    // 关闭数据库
    void closeDb();

    bool isTableExist(const QString& tableName);
    // 创建数据表
    bool createTable(QString sql_command);
    // 删除数据表
    bool deleteTable();

signals:
    void fileRootChange(QString file_root);
    void tableChanged();

public slots:
    // 病人信息 增删改查
    void AddPatBox();
    void insertData(const Patient &patient); // 插入单条数据
    void deleteData();
    void modifyBox();
    void modifyData(const Patient& pat);
    void findPatMess();
    void returnAllTable();

    //文件路径 增删改
    void SetFileBox();
    void DeleteFileBox();
    void ShowFileBox();
    void changePatFile(const QModelIndex &index,  const QModelIndex & pre);
    void flushTableBox(QString file_root);
    void emitSignal(QString file_root){emit fileRootChange(file_root);}
    void emitTableChangedSignal(){emit tableChanged();}
    void SetExePath(const QString file_name);
    void AutoMerge();
    void AutoClassify();


private:
    QSqlDatabase m_db;
    QSqlTableModel *m_model = nullptr ;
    QTableView *m_viewer = nullptr;
    QVBoxLayout *m_file_layout= nullptr;
    FileMonitorWidget * m_file;
    QMenuBar* menubar;
    QSignalMapper* signalMapper;
    QStringList need_add_exe_path = {"root", "CT", "MR", "Mo"};
    QHash<QString, QString> exe_map;
    QDir program_path;


    // 病人信息 增删改查
    QPushButton * AddPatBtn;
    QPushButton * DelPatBtn;
    QPushButton * ModifyBtn;
    QPushButton * ReturnBtn;
    QLineEdit * FindPatEdit;

    //文件路径 设置与删除
    QPushButton * SetFileBtn;
    QPushButton * DelFileBtn;
    QPushButton * ShowFileBtn;


    QGroupBox * createMessBox();
    QGroupBox * createFileBox();
    void createMenu();
    int get_selected_one_row();
    QList<int> get_selected_rows();
    QString fileRootHasBinded(const QString &file_root);
    QSet<QString> getSubFolders(QString path);
    bool patHasExist(const QString pat_name);
    void connectSql(const QString &db_file, const QString &table_name);
    void initExePath();



};

#endif // MESSDISPALYWIDGET_H
