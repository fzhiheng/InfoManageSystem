#ifndef FILECLASSBASE_H
#define FILECLASSBASE_H

#include <QString>

// 文件类别的一个基类，包含所有的文件
class FileClassBase
{
public:
    FileClassBase();

private:
    QString photo;
    QString x_ray;
    QString CT;
    QString MRI;
    QString survy;
    QString jaw_motion;
};

#endif // FILECLASSBASE_H
