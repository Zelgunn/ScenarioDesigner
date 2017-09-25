#include "dialognewitem.h"
#include "ui_dialognewitem.h"

DialogNewItem::DialogNewItem(bool MapStyAvailable, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DialogNewItem)
{
    m_ui->setupUi(this);
    displayFirstPanel();
    m_ui->pushButtonNext->setEnabled(false);
    m_scene = new QGraphicsScene();
    m_ui->graphicsView->setScene(m_scene);
    this->setWindowTitle("Nouveau...");

    m_ui->radioMap->setEnabled(MapStyAvailable);
    m_ui->radioSty->setEnabled(MapStyAvailable);

    QObject::connect(m_ui->pushButtonNext,SIGNAL(clicked()),this,SLOT(displaySecondPanel()));
    QObject::connect(m_ui->pushButtonPrevious,SIGNAL(clicked()),this,SLOT(displayFirstPanel()));
    QObject::connect(m_ui->radioMap,SIGNAL(clicked()),this,SLOT(optionSelected()));
    QObject::connect(m_ui->radioSce,SIGNAL(clicked()),this,SLOT(optionSelected()));
    QObject::connect(m_ui->radioSty,SIGNAL(clicked()),this,SLOT(optionSelected()));
    QObject::connect(m_ui->toolButton,SIGNAL(clicked()),this,SLOT(loadImage()));
}

int DialogNewItem::getOptionSelected()
{
    if(m_ui->radioSce->isChecked())
        return 0;
    if(m_ui->radioMap->isChecked())
        return 1;
    return 2;
}

QString DialogNewItem::getName()
{
    return m_ui->lineName->text();
}

QPixmap DialogNewItem::getImage()
{
    return this->m_image;
}

QString DialogNewItem::getImageBaseName()
{
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly)) return "";
    QFileInfo qfi(file);
    m_baseName = qfi.absoluteFilePath();
    file.close();
    return m_baseName;
}

DialogNewItem::~DialogNewItem()
{
    delete m_ui;
    delete m_scene;
}

void DialogNewItem::displaySecondPanel()
{
    m_ui->frame->hide();
    m_ui->frame_2->show();
    if(m_ui->radioSce->isChecked()) m_ui->groupImage->hide();
    else m_ui->groupImage->show();
    m_ui->lineName->setFocus();
}

void DialogNewItem::displayFirstPanel()
{
    m_ui->frame->show();
    m_ui->frame_2->hide();
}

void DialogNewItem::optionSelected()
{
    m_ui->pushButtonNext->setEnabled(true);
}

void DialogNewItem::loadImage()
{
    delete m_scene;
    m_scene = new QGraphicsScene();
    m_ui->graphicsView->setScene(m_scene);
    m_fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir Image"),"",tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"));
    m_ui->lineImage->setText(m_fileName);
    m_image = QPixmap(m_fileName);
    if(getOptionSelected()==2) m_image = m_image.scaled(32,32);
    QGraphicsPixmapItem *qgpi = new QGraphicsPixmapItem(m_image);
    m_scene->addItem(qgpi);
}
