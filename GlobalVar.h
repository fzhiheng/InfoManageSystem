#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <QVector>
#include <QString>

extern QStringList _INFO_KEY;          // 数据库中每行存放的信息
extern QStringList _INFO_KEY_CN;
extern QVector<bool> _INFO_REQUIRED;   // 是否必须提供
extern QVector<bool> _INFO_SHOW;       // 是否在界面上可视化

extern QStringList _FILE_CLASS;    // 所有待分类的文件

extern QString _EXE_PATH;

#endif // GLOBALVAR_H
