#ifndef MARKER_H
#define MARKER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QMenu>
#include <QVariant>
#include <QDebug>
#include <QDomElement>
#include "markerwindow.h"
#include "MyEnums.h"

class Marker : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Marker();
    Marker(QDomElement elem, QPixmap model);
    ~Marker();
    void initContextMenu();
    int getID();
    void setName(QString m_name);
    QString getName();
    void setDescription(QString m_description);
    QString getDescription();
    void setRelativePos(QPointF m_relativePos);
    QPointF getRelativePos();
    void setAbsolutePos(QPointF m_pos);
    QPointF getAbsolutePos();
    void setMapBoundaries(QRectF m_mapBoundaries);
    QRectF getMapBoundaries();
    void setModelNumber(int m_modelNumber);
    int getModelNumber();
    short isMarkerLocked();
    void setLocked(short locked);
    void updatePos();

signals:
    void contentChanged(int changeType);

public slots:
    void changeContent();
    void deleteMarker();
    void setLocked(bool locked);

protected:
    void setID(int m_id);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);

private:
    static int idIndex;
    int id;
    QPointF relativePos;
    QRectF mapBoundaries;
    int modelNumber;
    MarkerWindow *display;
    QMenu *contextMenu;
    QAction *actionLock;
    bool isLocked;
    QList <Marker*> neighbors;

};

#endif // MARKER_H
