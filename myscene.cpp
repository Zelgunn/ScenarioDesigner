#include "myscene.h"

MyScene::MyScene(QObject *parent) :
    QGraphicsScene(parent)
{
    map = NULL;
}

MyScene::~MyScene()
{
    for(int i=0;i<markers.size();i++)
    {
        delete markers[i];
    }
    markers.clear();
    delete map;
}

QString MyScene::getName()
{
    return this->name;
}

void MyScene::setName(QString m_name)
{
    this->name = m_name;
}

QString MyScene::getImageName()
{
    return this->imageName;
}

void MyScene::setImageName(QString m_ImageName)
{
    this->imageName = m_ImageName;
}

QPixmap MyScene::getImageMap()
{
    return this->imageMap;
}

QList<Marker *> MyScene::getMarkers()
{
    return this->markers;
}

Map* MyScene::loadFromXML(QDomElement elem)
{
    this->imageName = elem.attribute("image");
    this->imageMap = QPixmap(imageName,0,Qt::AutoColor);
    this->name = elem.attribute("name","Sans nom");
    map = new Map(this->imageMap);
    this->addItem(map);
    return map;
}

void MyScene::loadMarkerFromXML(QDomElement elem, QPixmap model)
{
    Marker* newMarker = new Marker(elem, model);
    // Connections
    QObject::connect(newMarker,SIGNAL(contentChanged(int)),this,SLOT(markersUpdate(int)));
    // Adding
    this->markers.push_back(newMarker);
    this->addItem(newMarker);
}

void MyScene::addMarkerFromPosAndImage(QPointF m_pos, QPixmap model, int modelNumber)
{
    Marker* newMarker = new Marker();
    // Name
    newMarker->setName("Nouveau marqueur");
    // Position
    newMarker->setMapBoundaries(itemsBoundingRect());
    m_pos.setX((m_pos.x()*itemsBoundingRect().width()/map->boundingRect().width())-8);
    m_pos.setY((m_pos.y()*itemsBoundingRect().height()/map->boundingRect().height())-8);
    newMarker->setAbsolutePos(m_pos);

    // Image
    model = model.scaled(16,16);
    newMarker->setPixmap(model);
    newMarker->setModelNumber(modelNumber);
    // Settings
    newMarker->setFlag(QGraphicsItem::ItemIsMovable);
    // Connections
    QObject::connect(newMarker,SIGNAL(contentChanged(int)),this,SLOT(markersUpdate(int)));
    // Adding
    this->markers.push_back(newMarker);
    this->addItem(newMarker);
}

void MyScene::zoom(int delta)
{
    float factor;
    if(delta>0) factor = 2;
    else factor = 0.5;

    map->setScale(map->scale()*factor);

    for(int i=0;i<markers.size();i++)
    {
        QRectF newPos = markers[i]->getMapBoundaries();
        newPos.setWidth(newPos.width()*factor);
        newPos.setHeight(newPos.height()*factor);
        markers[i]->setMapBoundaries(newPos);
    }
    this->setSceneRect(this->itemsBoundingRect());
}

void MyScene::setMap(Map *newMap)
{
    this->map = newMap;
}

void MyScene::deleteMarker(void* markerAddr)
{
    for(int i=0;i<markers.size();i++)
    {
        if (markers[i]==markerAddr)
        {
            Marker* tmpMarker = markers[i];
            markers.removeAt(i);
            delete tmpMarker;
        }
    }
}

void MyScene::markersUpdate(int changeType)
{  
    if (changeType==MyEnums::MarkerDeleted)
    {
        deleteMarker(QObject::sender());
    }
    emit contentChanged(changeType);
}
