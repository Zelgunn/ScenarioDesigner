#ifndef MARKERWINDOW_H
#define MARKERWINDOW_H

#include <QWidget>
#include <QTextCharFormat>
#include <QFont>

#include <QDebug>
#include "MyEnums.h"

namespace Ui {
class MarkerWindow;
}

class MarkerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MarkerWindow(QWidget *parent = 0);
    void setDescription(QString m_description);
    QString getDescription();
    void setName(QString m_name);
    QString getName();
    void applyTextCharFormat(const QTextCharFormat &m_format);
    ~MarkerWindow();

private slots:
    void changeTitle(QString m_name);
    void changeDescription();
    void toBold(bool isBold);
    void toItalic(bool isItalic);
    void toUnderlined(bool isUnderlined);
    void updateButtonsState();

signals:
    void contentChanged();

private:
     Ui::MarkerWindow *ui;
};

#endif // MARKERWINDOW_H
