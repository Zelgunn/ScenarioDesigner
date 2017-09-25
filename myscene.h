#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QString>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDomDocument>
#include <QDebug>
#include <QWheelEvent>
#include "marker.h"
#include "map.h"

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = 0);
    ~MyScene();
    QString getName();
    void setName(QString m_name);
    QString getImageName();
    void setImageName(QString m_ImageName);
    QPixmap getImageMap();
    QList<Marker *> getMarkers();
    bool hasNeverBeenSaved();
    void setAlreadySaved(bool m_neverSaved);
    Map *loadFromXML(QDomElement elem);
    void loadMarkerFromXML(QDomElement elem, QPixmap model);
    void addMarkerFromPosAndImage(QPointF m_pos, QPixmap model, int modelNumber);
    void zoom(int delta);
    void setMap(Map *newMap);
    void deleteMarker(void *markerAddr);

signals:
    void contentChanged(int changeType);

public slots:
    void markersUpdate(int changeType);

private:
    QString name;
    QString imageName;
    QPixmap imageMap;
    Map *map;
    QList<Marker*> markers;

};

#endif // MYSCENE_H
