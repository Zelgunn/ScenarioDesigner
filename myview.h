#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QList>
#include <QString>
#include <QFileDialog>
#include <QWheelEvent>
#include <QDebug>
#include "scenario.h"
#include "myscene.h"
#include "styles.h"

class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *centralWidget = 0);
    ~MyView();
    bool loadScenario(QString fileNameArgument = "");
    void loadStyles(QDomElement elem);
    bool saveScenario(QString filePath = "");
    bool hasNeverBeenSaved();
    void setNeverSaved(bool m_neverSaved);
    QList<Styles> getStyles();
    QList<MyScene *> getScenes();
    void setCurrentScene(int row);
    MyScene *getCurrentScene();
    void setScenario(Scenario m_scenario);
    Scenario getScenario();
    void setCurrentStyle(int m_currentStyle);
    Styles getCurrentStyle();
    void addScene(MyScene *scene);
    void clear();
    void newMap(QString name, QPixmap image);
    void newStyle(QString name, QString imageName);

signals:
    void rightClick();
    void contentChanged(int changeType);

public slots:
    void contentUpdate(int changeType);
    void mapClicked(QGraphicsSceneMouseEvent *event);

protected:
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QList<MyScene*> scenes;
    QList<Styles> styles;
    Scenario scenario;
    QString rootPath;
    QString rootFile;
    bool neverSaved;
    int currentScene;
    int currentStyle;
    bool ctrlPressed;
};

#endif // MYVIEW_H
