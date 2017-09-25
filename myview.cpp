#include "myview.h"

MyView::MyView(QWidget *centralWidget) :
    QGraphicsView(centralWidget)
{
    this->neverSaved = true;
    this->ctrlPressed = false;
    this->currentScene = -1;
    this->currentStyle = -1;
}

MyView::~MyView()
{

}

bool MyView::loadScenario(QString fileNameArgument)
{
    QString fileName;
    if (fileNameArgument != "")
    {
        fileName = fileNameArgument;
    }
    else
    {
        fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir Scénario"),"",tr("Scénario (*.sce)"));
        if(fileName.isEmpty()) return false;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QFileInfo qfi(file);
    this->rootPath = qfi.path()+'/';
    this->rootFile = qfi.completeBaseName();

    QDomDocument* dom = new QDomDocument();
    if(!dom->setContent(&file))
    {
        file.close();
        return false;
    }

    this->neverSaved = false;

    QDomElement elem = dom->documentElement();
    QDomNode node = elem.firstChild();

    while(!node.isNull())
    {
        if(!elem.isNull())
        {
            elem = node.toElement();
            if(elem.tagName()=="Map")
            {
                MyScene *mscene = new MyScene(this);
                Map* map = mscene->loadFromXML(elem);
                QObject::connect(map,SIGNAL(mapButtonPressed(QGraphicsSceneMouseEvent*)),this,SLOT(mapClicked(QGraphicsSceneMouseEvent*)));
                this->addScene(mscene);
            }
            else if(elem.tagName()=="Sty")
            {
                this->loadStyles(elem);
            }
            else if(elem.tagName()=="Mar")
            {
                int model = elem.attribute("model","0").toInt();
                if((!scenes.isEmpty())&&(styles.size()>model))
                {
                    scenes.last()->loadMarkerFromXML(elem,styles[model].getImage());
                }
            }
            else if(elem.tagName()=="Name")
            {
                this->scenario.setName(elem.attribute("value"));
            }

        }
        // Incrément
        node = node.nextSibling();
    }

    if (!scenes.isEmpty())
    {
        this->setScene(this->scenes[0]);
        this->currentScene = 0;
    }
    this->scenario.setSaved(true);

    file.close();
    return true;
}

void MyView::loadStyles(QDomElement elem)
{
    Styles newStyle(elem.attribute("image"),elem.attribute("name","Sans nom"));
    this->styles.push_back(newStyle);
}

bool MyView::saveScenario(QString filePath)
{
    if (filePath==".sce") return false;
    // Distinction Save/Save as...
    if (filePath.isEmpty()) filePath = this->rootPath+this->rootFile;
    if(!filePath.endsWith(".sce")) filePath.append(".sce");

    // Init Dom
    QDomDocument dom;
    QDomElement elem;
    QDomProcessingInstruction qdpi = dom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
    dom.appendChild(qdpi);
    QDomElement root = dom.createElement("root");
    QDomText qdt;
    dom.appendChild(root);

    // Names
    elem = dom.createElement("Name");
    elem.setAttribute("value",this->scenario.getName());
    root.appendChild(elem);
    // Styles
    for (int i=0;i<styles.size();i++)
    {
        elem = dom.createElement("Sty");
        elem.setAttribute("name",styles[i].getName());
        elem.setAttribute("image",styles[i].getImagePath());
        root.appendChild(elem);
    }
    // Datas
    for (int i=0;i<scenes.size();i++)
    {
        // Map
        elem = dom.createElement("Map");
        elem.setAttribute("name",scenes[i]->getName());
        elem.setAttribute("image",scenes[i]->getImageName());
        root.appendChild(elem);
        // Markers
        QList<Marker *> m_markers = scenes[i]->getMarkers();
        for (int j=0;j<m_markers.size();j++)
        {
            elem = dom.createElement("Mar");
            elem.setAttribute("name",m_markers[j]->getName());
            qdt = dom.createTextNode(m_markers[j]->getDescription());
            elem.appendChild(qdt);
            elem.setAttribute("posx",m_markers[j]->getRelativePos().x());
            elem.setAttribute("posy",m_markers[j]->getRelativePos().y());
            elem.setAttribute("model",m_markers[j]->getModelNumber());
            elem.setAttribute("lock",m_markers[j]->isMarkerLocked());
            root.appendChild(elem);
        }
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        file.close();
        return false;
    }

    QFileInfo qfi(file);
    this->rootPath = qfi.path()+'/';
    this->rootFile = qfi.completeBaseName();

    scenario.setSaved(true);

    QTextStream out(&file);
    out << dom.toString();

    this->neverSaved = false;
    this->scenario.setSaved(true);

    file.close();
    return true;
}

bool MyView::hasNeverBeenSaved()
{
    return this->neverSaved;
}

void MyView::setNeverSaved(bool m_neverSaved)
{
    this->neverSaved = m_neverSaved;
}

QList<Styles> MyView::getStyles()
{
    return this->styles;
}

QList<MyScene *> MyView::getScenes()
{
    return this->scenes;
}

void MyView::setCurrentScene(int row)
{
    if ((row>=0)&&(row<scenes.size()))
    {
        this->currentScene = row;
        this->setScene(scenes[row]);
    }
}

MyScene *MyView::getCurrentScene()
{
    if (currentScene>=0)
    {
        return this->scenes[currentScene];
    }
    return NULL;
}

void MyView::setScenario(Scenario m_scenario)
{
    this->scenario = m_scenario;
}

Scenario MyView::getScenario()
{
    return this->scenario;
}

void MyView::setCurrentStyle(int m_currentStyle)
{
    this->currentStyle = m_currentStyle;
}

Styles MyView::getCurrentStyle()
{
    if (currentStyle>=0) return styles[currentStyle];
    Styles sty_err("","Err");
    return sty_err;
}

void MyView::addScene(MyScene *scene)
{
    this->scenes.push_back(scene);
    QObject::connect(scene,SIGNAL(contentChanged(int)),this,SLOT(contentUpdate(int)));
}

void MyView::clear()
{
    this->styles.clear();
    for(int i=0;i<scenes.size();i++)
    {
        delete scenes[i];
    }
    this->scenes.clear();
    this->scenario = Scenario();
    this->rootFile = "";
    this->rootPath = "";
    this->neverSaved = true;
}

void MyView::newMap(QString name, QPixmap image)
{
    Map *newMap = new Map(image);
    MyScene *mscene = new MyScene(this);
    mscene->setMap(newMap);
    mscene->setName(name);
    mscene->addItem(newMap);
    QObject::connect(newMap,SIGNAL(mapButtonPressed(QGraphicsSceneMouseEvent*)),this,SLOT(mapClicked(QGraphicsSceneMouseEvent*)));
    scenes.push_back(mscene);
    setScene(mscene);
}

void MyView::newStyle(QString name, QString imageName)
{
    Styles newStyles(imageName,name);
    styles.push_back(newStyles);
}

void MyView::wheelEvent(QWheelEvent *event)
{
    if (ctrlPressed)
    {
        if(getCurrentScene())getCurrentScene()->zoom(event->delta());
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Control)
    {
        ctrlPressed = true;
    }
}

void MyView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Control)
    {
        ctrlPressed = false;
    }
}

void MyView::contentUpdate(int changeType)
{
    this->scenario.setSaved(false);
    emit contentChanged(changeType);
}

void MyView::mapClicked(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        if (currentStyle>=0)
        {
            QPixmap currentImage = getCurrentStyle().getImage();
            QPointF position = event->pos();
            if (getCurrentScene()) getCurrentScene()->addMarkerFromPosAndImage(position,currentImage,currentStyle);
            this->scenario.setSaved(false);
            emit contentChanged(MyEnums::MarkerChanged);
        }
    }
	
    if (event->button()==Qt::RightButton)
    {
        emit rightClick();
    }
}
