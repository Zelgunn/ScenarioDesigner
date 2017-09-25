#include "styles.h"

Styles::Styles(QString m_imageName, QString m_name)
{
    this->imageName = m_imageName;
    if (!m_imageName.isEmpty()) this->image = QPixmap(m_imageName,0,Qt::AutoColor);
    this->name = m_name;
}

void Styles::setImage(QString m_imagePath)
{
    this->imageName = m_imagePath;
    this->image = QPixmap(m_imagePath,0,Qt::AutoColor);
}

QPixmap Styles::getImage()
{
    return this->image;
}

QString Styles::getImagePath()
{
    return this->imageName;
}

void Styles::setName(QString m_name)
{
    this->name = m_name;
}

QString Styles::getName()
{
    return this->name;
}
