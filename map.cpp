#include "map.h"

Map::Map()
{
}

Map::Map(QPixmap image)
{
    this->setPixmap(image);
}

Map::~Map()
{

}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit mapButtonPressed(event);
}

