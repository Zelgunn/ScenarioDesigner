#ifndef MAP_H
#define MAP_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class Map : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Map();
    Map(QPixmap image);
    ~Map();

signals:
    void mapButtonPressed(QGraphicsSceneMouseEvent *event);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // MAP_H
