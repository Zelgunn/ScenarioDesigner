#ifndef STYLES_H
#define STYLES_H

#include <QPixmap>
#include <QString>
#include <QDebug>

class Styles
{
public:
    Styles(QString m_imageName, QString m_name = "Sans nom");
    void setImage(QString m_imagePath);
    QPixmap getImage();
    QString getImagePath();
    void setName(QString m_name);
    QString getName();



private:
    QPixmap image;
    QString imageName;
    QString name;
};

#endif // STYLES_H
