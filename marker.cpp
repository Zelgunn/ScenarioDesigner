#include "marker.h"

int Marker::idIndex = 0;

Marker::Marker()
{
    display = new MarkerWindow();
    this->setAcceptDrops(true);
    this->setFlag(ItemIsSelectable,true);
    isLocked = false;
    neighbors = QList<Marker*>();
    QObject::connect(display,SIGNAL(contentChanged()),this,SLOT(changeContent()));
    initContextMenu();
}

Marker::Marker(QDomElement elem, QPixmap model) : Marker()
{
    // ID
    setID(elem.attribute("id","0").toInt());
    // Position
    qreal posx = elem.attribute("posx","0").toDouble();
    qreal posy = elem.attribute("posy","0").toDouble();
    QPointF markerPos(posx,posy);
    //setMapBoundaries(map->boundingRect());
    setRelativePos(markerPos);
    // Image
    model = model.scaled(16,16);
    setPixmap(model);
    setModelNumber(elem.attribute("model","0").toInt());
    // Settings
    setFlag(QGraphicsItem::ItemIsMovable);
    // Locked
    setLocked(elem.attribute("lock","false").toShort());

    // Window
        // Name
    this->display->setName(elem.attribute("name","Sans nom"));
        // Description
    this->display->setDescription(elem.text());
}

Marker::~Marker()
{
    delete actionLock;
    delete contextMenu;
    delete display;
}

void Marker::initContextMenu()
{
    contextMenu = new QMenu("Name");

    // Supprimer
    contextMenu->addAction("Supprimer",this,SLOT(deleteMarker()));

    // Vérouiller
    actionLock = new QAction("Verrouiller",this);
    actionLock->setCheckable(true);
    QObject::connect(actionLock,SIGNAL(triggered(bool)),this,SLOT(setLocked(bool)));
    contextMenu->insertAction(0,actionLock);

    // Fin Init

}

void Marker::setID(int m_id)
{
    // Id valide
    if(m_id<=idIndex)
    {
        id = m_id;
        idIndex = m_id+1;
    }
    else
    {
        idIndex++;
        id = idIndex;
    }
}

int Marker::getID()
{
    return id;
}

void Marker::setName(QString m_name)
{
    if (display->getName()!=m_name)
    {
        this->display->setName(m_name);
        emit contentChanged(MyEnums::MarkerChanged);
    }
}

QString Marker::getName()
{
    return display->getName();
}

void Marker::setDescription(QString m_description)
{
    if(display->getDescription()!=m_description)
    {
        this->display->setDescription(m_description);
        emit contentChanged(MyEnums::MarkerChanged);
    }
}

QString Marker::getDescription()
{
    return display->getDescription();
}

void Marker::setRelativePos(QPointF m_relativePos)
{
    this->relativePos = m_relativePos;
    updatePos();
}

QPointF Marker::getRelativePos()
{
    return this->relativePos;
}

void Marker::setAbsolutePos(QPointF m_pos)
{
    this->relativePos.setX(m_pos.x()/this->mapBoundaries.width());
    this->relativePos.setY(m_pos.y()/this->mapBoundaries.height());
    this->setPos(m_pos);
}

QPointF Marker::getAbsolutePos()
{
    QPointF absolutePos;
    absolutePos.setX(relativePos.x()*mapBoundaries.width());
    absolutePos.setY(relativePos.y()*mapBoundaries.height());
    return absolutePos;
}

void Marker::setMapBoundaries(QRectF m_mapBoundaries)
{
    this->mapBoundaries = m_mapBoundaries;
    updatePos();
}

QRectF Marker::getMapBoundaries()
{
    return this->mapBoundaries;
}

void Marker::setModelNumber(int m_modelNumber)
{
    if (modelNumber!=m_modelNumber)
    {
        this->modelNumber = m_modelNumber;
        emit contentChanged(MyEnums::MarkerChanged);
    }
}

int Marker::getModelNumber()
{
    return this->modelNumber;
}

short Marker::isMarkerLocked()
{
    if(isLocked)
        return 1;
    else return 0;
}

void Marker::setLocked(short locked)
{
    if (locked != 0)
        isLocked = false;
    else
        isLocked = true;
    actionLock->setChecked(isLocked);
}

void Marker::updatePos()
{
    setPos(getAbsolutePos());
}

void Marker::changeContent()
{
    emit contentChanged(MyEnums::MarkerChanged);
}

void Marker::deleteMarker()
{
    emit contentChanged(MyEnums::MarkerDeleted);
}

void Marker::setLocked(bool locked)
{
    isLocked = locked;
}

void Marker::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
    if(this->display->isHidden())
    {
        this->display->show();
    }
    else if(this->display->isActiveWindow())
    {
        this->display->hide();
    }
    else this->display->activateWindow();
}

void Marker::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if(event->button()==Qt::LeftButton)
    {
        QGraphicsItem::mouseReleaseEvent(event);
        if(isLocked)
        {
            setPos(getAbsolutePos());
        }
        else
        {
            this->relativePos.setX(pos().x()/mapBoundaries.width());
            this->relativePos.setY(pos().y()/mapBoundaries.height());
            emit contentChanged(MyEnums::MarkerChanged);
        }
    }
    if(event->button()==Qt::RightButton)
    {
        contextMenu->show();
        contextMenu->move(event->screenPos());
    }
}

QVariant Marker::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(value == true)
    {

    }
    return QGraphicsItem::itemChange(change, value);
}
