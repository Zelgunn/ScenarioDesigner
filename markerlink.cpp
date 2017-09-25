#include "markerlink.h"

MarkerLink::MarkerLink(Marker* src, Marker* dest)
{
    m_src = src;
    m_dest = dest;
}

MarkerLink::MarkerLink(QDomElement element)
{

}

QRectF MarkerLink::boundingRect() const
{
    return QRectF();
}

void MarkerLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
