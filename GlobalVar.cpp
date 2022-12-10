#include "GlobalVar.h"

QStringList _INFO_KEY = {"name", "sex", "age", "id_number", "outpatient_number", "file_root"};
QStringList _INFO_KEY_CN = {"姓名", "性别", "年龄","身份证号","就诊号","资料根目录"};
QVector<bool> _INFO_REQUIRED = {true, false, false, false, false, false};
QVector<bool> _INFO_SHOW = {true, true, true, true, true, false};
QString _EXE_PATH = "program.dat";

QStringList _FILE_CLASS = {"Ph", "Xray", "CT", "MR", "Qu", "Mo"};
