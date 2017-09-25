#ifndef MARKERLINK_H
#define MARKERLINK_H

#include <QGraphicsItem>
#include <QDomElement>
#include <marker.h>

class MarkerLink : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    MarkerLink(Marker* src, Marker* dest);
    MarkerLink(QDomElement element);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:

private:
    Marker* m_src;
    Marker* m_dest;

};

#endif // MARKERLINK_H
