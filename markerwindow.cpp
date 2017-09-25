#include "markerwindow.h"
#include "ui_markerwindow.h"

MarkerWindow::MarkerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarkerWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(changeTitle(QString)));
    QObject::connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(changeDescription()));
    QObject::connect(ui->boldButton,SIGNAL(clicked(bool)),this,SLOT(toBold(bool)));
    QObject::connect(ui->italicButton,SIGNAL(clicked(bool)),this,SLOT(toItalic(bool)));
    QObject::connect(ui->underlineButton,SIGNAL(clicked(bool)),this,SLOT(toUnderlined(bool)));
    QObject::connect(ui->textEdit,SIGNAL(selectionChanged()),this,SLOT(updateButtonsState()));
}

void MarkerWindow::setDescription(QString m_description)
{
    ui->textEdit->setHtml(m_description);
    ui->textEdit->moveCursor(QTextCursor::End);
}

QString MarkerWindow::getDescription()
{
    return ui->textEdit->document()->toHtml();
}

void MarkerWindow::setName(QString m_name)
{
    ui->lineEdit->setText(m_name);
    this->setWindowTitle(m_name);
}

QString MarkerWindow::getName()
{
    return ui->lineEdit->text();
}

void MarkerWindow::applyTextCharFormat(const QTextCharFormat &m_format)
{
    QTextCursor qtc = ui->textEdit->textCursor();
    if(!qtc.hasSelection())
        qtc.select(QTextCursor::WordUnderCursor);
    qtc.mergeCharFormat(m_format);
    ui->textEdit->mergeCurrentCharFormat(m_format);
}

MarkerWindow::~MarkerWindow()
{
    delete ui;
}

void MarkerWindow::changeTitle(QString m_name)
{
    if(m_name.isEmpty())
        this->setWindowTitle("Marqueur");
    else this->setWindowTitle(m_name);
    emit contentChanged();
}

void MarkerWindow::changeDescription()
{
    emit contentChanged();
}

void MarkerWindow::toBold(bool isBold)
{
    QTextCharFormat tmpCF;
    tmpCF.setFontWeight(isBold ? QFont::Bold : QFont::Normal);
    applyTextCharFormat(tmpCF);
}

void MarkerWindow::toItalic(bool isItalic)
{
    QTextCharFormat tmpCF;
    tmpCF.setFontItalic(isItalic);
    applyTextCharFormat(tmpCF);
}

void MarkerWindow::toUnderlined(bool isUnderlined)
{
    QTextCharFormat tmpCF;
    tmpCF.setFontUnderline(isUnderlined);
    applyTextCharFormat(tmpCF);
}

void MarkerWindow::updateButtonsState()
{
    QTextCursor qtc = ui->textEdit->textCursor();
    QTextCharFormat qtcf = qtc.charFormat();

    ui->boldButton->setChecked(qtcf.fontWeight()==QFont::Bold);
    ui->italicButton->setChecked(qtcf.fontItalic());
    ui->underlineButton->setChecked(qtcf.fontUnderline());
}
