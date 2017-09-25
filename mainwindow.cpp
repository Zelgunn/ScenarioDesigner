#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    this->showPanels(false);
    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle("Zelgunn's Designer");

    // Connections
    QObject::connect(m_ui->actionOuvrir,SIGNAL(triggered()),this,SLOT(loadScenario()));
    QObject::connect(m_ui->actionSauvegarder,SIGNAL(triggered()),this,SLOT(saveScenario()));
    QObject::connect(m_ui->actionSauvegarder_sous,SIGNAL(triggered()),this,SLOT(saveAsScenario()));
    QObject::connect(m_ui->listStyles,SIGNAL(itemSelectionChanged()),this,SLOT(styleSelection()));
    QObject::connect(m_ui->graphicsView,SIGNAL(rightClick()),this,SLOT(resetStylesSelection()));
    QObject::connect(m_ui->graphicsView,SIGNAL(contentChanged(int)),this,SLOT(updateScenario(int)));
    QObject::connect(m_ui->actionNouveau,SIGNAL(triggered()),this,SLOT(newAction()));
    QObject::connect(m_ui->listScenes,SIGNAL(currentIndexChanged(int)),this,SLOT(sceneSelection(int)));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::showPanels(bool on)
{
    m_ui->graphicsView->setVisible(on);
    m_ui->listScenes->setVisible(on);
    m_ui->actionMarqueurs->setEnabled(on);
    m_ui->dockMarkers->setVisible(on);
    m_ui->listWidget->setVisible(on);
}

void MainWindow::clear()
{
    m_ui->graphicsView->clear();
    m_ui->listStyles->clear();
    m_ui->listWidget->clear();
    m_ui->listAddedMarkers->clear();
}

void MainWindow::loadStylesList()
{
    m_ui->listStyles->clear();
    QList<Styles> styles = m_ui->graphicsView->getStyles();
    for (int i=0;i<styles.size();i++)
    {
        // Name
        m_ui->listStyles->addItem(styles[i].getName());
        // Icon
        QIcon icon(styles[i].getImage().scaled(32,32));
        m_ui->listStyles->item(i)->setIcon(icon);
        m_ui->listStyles->item(i)->setTextAlignment(Qt::AlignHCenter);
    }
}

void MainWindow::loadMarkersList()
{
    m_ui->listAddedMarkers->clear();
    MyScene* scene = m_ui->graphicsView->getCurrentScene();
    if(!scene) return;
    QList<Styles> styles = m_ui->graphicsView->getStyles();
    QList<Marker *> markers = scene->getMarkers();
    for (int i=0;i<markers.size();i++)
    {
        // Name
        m_ui->listAddedMarkers->addItem(markers[i]->getName());
        // Icon
        QIcon icon(styles[markers[i]->getModelNumber()].getImage());
        m_ui->listAddedMarkers->item(i)->setIcon(icon);
    }
}

void MainWindow::loadScenesList()
{
    m_ui->listScenes->clear();
    QList<MyScene *> scenes = m_ui->graphicsView->getScenes();
    for (int i=0; i<scenes.size();i++)
    {
        m_ui->listScenes->addItem(scenes[i]->getName());
    }
}

void MainWindow::loadEveryList()
{
    loadMarkersList();
    loadScenesList();
    loadStylesList();
}

void MainWindow::newAction()
{
    bool MapStyAvailable = !(m_ui->graphicsView->hasNeverBeenSaved());
    DialogNewItem *newActionWindow = new DialogNewItem(MapStyAvailable,this);
    QString name,imagePath;
    QPixmap image;
    Scenario newScenario;
    if(newActionWindow->exec())
    {
        name = newActionWindow->getName();
        image = newActionWindow->getImage();
        imagePath = newActionWindow->getImageBaseName();
        switch (newActionWindow->getOptionSelected()) {
        case 0:      
            clear();
            newScenario.setName(name);
            m_ui->graphicsView->setScenario(newScenario);
            saveAsScenario();
            break;
        case 1:
            m_ui->graphicsView->newMap(name,image);
            m_ui->graphicsView->getScenes().last()->setImageName(imagePath);
            showPanels(true);
            loadEveryList();
            break;
        default:
            m_ui->graphicsView->newStyle(name,imagePath);
            loadStylesList();
            break;
        }
    }
    delete newActionWindow;
}

void MainWindow::loadScenario(QString fileNameArgument)
{
    this->clear();
    if(m_ui->graphicsView->loadScenario(fileNameArgument))
    {
        this->showPanels(true);
        loadEveryList();
    }
}

void MainWindow::saveScenario()
{
    if(m_ui->graphicsView->hasNeverBeenSaved())
    {
        this->saveAsScenario();
        return;
    }
    if(m_ui->graphicsView->saveScenario())
    {

    }
}

void MainWindow::saveAsScenario()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Ouvrir Scénario"),"",tr("Scénario (*.sce)"));
    if(!fileName.endsWith(".sce")) fileName.append(".sce");
    if(m_ui->graphicsView->saveScenario(fileName))
    {

    }
}

void MainWindow::styleSelection()
{
    int row = m_ui->listStyles->currentRow();
    //if(row<0) return;
    // Selection
    Styles selectedStyles = m_ui->graphicsView->getStyles()[row];
    QPixmap image = selectedStyles.getImage().scaled(16,16);
    QIcon icon(image);
    QListWidgetItem *item = new QListWidgetItem(selectedStyles.getName());
    m_ui->listWidget->clear();
    m_ui->listWidget->addItem(item->text());
    m_ui->listWidget->item(0)->setIcon(icon);
    // Marker mode
    QCursor selectedCursor(image);
    m_ui->graphicsView->setCursor(selectedCursor);
    m_ui->graphicsView->setCurrentStyle(row);
}

void MainWindow::resetStylesSelection()
{
    m_ui->listStyles->clearSelection();
    m_ui->graphicsView->setCursor(Qt::ArrowCursor);
    m_ui->graphicsView->setCurrentStyle(-1);
    m_ui->listWidget->clear();


}

void MainWindow::updateScenario(int changeType)
{
    if((changeType==MyEnums::MarkerChanged)||
            changeType==MyEnums::MarkerDeleted)
    {
        loadMarkersList();
    }
}

void MainWindow::sceneSelection(int row)
{
    m_ui->graphicsView->setCurrentScene(row);
    loadMarkersList();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!m_ui->graphicsView->getScenario().isSaved())
        {
            QMessageBox qmb;
            qmb.setText("Il y a des modifications non sauvegardées.\nVoulez-vous sauvegarder ?");
            qmb.setInformativeText("Sauvegarder ?");
            qmb.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            qmb.setDefaultButton(QMessageBox::Save);
            int ret = qmb.exec();
            switch(ret)
            {
            case QMessageBox::Cancel:
                event->ignore();
                break;
            case QMessageBox::Discard:
                event->accept();
                break;
            case QMessageBox::Save:
                saveScenario();
                event->accept();
                break;
            }
    }
}
