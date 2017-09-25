#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QCloseEvent>
#include <QWheelEvent>
#include "dialognewitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showPanels(bool on);
    void clear();
    void loadStylesList();
    void loadMarkersList();
    void loadScenesList();
    void loadEveryList();

public slots:
    void newAction();
    void loadScenario(QString fileNameArgument = "");
    void saveScenario();
    void saveAsScenario();
    void styleSelection();
    void resetStylesSelection();
    void updateScenario(int changeType);
    void sceneSelection(int row);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *m_ui;
};

#endif // MAINWINDOW_H
