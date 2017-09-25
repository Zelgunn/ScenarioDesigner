#ifndef DIALOGNEWITEM_H
#define DIALOGNEWITEM_H

#include <QDialog>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>

namespace Ui {
class DialogNewItem;
}

class DialogNewItem : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewItem(bool MapStyAvailable, QWidget *parent = 0);
    int getOptionSelected();
    QString getName();
    QPixmap getImage();
    QString getImageBaseName();
    ~DialogNewItem();

public slots:
    void displaySecondPanel();
    void displayFirstPanel();
    void optionSelected();
    void loadImage();

private:
    Ui::DialogNewItem *m_ui;
    QGraphicsScene *m_scene;
    QPixmap m_image;
    QString m_fileName;
    QString m_baseName;
};

#endif // DIALOGNEWITEM_H
